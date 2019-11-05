#pragma once

#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include <gasp/torricelly/impl/debugger.hpp>
#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

class torricelly_debugger_command_bf_load : public torricelly_debugger_command
{
   bool load(std::ostream& out, const std::vector<std::string> parameters);
public:
   torricelly_debugger_command_bf_load(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_bf_load();

   std::string command() const override;
   std::string description() const override;
   bool execute(std::ostream &out, const std::vector<std::string> &parameters) override;
};

} // namespace gasp::torricelly::debugger