#include <ostream>

#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_value value) { 

   switch(value.type()){
      case torricelly_type_type::UNDEFINED:
      break;
      case torricelly_type_type::SYSTEM:
      {
         switch(value.system_type()) { 
            case torricelly_system_type_type::UNDEFINED: break;
            case torricelly_system_type_type::VOID: break;
            case torricelly_system_type_type::INTEGER: os << value.get_integer(); break;
            case torricelly_system_type_type::FLOAT: os << value.get_float();  break;
            case torricelly_system_type_type::DOUBLE: os << value.get_double();  break;
            case torricelly_system_type_type::CHAR: os << value.get_char();  break;
            case torricelly_system_type_type::BOOLEAN: os << (value.get_boolean() ? "true" : "false");  break;
            case torricelly_system_type_type::STRING_LITERAL: os << "[" << value.get_string_literal().length() << "]" << value.get_string_literal();  break;
            default:
               throw torricelly_error("Unknown torricelly system type");
         }
      }
      break;
      case torricelly_type_type::STRUCTURED:
      break;
      default:
         throw torricelly_error("Unknown torricelly type");
   }

   return os;
}
