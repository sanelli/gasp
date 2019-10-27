#pragma once

#include <memory>
#include <string>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/ast.hpp>

namespace gasp::blaise
{
class blaise_module_loader
{
   std::string _current_folder;
   std::vector<std::string> _path;

   blaise_module_loader(std::string current_folder, const std::vector<std::string> path);

public:
   std::shared_ptr<ast::blaise_ast_module> get_module(std::string module) const;

   friend sanelli::memory;
};
} // namespace gasp::blaise