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

class torricelly_debugger_command_bp : public torricelly_debugger_command
{
   void list(std::ostream &out);
   void list(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine);
   void add(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip);
   void remove(std::ostream &out, std::shared_ptr<torricelly::torricelly_subroutine> subroutine, unsigned int ip);

public:
   torricelly_debugger_command_bp(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command_bp();

   std::string command() const override;
   bool execute(std::ostream &out, const std::vector<std::string> &parameters) override;
};

} // namespace gasp::torricelly::debugger