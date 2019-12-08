#include <random>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include <gasp/torricelly/interpreter.hpp>

using namespace std;
using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

namespace gasp::torricelly::native::strings
{

inline std::string __char_array_to_string(std::shared_ptr<torricelly_activation_record_local_multidimensional_array> str)
{
   std::stringstream stream;
   for (int index = 0; str->get(index)._char != '\0'; ++index)
      stream << str->get(index)._char;
   return stream.str();
}

template <typename TType>
inline void __from_string(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context,
                          std::function<TType(std::string)> converter)
{
   auto char_array = context->pop().get_array_pointer();
   auto str = __char_array_to_string(char_array);

   auto value = converter(str);
   auto result = interpreter::torricelly_activation_record_local::make(value);

   context->push(result);
}

extern "C" void str2byte_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<int8_t>(context, [](std::string s) { return (int8_t)std::stoi(s); });
}

extern "C" void str2short_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<int16_t>(context, [](std::string s) { return (int16_t)std::stoi(s); });
}

extern "C" void str2integer_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<int32_t>(context, [](std::string s) { return (int32_t)std::stoi(s); });
}
extern "C" void str2long_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<int64_t>(context, [](std::string s) { return (int64_t)std::stoi(s); });
}
extern "C" void str2float_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<float>(context, [](std::string s) { return (float)std::stoi(s); });
}
extern "C" void str2double_ac1xu(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __from_string<double>(context, [](std::string s) { return (double)std::stoi(s); });
}

inline void __fill_array_from_string(std::shared_ptr<torricelly_activation_record_local_multidimensional_array> char_array,
                                     const std::string &str)
{
   interpreter::torricelly_activation_record_local_union u;
   for (auto index = 0; index < str.size(); ++index)
   {
      u._char = str[index];
      char_array->set(index, u);
   }
   u._char = '\0';
   char_array->set(str.size(), u);
}

inline void __to_string(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context,
                        std::function<std::string(torricelly_activation_record_local)> converter)
{
   auto value = context->pop();

   auto char_array_value = context->pop();
   auto char_array = char_array_value.get_array_pointer();

   auto result_string = converter(value);
   __fill_array_from_string(char_array, result_string);

   context->push(char_array_value);
}

extern "C" void to_string_ac1xuy(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_byte()); });
}
extern "C" void to_string_ac1xus(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_short()); });
}
extern "C" void to_string_ac1xui(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_integer()); });
}
extern "C" void to_string_ac1xul(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_long()); });
}
extern "C" void to_string_ac1xuf(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_float()); });
}
extern "C" void to_string_ac1xud(std::shared_ptr<gasp::torricelly::interpreter::torricelly_native_context> context)
{
   __to_string(context, [](torricelly_activation_record_local local) { return std::to_string(local.get_double()); });
}

} // namespace gasp::torricelly::native::strings
