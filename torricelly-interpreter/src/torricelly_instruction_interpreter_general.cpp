#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_noop(const torricelly::torricelly_instruction &instruction)
{
   // Literlly nothing to for NOOP
}
