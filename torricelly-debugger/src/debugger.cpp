#include <memory>

#include <gasp/torricelly/debugger.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::debugger;

torricelly_debugger::torricelly_debugger() : _interpreter(nullptr) {}
torricelly_debugger::~torricelly_debugger() {}