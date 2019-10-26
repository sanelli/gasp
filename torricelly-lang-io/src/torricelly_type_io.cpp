#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const std::shared_ptr<torricelly_type> type)
{
   return os << to_string(type);
}

char torricelly::to_binary(torricelly_type_type type)
{
   switch (type)
   {
   case torricelly_type_type::UNDEFINED:
      return 'u';
   case torricelly_type_type::SYSTEM:
      return 's';
   case torricelly_type_type::ARRAY:
      return 'a';
   case torricelly_type_type::STRUCTURED:
      return 'z';
   default:
      throw torricelly_error("Unknown torricelly type while converting type to binary format");
   }
}

char torricelly::to_binary(torricelly_system_type_type type)
{
   switch (type)
   {
   case torricelly_system_type_type::UNDEFINED:
      return 'u';
   case torricelly_system_type_type::VOID:
      return 'v';
   case torricelly_system_type_type::INTEGER:
      return 'i';
   case torricelly_system_type_type::FLOAT:
      return 'f';
   case torricelly_system_type_type::DOUBLE:
      return 'd';
   case torricelly_system_type_type::CHAR:
      return 'c';
   case torricelly_system_type_type::BOOLEAN:
      return 'b';
   case torricelly_system_type_type::STRING_LITERAL:
      return 's';
   default:
      throw torricelly_error("Unknown torricelly system type while converting type to binary format");
   }
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, torricelly_type_type type)
{
   return os << to_binary(type);
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, torricelly_system_type_type type)
{
   return os << to_binary(type);
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const std::shared_ptr<torricelly_type> type)
{
   os << type->type_type();
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      break;
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(type);
      os << system_type->system_type();
   }
   break;
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(type);
      os << array_type->underlying_type();
      os << (int32_t)array_type->dimensions();
      for (auto dimension = 0U; dimension < array_type->dimensions(); ++dimension)
      {
         auto dim = array_type->dimension(dimension);
         os << (dim != torricelly_array_type::undefined_dimension()
                    ? (int32_t)array_type->dimensions()
                    : (int32_t)0);
      }
   }
   break;
   case torricelly_type_type::STRUCTURED:
      break;
   default:
      throw torricelly_error("Unknown type while converting type to binary format");
   }
   return os;
}