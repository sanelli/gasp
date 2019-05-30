#include <ostream>
#include <stdexcept>

#include <gasp/blaise/language.hpp>

std::ostream &gasp::blaise::language::operator<<(std::ostream &os, blaise_language_expression_type type)
{
   switch(type){
      case VOID:
         return os << "void";
      case INTEGER:
         return os << "integer";
      case FLOAT:
         return os << "float";
      case CHAR:
         return os << "char";
      case STRING:
         return os << "string";
      case BOOLEAN:
         return os << "boolean";
   }
  throw std::runtime_error("Unexpected type when converting type into string representation.");
}