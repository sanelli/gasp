#pragma once

#include <memory>

#include <gasp/blaise/tokenizer/tokens.hpp>

#include <sanelli/sanelli.hpp>

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
      virtual ~blaise_ast_type();
      blaise_ast_type_type type_type() const;
      inline virtual bool equals(std::shared_ptr<blaise_ast_type> other) const = 0;
};

class blaise_ast_plain_type : public blaise_ast_type {
   blaise_ast_system_type _system_type;
   protected:
      blaise_ast_plain_type(blaise_ast_system_type system_type);
   public:
      ~blaise_ast_plain_type() override;
      blaise_ast_system_type system_type() const;
      inline bool equals(std::shared_ptr<blaise_ast_type> other) const override;

      friend sanelli::memory;
};

class blaise_ast_array_type : public blaise_ast_type {
   std::shared_ptr<blaise_ast_type> _inner_type;
   unsigned int _size;
   protected:
      blaise_ast_array_type(std::shared_ptr<blaise_ast_type> inner_type, unsigned int size);
   public:
      ~blaise_ast_array_type() override;
      std::shared_ptr<blaise_ast_type> inner_type() const;
      unsigned int size() const;
      bool is_unbounded() const;
      inline bool equals(std::shared_ptr<blaise_ast_type> other) const override;

      friend sanelli::memory;
};

std::shared_ptr<blaise_ast_type> get_type_from_token(const sanelli::token<gasp::blaise::blaise_token_type>& token);
std::shared_ptr<blaise_ast_type> make_plain_type(blaise_ast_system_type system);
std::shared_ptr<blaise_ast_type> get_array_type_from_token(
   const sanelli::token<gasp::blaise::blaise_token_type>& reference, 
   std::shared_ptr<blaise_ast_type> inner_type,
   const int array_size, const bool accept_unbounded_array);

}