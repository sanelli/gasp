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

torricelly_type_type torricelly::torricelly_type_type_from_binary(char byte)
{
   switch (byte)
   {
   case 'u':
      return torricelly_type_type::UNDEFINED;
   case 's':
      return torricelly_type_type::SYSTEM;
   case 'a':
      return torricelly_type_type::ARRAY;
   case 'z':
      return torricelly_type_type::STRUCTURED;
   default:
      throw torricelly_error(sanelli::make_string("Cannot convert binary value '", (int)byte, "' into torricelly type type"));
   }
}

torricelly_system_type_type torricelly::torricelly_system_type_type_from_binary(char byte)
{
   switch (byte)
   {
   case 'u':
      return torricelly_system_type_type::UNDEFINED;
   case 'v':
      return torricelly_system_type_type::VOID;
   case 'i':
      return torricelly_system_type_type::INTEGER;
   case 'f':
      return torricelly_system_type_type::FLOAT;
   case 'd':
      return torricelly_system_type_type::DOUBLE;
   case 'c':
      return torricelly_system_type_type::CHAR;
   case 'b':
      return torricelly_system_type_type::BOOLEAN;
   case 's':
      return torricelly_system_type_type::STRING_LITERAL;
   default:
      throw torricelly_error(sanelli::make_string("Cannot convert binary value '", (int)byte, "' into torricelly system type type"));
   }
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, torricelly_type_type &type)
{
   char byte;
   is >> byte;
   type = torricelly_type_type_from_binary(byte);
   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, torricelly_system_type_type &type)
{
   char byte;
   is >> byte;
   type = torricelly_system_type_type_from_binary(byte);
   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, std::shared_ptr<torricelly_type> &type)
{
   torricelly_type_type type_type;
   is >> type_type;

   switch (type_type)
   {
   case torricelly_type_type::UNDEFINED:
      throw torricelly_error("Type undefined cannot be deserialized");
   case torricelly_type_type::SYSTEM:
   {
      torricelly_system_type_type system_type;
      is >> system_type;
      type = torricelly::make_torricelly_system_type(system_type);
   }
   break;
   case torricelly_type_type::ARRAY:
   {
      std::shared_ptr<torricelly_type> underlying_type;
      is >> underlying_type;
      int32_t number_of_dimentions;
      is >> number_of_dimentions;
      std::vector<unsigned int> dimensions;
      for (auto dimension_index = 0U; dimension_index < number_of_dimentions; ++dimension_index)
      {
         int32_t dimension;
         is >> dimension;
         dimensions.push_back(dimension != 0
                                  ? (unsigned int)dimension
                                  : torricelly_array_type::undefined_dimension());
      }
      type = torricelly::make_torricelly_array_type(underlying_type, dimensions);
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_error("Type structured cannot be deserialized");
      break;
   default:
      throw torricelly_error(sanelli::make_string("Cannot convert binary type with value '", (int)type->type_type(), "' into torricelly system type type"));
   }

   return is;
}
