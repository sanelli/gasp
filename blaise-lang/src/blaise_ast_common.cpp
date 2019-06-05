#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/ast.hpp>

using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

blaise_ast::blaise_ast(unsigned int line, unsigned int column) 
   : _line(line), _column(column) { }
blaise_ast::blaise_ast(const token<blaise_token_type>& reference)
   : _line(reference.line()), _column(reference.column()) {  }
blaise_ast::blaise_ast(const token<blaise_token_type>& reference, blaise_token_type expected_type)
   : _line(reference.line()), _column(reference.column()) { 
      if(reference.type() != expected_type)
         throw blaise_ast_error(reference.line(), reference.column(), make_string("A token of type '", expected_type, "' was expected but '", reference.type(), "' was found."));
    }
unsigned int blaise_ast::line() const { return _line; }
unsigned int blaise_ast::column() const { return _column; }
