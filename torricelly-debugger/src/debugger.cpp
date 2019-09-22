#include <memory>
#include <istream>
#include <vector>
#include <string>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;
using namespace gasp::torricelly::interpreter;
using namespace gasp::torricelly::debugger;

torricelly_debugger::torricelly_debugger() : _interpreter(nullptr), _status(torricelly_debugger_status::UNLOADED) {}
torricelly_debugger::~torricelly_debugger()
{
   unload();
   _commands.clear();
}

void torricelly_debugger::load(std::istream &stream, const std::vector<std::string> &parameters)
{

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;

   tokenizer.tokenize(stream, context);
   parser.parse(context);
   translator translator(context.module());
   translator.execute(_modules);
   auto interpreter = make_torricelly_interpreter(
       _modules.at(0),
       [this, parameters](unsigned int index) { return index < parameters.size() ? parameters.at(index) : ""; });
   interpreter->initialize();

   _status = torricelly_debugger_status::LOADED;
}

void torricelly_debugger::unload()
{
   _status = torricelly_debugger_status::UNLOADED;
   _interpreter = nullptr;
   _modules.clear();
}

torricelly_debugger_status torricelly_debugger::status() const { return _status; }
std::shared_ptr<interpreter::torricelly_interpreter> torricelly_debugger::interpreter() const { return _interpreter; }
unsigned int torricelly_debugger::count_modules() const { return _modules.size(); }
std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_debugger::get_module(unsigned int index) const { return _modules.at(index); }

void torricelly_debugger::install_command(std::shared_ptr<torricelly_debugger_command> command)
{
   _commands[command->command()] = command;
}

void torricelly_debugger::run(std::istream &input, std::ostream &output)
{
}