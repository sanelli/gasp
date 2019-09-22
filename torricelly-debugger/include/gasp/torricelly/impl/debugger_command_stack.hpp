#pragma once

#include <memory>
#include <string>
#include <vector>

#include <gasp/torricelly/impl/debugger.hpp>
#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

class torricelly_debugger_command_stack : public torricelly_debugger_command
{

public:
   torricelly_debugger_command_stack(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_stack();

   std::string command() const override;
   bool execute(const std::vector<std::string>& parameters) override;
};

} // namespace gasp::torricelly::debugger