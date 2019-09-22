#include <memory>
#include <string>
#include <vector>

#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command_stack::torricelly_debugger_command_stack(std::shared_ptr<torricelly_debugger> debugger)
    : torricelly_debugger_command(debugger) {}
torricelly_debugger_command_stack::~torricelly_debugger_command_stack() {}

std::string torricelly_debugger_command_stack::command() const { return "stack"; }

bool torricelly_debugger_command_stack::execute(const std::vector<std::string> &parameters)
{
   return true;
}