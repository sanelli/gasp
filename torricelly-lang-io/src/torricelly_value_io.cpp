#include <ostream>
#include <cctype>
#include <sstream>
#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

std::string gasp::torricelly::to_string(torricelly_value value)
{
   std::stringstream os;
   switch (value.type()->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      break;
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(value.type());
      switch (system_type->system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         break;
      case torricelly_system_type_type::VOID:
         break;
      case torricelly_system_type_type::INTEGER:
         os << value.get_integer();
         break;
      case torricelly_system_type_type::FLOAT:
         os << value.get_float();
         break;
      case torricelly_system_type_type::DOUBLE:
         os << value.get_double();
         break;
      case torricelly_system_type_type::CHAR:
      {
         auto cvalue = value.get_char();
         if (std::isprint(cvalue))
            os << cvalue;
         else
            os << '\\' << (int)cvalue;
      }
      break;
      case torricelly_system_type_type::BOOLEAN:
         os << (value.get_boolean() ? "true" : "false");
         break;
      case torricelly_system_type_type::STRING_LITERAL:
         os << "[" << value.get_string_literal().length() << "]" << value.get_string_literal();
         break;
      default:
         throw torricelly_error("Unknown torricelly system type");
      }
   }
   break;
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(value.type());
      os << "[" << array_type->dimensions() << "] ";
      os << "(";
      bool has_defined_dimensions = true;
      for (auto dimension = 0; dimension < array_type->dimensions(); ++dimension)
      {
         if (dimension > 0)
            os << ",";
         auto dim = array_type->dimension(dimension);
         auto is_dimension_defined = dim != torricelly_array_type::undefined_dimension();
         has_defined_dimensions = has_defined_dimensions && is_dimension_defined;
         os << (is_dimension_defined ? std::to_string(dim) : "U");
      }
      os << ") {";
      if (has_defined_dimensions)
      {
         auto array = value.get_array();
         bool first = true;
         for (auto it = array->begin(); it != array->end(); ++it, first = false)
         {
            if (!first)
               os << ",";
            os << to_string(*it);
         }
      }
      os << "}";
   }
   break;
   case torricelly_type_type::STRUCTURED:
      break;
   default:
      throw torricelly_error("Unknown torricelly type");
   }
   return os.str();
}

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, torricelly_value value)
{
   return os << to_string(value);
}
