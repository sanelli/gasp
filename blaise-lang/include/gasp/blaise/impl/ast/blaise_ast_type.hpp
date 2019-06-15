#pragma once

#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::ast {

enum class blaise_ast_type_type {
   PLAIN,
   ARRAY,
   POINTER,
   USER_DEFINED
};

enum class blaise_ast_system_type {
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   STRING,
   BOOLEAN
};

class blaise_ast_type {
   blaise_ast_type_type _type_type;
   protected:
      blaise_ast_type(blaise_ast_type_type type_type);
   public:
      ~blaise_ast_type();
      blaise_ast_type_type type_type() const;
      inline virtual bool equals(std::shared_ptr<blaise_ast_type> other) const = 0;
};

class blaise_ast_plain_type : public blaise_ast_type {
   blaise_ast_system_type _system_type;
   protected:
      blaise_ast_plain_type(blaise_ast_system_type system_type);
   public:
      ~blaise_ast_plain_type();
      blaise_ast_system_type system_type() const;
      inline virtual bool equals(std::shared_ptr<blaise_ast_type> other) const;

      friend std::shared_ptr<blaise_ast_type> make_plain_type(blaise_ast_system_type system);
};
std::shared_ptr<blaise_ast_type> make_plain_type(blaise_ast_system_type system);

std::shared_ptr<blaise_ast_type> get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token);

}