#pragma once

#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include <gasp/torricelly/impl/debugger.hpp>
#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

class torricelly_debugger_command_help : public torricelly_debugger_command
{
public:
   torricelly_debugger_command_help(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_help();

   std::string command() const override;
   std::string description() const override;
   bool execute(std::ostream &out, const std::vector<std::string> &parameters) override;
};

} // namespace gasp::torricelly::debugger