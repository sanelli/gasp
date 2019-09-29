#pragma once

#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include <gasp/torricelly/torricelly.hpp>

#include <gasp/torricelly/impl/debugger.hpp>
#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

class torricelly_debugger_command_code : public torricelly_debugger_command
{
   void print_instruction(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_subroutine> subroutine, unsigned int ip);
   void print_subroutine(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_subroutine> subroutine);
   void print_module(std::ostream &out, std::string prefix, std::shared_ptr<torricelly_module> module);

public:
   torricelly_debugger_command_code(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_code();

   std::string command() const override;
   std::string description() const override;
   bool execute(std::ostream &out, const std::vector<std::string> &parameters) override;
};

} // namespace gasp::torricelly::debugger