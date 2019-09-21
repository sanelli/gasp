#pragma once

#include <memory>

#include <gasp/torricelly/interpreter.hpp>

namespace gasp::torricelly::debugger {

   class torricelly_debugger {

      std::unique_ptr<interpreter::torricelly_interpreter> _interpreter;

      public:
         torricelly_debugger();
         ~torricelly_debugger();
   };
}