#pragma once

#include <memory>
#include <string>
#include <vector>

namespace gasp::torricelly::debugger
{

class torricelly_debugger;

class torricelly_debugger_command
{
   std::weak_ptr<torricelly_debugger> _debugger;

protected:
   std::shared_ptr<torricelly_debugger> debugger();

public:
   torricelly_debugger_command(std::shared_ptr<torricelly_debugger> debugger);
   virtual ~torricelly_debugger_command();

   virtual std::string command() const = 0;
   virtual std::string description() const = 0;
   virtual bool execute(std::ostream& out, const std::vector<std::string>& parameters) = 0;
};

} // namespace gasp::torricelly::debugger