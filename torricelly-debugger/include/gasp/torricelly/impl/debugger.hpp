#pragma once

#include <memory>
#include <istream>
#include <vector>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

enum class torricelly_debugger_status
{
   UNLOADED,
   LOADED
};

class torricelly_debugger
{

   std::shared_ptr<interpreter::torricelly_interpreter> _interpreter;
   std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> _modules;
   torricelly_debugger_status _status;
   std::map<std::string, std::shared_ptr<torricelly_debugger_command>> _commands;
   std::multimap<std::string, unsigned int> _breakpoints;
   std::vector<std::string> _parameters;

   torricelly_debugger();

public:
   ~torricelly_debugger();

   void run(std::istream &input, std::ostream &output);
   void load(typename std::vector<std::string>::const_iterator &from, typename std::vector<std::string>::const_iterator &to);
   void unload();

   torricelly_debugger_status status() const;
   std::shared_ptr<interpreter::torricelly_interpreter> interpreter() const;
   void add_module(std::shared_ptr<gasp::torricelly::torricelly_module> module);
   unsigned int count_modules() const;
   std::shared_ptr<gasp::torricelly::torricelly_module> get_module(unsigned int index) const;

   void install_command(std::shared_ptr<torricelly_debugger_command> command);
   typename std::map<std::string, std::shared_ptr<torricelly_debugger_command>>::const_iterator begin_commands() const;
   typename std::map<std::string, std::shared_ptr<torricelly_debugger_command>>::const_iterator end_commands() const;

   void add_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip);
   void remove_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip);
   bool is_breakpoint(std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip) const;
   std::pair <typename std::multimap<std::string, unsigned int>::const_iterator, typename std::multimap<std::string, unsigned int>::const_iterator> breakpoints(std::shared_ptr<torricelly::torricelly_subroutine> subroutine) const;

   friend sanelli::memory;

};


std::shared_ptr<torricelly_debugger> make_torricelly_debugger();

} // namespace gasp::torricelly::debugger