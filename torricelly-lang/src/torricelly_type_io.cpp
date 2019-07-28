#include <ostream>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_type_type type){
   switch(type){
      case torricelly_type_type::UNDEFINED:
         return os << "undefined";
      case torricelly_type_type::SYSTEM:
         return os << "system";
      case torricelly_type_type::STRUCTURED:
         return os << "structured";
      default:
         throw torricelly_error("Unknown torricelly type");
   }
   return os;
}
std::ostream &torricelly::operator<<(std::ostream &os, torricelly_system_type_type type) {
 switch(type){
   case torricelly_system_type_type::UNDEFINED:
      return os << "undefined";
   case torricelly_system_type_type::VOID:
      return os << "void";
   case torricelly_system_type_type::INTEGER:
      return os << "integer";
   case torricelly_system_type_type::FLOAT:
      return os << "float";
   case torricelly_system_type_type::DOUBLE:
      return os << "double";
   case torricelly_system_type_type::CHAR:
      return os << "char";
   case torricelly_system_type_type::BOOLEAN:
      return os << "boolean";
   default:
      throw torricelly_error("Unknown torricelly system type");
   }
   return os;
}