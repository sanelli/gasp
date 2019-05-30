#pragma once

#include <string>
#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_variable.hpp>
#include <gasp/blaise/impl/language/blaise_language_type.hpp>
#include <gasp/blaise/impl/language/blaise_language_module.hpp>

namespace gasp::blaise::language {

class blaise_module;

class blaise_subroutine {
   std::string _name;
   std::weak_ptr<blaise_module> _module;
   blaise_language_type _return_type;
   //TODO: Add parameters
   std::vector<std::shared_ptr<blaise_variable>> _variables;
public:
   blaise_subroutine(std::weak_ptr<blaise_module> module, const std::string& name);
   blaise_subroutine(std::weak_ptr<blaise_module> module, const std::string& name, blaise_language_type return_type);

   std::string name() const;
   blaise_language_type return_type() const;
   std::weak_ptr<blaise_module> module() const;

   // Variables management;
   void add_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_variable> get_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
};
}