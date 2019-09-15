#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_instruction_interpreter::torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter)
   : _interpreter(interpreter){}
   
void torricelly_instruction_interpreter::execute_instruction(std::shared_ptr<torricelly::torricelly_instruction> instruction) 
{

}