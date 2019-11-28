#include <cmath>
#include <memory>
#include <functional>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;

namespace gasp::torricelly::native::matrices
{

inline void operation_on_matrices(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context,
                                  std::function<void(interpreter::torricelly_activation_record_local_union &, interpreter::torricelly_activation_record_local_union)> compute)
{
   auto columns = context->pop().get_integer();
   auto lines = context->pop().get_integer();
   auto array = context->pop().get_array_pointer();

   std::vector<unsigned int> index;
   index.resize(2);
   for (auto i = 0; i < lines; ++i)
   {
      index.at(0) = i;
      for (auto j = 0; j < columns; ++j)
      {
         index.at(1) = j;
         auto computed_index = array->index(index);
         auto value = array->get(computed_index);
         interpreter::torricelly_activation_record_local_union u;
         compute(u, value);
         array->set(computed_index, u);
      }
   }
}

extern "C" void sqrt_af2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._float = std::sqrtf(v._float); });
}

extern "C" void sqrt_ad2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._double = std::sqrt(v._double); });
}

extern "C" void log10_af2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._float = std::log10f(v._float); });
}

extern "C" void log10_ad2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._double = std::log10(v._double); });
}

extern "C" void log2_af2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._float = std::log2f(v._float); });
}

extern "C" void log2_ad2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._double = std::log2(v._double); });
}

extern "C" void log_af2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._float = std::logf(v._float); });
}

extern "C" void log_ad2xuxu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   operation_on_matrices(context, [](auto u, auto v) { u._double = std::log(v._double); });
}

} // namespace gasp::torricelly::native::matrices