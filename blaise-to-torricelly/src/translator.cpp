#include <memory>
#include <vector>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

blaise_to_torricelly::translator::translator(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module) : _blaise_module(blaise_module) {}
std::shared_ptr<blaise::ast::blaise_ast_module> blaise_to_torricelly::translator::blaise_module() const { return _blaise_module; }

void blaise_to_torricelly::translator::execute(std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>>& torricelly_modules) const {
 // TODO: Implement me
}
