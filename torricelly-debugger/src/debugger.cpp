#include <memory>
#include <istream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger::torricelly_debugger()
    : _interpreter(nullptr),
      _status(torricelly_debugger_status::UNLOADED),
      _stop(false) {}
torricelly_debugger::~torricelly_debugger()
{
   unload();
   _commands.clear();
}

void torricelly_debugger::load(typename std::vector<std::string>::const_iterator &from, typename std::vector<std::string>::const_iterator &to)
{
   std::copy(from, to, std::back_inserter(_parameters));
   _interpreter = make_torricelly_interpreter(
       _modules.at(0),
       [this](unsigned int index) { return index < this->_parameters.size() ? this->_parameters.at(index) : ""; });
   _interpreter->initialize();

   _status = torricelly_debugger_status::LOADED;
}

void torricelly_debugger::unload()
{
   _status = torricelly_debugger_status::UNLOADED;
   _interpreter = nullptr;
   _modules.clear();
   _breakpoints.clear();
}

void torricelly_debugger::stop() { _stop = true; }

torricelly_debugger_status torricelly_debugger::status() const { return _status; }
std::shared_ptr<interpreter::torricelly_interpreter> torricelly_debugger::interpreter() const { return _interpreter; }
void torricelly_debugger::add_module(std::shared_ptr<gasp::torricelly::torricelly_module> module)
{
   _modules.push_back(module);
}
unsigned int torricelly_debugger::count_modules() const { return _modules.size(); }
std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_debugger::get_module(unsigned int index) const { return _modules.at(index); }

void torricelly_debugger::install_command(std::shared_ptr<torricelly_debugger_command> command)
{
   _commands[command->command()] = command;
}

typename std::map<std::string, std::shared_ptr<torricelly_debugger_command>>::const_iterator torricelly_debugger::begin_commands() const
{
   return _commands.begin();
}

typename std::map<std::string, std::shared_ptr<torricelly_debugger_command>>::const_iterator torricelly_debugger::end_commands() const
{
   return _commands.end();
}

void torricelly_debugger::add_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip)
{
   auto name = subroutine->name();
   if (!is_breakpoint(subroutine, ip))
      _breakpoints.insert(std::make_pair(name, ip));
}

void torricelly_debugger::remove_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip)
{
   auto name = subroutine->name();
   if (is_breakpoint(subroutine, ip))
   {
      auto range = _breakpoints.equal_range(name);
      for (auto it = range.first; it != range.second; ++it)
      {
         if (ip == it->second)
         {
            _breakpoints.erase(it);
            return;
         }
      }
   }
}

bool torricelly_debugger::is_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip) const
{
   auto range = _breakpoints.equal_range(subroutine->name());
   for (auto it = range.first; it != range.second; ++it)
   {
      if (ip == it->second)
         return true;
   }
   return false;
}

std::pair<typename std::multimap<std::string, unsigned int>::const_iterator, typename std::multimap<std::string, unsigned int>::const_iterator> torricelly_debugger::breakpoints(std::shared_ptr<torricelly::torricelly_subroutine> subroutine) const
{
   auto name = subroutine->name();
   return _breakpoints.equal_range(name);
}

std::shared_ptr<torricelly_debugger> gasp::torricelly::debugger::make_torricelly_debugger()
{
   auto debugger = sanelli::memory::make_shared<torricelly_debugger>();
   debugger->install_command(std::make_shared<torricelly_debugger_command_bp>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_code>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_ip>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_locals>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_stack>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_step>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_run>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_help>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_stop>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_load>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_blaise_load>(debugger));
   debugger->install_command(std::make_shared<torricelly_debugger_command_bf_load>(debugger));

   return debugger;
}

bool torricelly_debugger::run(std::istream &in, std::ostream &out)
{
   std::string last_string_command("");

   out << "Torricelly debugger. Type 'help' for more commands." << std::endl;
   while (!_stop)
   {
      std::string string_command_line;
      out << "> ";
      getline(in, string_command_line);
      sanelli::trim(string_command_line);
      if (string_command_line == "")
         string_command_line = last_string_command;
      if (string_command_line == "")
         continue;
      last_string_command = string_command_line;
      std::vector<std::string> command_line;
      sanelli::split(string_command_line, ' ', command_line);
      std::transform(command_line.begin(), command_line.end(), command_line.begin(),
                     [](std::string s) { sanelli::trim(s); return s; });
      if (command_line.size() <= 0)
         continue;
      auto string_command = command_line.at(0);
      std::vector<std::string> parameters;
      parameters.resize(command_line.size() - 1);
      auto command_line_it = command_line.begin();
      std::advance(command_line_it, 1);
      std::copy(command_line_it, command_line.end(), parameters.begin());
      command_line.clear(); // No need to keep it

      auto command_it = _commands.find(string_command);
      if (command_it == _commands.end())
      {
         out << "Cannot execute command '" << string_command << "'" << std::endl;
         continue;
      }

      try
      {
         command_it->second->execute(out, parameters);
      }
      catch (const std::exception &e)
      {
         out << "[!] Command '" << string_command << "' caused an error: " << e.what() << std::endl;
      }
   }

   return true;
}
