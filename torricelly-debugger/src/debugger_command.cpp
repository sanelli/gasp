
#include <memory>

#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::debugger;

torricelly_debugger_command::torricelly_debugger_command(std::shared_ptr<torricelly_debugger> debugger) 
  : _debugger(debugger) {}

torricelly_debugger_command::~torricelly_debugger_command() { 
   _debugger.reset();
}

std::shared_ptr<torricelly_debugger> torricelly_debugger_command::debugger() { return _debugger.lock(); }
