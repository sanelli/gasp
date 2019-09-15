#pragma once

#include <memory>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter {

class torricelly_interpreter;

class torricelly_instruction_interpreter {
private:
std::shared_ptr<torricelly_interpreter> _interpreter;
torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter);
void execute_instruction(std::shared_ptr<torricelly::torricelly_instruction> instruction);

friend gasp::torricelly::interpreter::torricelly_interpreter;
friend sanelli::memory;

};

}