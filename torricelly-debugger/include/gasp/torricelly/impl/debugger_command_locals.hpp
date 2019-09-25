#pragma once

#include <memory>
#include <string>
#include <ostream>

#include <gasp/torricelly/torricelly.hpp>

#include <gasp/torricelly/impl/debugger.hpp>
#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

class torricelly_debugger_command_locals : public torricelly_debugger_command
{
   bool show(std::ostream& out, std::shared_ptr<gasp::torricelly::interpreter::torricelly_activation_record> activation_record, unsigned int index);
   void show(std::ostream& out, std::shared_ptr<gasp::torricelly::interpreter::torricelly_activation_record> activation_record);

public:
   torricelly_debugger_command_locals(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_locals();

   std::string command() const override;
   bool execute(std::ostream &out, const std::vector<std::string> &parameters) override;
};

} // namespace gasp::torricelly::debugger