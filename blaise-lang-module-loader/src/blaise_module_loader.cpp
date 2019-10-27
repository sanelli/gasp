
#include <memory>
#include <string>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/blaise_module_loader.hpp>

using namespace gasp;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

blaise_module_loader::blaise_module_loader(std::string current_folder, const std::vector<std::string> path)
    : _current_folder(current_folder), _path(path) {}

std::shared_ptr<ast::blaise_ast_module> blaise_module_loader::get_module(std::string module) const
{
   return nullptr;
}