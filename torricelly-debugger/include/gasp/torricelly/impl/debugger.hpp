#pragma once

#include <memory>
#include <istream>
#include <vector>
#include <string>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

#include <gasp/torricelly/impl/debugger_command.hpp>

namespace gasp::torricelly::debugger
{

enum class torricelly_debugger_status
{
   UNLOADED,
   LOADED
};

class torricelly_debugger
{

   std::shared_ptr<interpreter::torricelly_interpreter> _interpreter;
   std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> _modules;
   torricelly_debugger_status _status;
   std::map<std::string, std::shared_ptr<torricelly_debugger_command>> _commands;

public:
   torricelly_debugger();
   ~torricelly_debugger();

   void run(std::istream &input, std::ostream &output);
   void load(std::istream &, const std::vector<std::string> &parameters);
   void unload();

   torricelly_debugger_status status() const;
   std::shared_ptr<interpreter::torricelly_interpreter> interpreter() const;
   unsigned int count_modules() const;
   std::shared_ptr<gasp::torricelly::torricelly_module> get_module(unsigned int index) const;

   void install_command(std::shared_ptr<torricelly_debugger_command> command);
};
} // namespace gasp::torricelly::debugger