#include <memory>
#include <functional>

#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace std;
using namespace gasp::blaise;


gasp::blaise::blaise_parser_context::blaise_parser_context(std::function<std::shared_ptr<ast::blaise_ast_module>(std::string)> get_dependency) 
   : sanelli::parser_context<blaise_token_type>(), 
   _main_subroutine(nullptr), _module(nullptr), _current_subroutine(nullptr),
   _get_dependency(get_dependency) {
}
void gasp::blaise::blaise_parser_context::module(std::shared_ptr<ast::blaise_ast_module> module){
   _module = module;
}
std::shared_ptr<ast::blaise_ast_module> gasp::blaise::blaise_parser_context::module() const {
   return _module;
}
void gasp::blaise::blaise_parser_context::main_subroutine(std::shared_ptr<ast::blaise_ast_subroutine> subroutine){
   _main_subroutine = subroutine;
}
std::shared_ptr<ast::blaise_ast_subroutine> gasp::blaise::blaise_parser_context::main_subroutine() const {
   return _main_subroutine;
}
void gasp::blaise::blaise_parser_context::current_subroutine(std::shared_ptr<ast::blaise_ast_subroutine> subroutine){
   _current_subroutine = subroutine;
}
std::shared_ptr<ast::blaise_ast_subroutine> gasp::blaise::blaise_parser_context::current_subroutine() const {
   return _current_subroutine;
}

std::shared_ptr<ast::blaise_ast_module> gasp::blaise::blaise_parser_context::get_dependency(std::string dependency) const {
   return _get_dependency(dependency);
}