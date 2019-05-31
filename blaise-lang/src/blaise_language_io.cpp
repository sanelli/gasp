#include <ostream>
#include <stdexcept>

#include <gasp/blaise/language.hpp>

std::ostream &gasp::blaise::language::operator<<(std::ostream &os, blaise_language_type type)
{
   switch(type){
      case blaise_language_type::VOID:
         return os << "void";
      case blaise_language_type::INTEGER:
         return os << "integer";
      case blaise_language_type::FLOAT:
         return os << "float";
      case blaise_language_type::DOUBLE:
         return os << "double";
      case blaise_language_type::CHAR:
         return os << "char";
      case blaise_language_type::STRING:
         return os << "string";
      case blaise_language_type::BOOLEAN:
         return os << "boolean";
   }
  throw std::runtime_error("Unexpected type when converting type into string representation.");
}