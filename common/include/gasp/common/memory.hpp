#pragma once

#include <memory>

namespace gasp::common{
   class memory {
      public:
      template<typename TResult, typename... TParams>
      static std::shared_ptr<TResult> gasp_make_shared(TParams... args) {
         return std::shared_ptr<TResult>(new TResult(args...));
      }
   };
}