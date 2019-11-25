#include <ostream>
#include <stdexcept>
#include <vector>

#include <gasp/blaise/ast.hpp>

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, gasp::blaise::ast::blaise_ast_system_type type)
{
   return os << ast::to_string(type);
}

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, std::shared_ptr<gasp::blaise::ast::blaise_ast_type> type)
{
   return os << ast::to_string(type);
}

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, const std::vector<std::shared_ptr<gasp::blaise::ast::blaise_ast_type>> &list)
{
   for (int index = 0; index < list.size(); index++)
   {
      if (index > 0)
         os << ", ";
      os << list.at(index);
   }
   return os;
}