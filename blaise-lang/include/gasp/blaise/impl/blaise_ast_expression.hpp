#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/impl/blaise_ast_common.hpp>
#include <gasp/blaise/impl/blaise_ast_type.hpp>
#include <gasp/blaise/impl/blaise_ast_expression_type.hpp>
#include <gasp/blaise/impl/blaise_ast_subroutine.hpp>

namespace gasp::blaise::ast
{

// Need this forward declaration otherwise the compile will complain
class blaise_ast_subroutine;

class blaise_ast_expression : public blaise_ast
{
   std::shared_ptr<blaise_ast_type> _result_type;
   blaise_ast_expression_type _expression_type;

public:
   blaise_ast_expression(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                         blaise_ast_expression_type expression_type,
                         std::shared_ptr<blaise_ast_type> result_type);
   std::shared_ptr<blaise_ast_type> result_type() const;
   blaise_ast_expression_type expression_type() const;
};

// NEW EXPRESSION
class blaise_ast_expression_new : public blaise_ast_expression
{
   std::vector<std::shared_ptr<blaise_ast_expression>> _expressions;
   blaise_ast_expression_new(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                             std::shared_ptr<blaise_ast_type> target_type,
                             const std::vector<std::shared_ptr<blaise_ast_expression>> &expressions);

public:
   unsigned int count_parameters() const;
   std::shared_ptr<blaise_ast_expression> get_parameter(unsigned int index) const;

   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_new> make_blaise_ast_expression_new(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                          std::shared_ptr<blaise_ast_type> target_type,
                                                                          const std::vector<std::shared_ptr<blaise_ast_expression>> &expressions);

//
// TERNARY EXPRESSION
//
class blaise_ast_expression_ternary : public blaise_ast_expression
{
   std::shared_ptr<blaise_ast_expression> _condition;
   std::shared_ptr<blaise_ast_expression> _then_expression;
   std::shared_ptr<blaise_ast_expression> _else_expression;
   blaise_ast_expression_ternary(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                 std::shared_ptr<blaise_ast_expression> condition,
                                 std::shared_ptr<blaise_ast_expression> then_expression,
                                 std::shared_ptr<blaise_ast_expression> else_expression);

public:
   std::shared_ptr<blaise_ast_expression> condition() const;
   std::shared_ptr<blaise_ast_expression> then_expression() const;
   std::shared_ptr<blaise_ast_expression> else_expression() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_ternary> make_blaise_ast_expression_ternary(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                  std::shared_ptr<blaise_ast_expression> condition,
                                                                                  std::shared_ptr<blaise_ast_expression> then_expression,
                                                                                  std::shared_ptr<blaise_ast_expression> else_expression);

//
// CAST EXPRESSION
//
class blaise_ast_expression_cast : public blaise_ast_expression
{
   std::shared_ptr<blaise_ast_expression> _operand;
   blaise_ast_expression_cast(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                              std::shared_ptr<blaise_ast_type> target_type,
                              std::shared_ptr<blaise_ast_expression> operand);

public:
   std::shared_ptr<blaise_ast_expression> operand() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_cast> make_blaise_ast_expression_cast(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                            std::shared_ptr<blaise_ast_type> target_type,
                                                                            std::shared_ptr<blaise_ast_expression> operand);
std::shared_ptr<blaise_ast_expression> introduce_cast_if_required(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                  std::shared_ptr<blaise_ast_type> target_type,
                                                                  std::shared_ptr<blaise_ast_expression> expression);
void introduce_cast_if_required(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                std::shared_ptr<blaise_ast_subroutine> subroutine,
                                std::vector<std::shared_ptr<blaise_ast_expression>> &expressions);
std::shared_ptr<ast::blaise_ast_expression> cast_to_boolean(
    const sanelli::token<gasp::blaise::blaise_token_type> &reference,
    std::shared_ptr<ast::blaise_ast_expression> expression);

//
// SUBROUTINE CALL EXPRESSION
//
class blaise_ast_expression_subroutine_call : public blaise_ast_expression
{
   std::shared_ptr<blaise_ast_subroutine> _subroutine;
   std::vector<std::shared_ptr<blaise_ast_expression>> _expressions;
   blaise_ast_expression_subroutine_call(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                         std::shared_ptr<blaise_ast_subroutine> subroutine,
                                         const std::vector<std::shared_ptr<blaise_ast_expression>> &expressions);

public:
   typename std::vector<std::shared_ptr<blaise_ast_expression>>::const_iterator begin_actual_parameters() const;
   typename std::vector<std::shared_ptr<blaise_ast_expression>>::const_iterator end_actual_parameters() const;
   std::shared_ptr<blaise_ast_subroutine> subroutine() const;
   unsigned int count_parameters() const;
   std::shared_ptr<blaise_ast_expression> get_parameter(unsigned int index) const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_subroutine_call> make_blaise_ast_expression_subroutine_call(
    sanelli::token<gasp::blaise::blaise_token_type> subroutine_name_token,
    std::shared_ptr<blaise_ast_subroutine> subroutine,
    const std::vector<std::shared_ptr<blaise_ast_expression>> &expressions);

//
// BINARY EXPRESSION
//
class blaise_ast_expression_binary : public blaise_ast_expression
{
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_ast_expression> _left;
   std::shared_ptr<blaise_ast_expression> _right;
   blaise_ast_expression_binary(
       const sanelli::token<gasp::blaise::blaise_token_type> &reference,
       std::shared_ptr<blaise_ast_expression> left,
       gasp::blaise::blaise_token_type op,
       std::shared_ptr<blaise_ast_expression> right);

public:
   std::shared_ptr<blaise_ast_expression> left() const;
   std::shared_ptr<blaise_ast_expression> right() const;
   gasp::blaise::blaise_token_type op() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_binary> make_blaise_ast_expression_binary(
    std::shared_ptr<blaise_ast_expression> left,
    sanelli::token<gasp::blaise::blaise_token_type> token_operator,
    std::shared_ptr<blaise_ast_expression> right);

//
// UNARY EXPRESSION
//
class blaise_ast_expression_unary : public blaise_ast_expression
{
   gasp::blaise::blaise_token_type _operator;
   std::shared_ptr<blaise_ast_expression> _operand;
   blaise_ast_expression_unary(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                               gasp::blaise::blaise_token_type op,
                               std::shared_ptr<blaise_ast_expression> operand);

public:
   std::shared_ptr<blaise_ast_expression> operand() const;
   gasp::blaise::blaise_token_type op() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_unary> make_blaise_ast_expression_unary(sanelli::token<gasp::blaise::blaise_token_type> token,
                                                                              std::shared_ptr<blaise_ast_expression> operand);

//
// EXPRESSION IDENTIFIER
//
enum class blaise_ast_expression_memory_access_type
{
   MEMORY_LOCATION,
   ARRAY
};
class blaise_ast_expression_generic_memory_access : public blaise_ast_expression
{
   blaise_ast_expression_memory_access_type _memory_access_type;

protected:
   blaise_ast_expression_generic_memory_access(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                               blaise_ast_expression_memory_access_type identifier_type,
                                               std::shared_ptr<blaise_ast_type> result_type);

public:
   blaise_ast_expression_memory_access_type memory_access_type() const;
};

class blaise_ast_expression_memory_access : public blaise_ast_expression_generic_memory_access
{
   std::shared_ptr<blaise_ast_generic_memory_location> _memory_location;
   blaise_ast_expression_memory_access(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                       std::shared_ptr<blaise_ast_generic_memory_location> memory_location);

public:
   std::shared_ptr<blaise_ast_generic_memory_location> memory_location() const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_memory_access> make_blaise_ast_expression_memory_access(
    const sanelli::token<gasp::blaise::blaise_token_type> &reference,
    std::shared_ptr<blaise_ast_generic_memory_location> memory_location);

class blaise_ast_expression_array_access : public blaise_ast_expression_generic_memory_access
{
   std::shared_ptr<blaise_ast_generic_memory_location> _array;
   std::vector<std::shared_ptr<blaise_ast_expression>> _indexing;
   blaise_ast_expression_array_access(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                      std::shared_ptr<blaise_ast_generic_memory_location> array,
                                      std::vector<std::shared_ptr<blaise_ast_expression>>& indexing);

public:
   std::shared_ptr<blaise_ast_generic_memory_location> array() const;
   unsigned int count_indexes() const;
   std::shared_ptr<blaise_ast_expression> indexing(unsigned int index) const;
   friend sanelli::memory;
};
std::shared_ptr<blaise_ast_expression_array_access> make_blaise_ast_expression_array_access(
    const sanelli::token<gasp::blaise::blaise_token_type> &reference,
    std::shared_ptr<blaise_ast_generic_memory_location> array,
    std::vector<std::shared_ptr<blaise_ast_expression>>& indexing);

//
// EXPRESSION LITERAL VALUES
//
template <blaise_ast_expression_type TExpressionType,
          blaise_ast_system_type TSystemType,
          typename TValue>
class blaise_ast_expression_value : public blaise_ast_expression
{
   TValue _value;

public:
   blaise_ast_expression_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, TValue value)
       : blaise_ast_expression(reference, TExpressionType, make_plain_type(TSystemType)), _value(value) {}

   TValue value() { return _value; }
};

class blaise_ast_expression_byte_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_BYTE, blaise_ast_system_type::BYTE, int8_t>
{
   friend sanelli::memory;
   blaise_ast_expression_byte_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, int8_t value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_short_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_SHORT, blaise_ast_system_type::SHORT, int16_t>
{
   friend sanelli::memory;
   blaise_ast_expression_short_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, int16_t value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_integer_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_INTEGER, blaise_ast_system_type::INTEGER, int32_t>
{
   friend sanelli::memory;
   blaise_ast_expression_integer_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, int32_t value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_long_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_LONG, blaise_ast_system_type::LONG, int64_t>
{
   friend sanelli::memory;
   blaise_ast_expression_long_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, int64_t value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_float_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_FLOAT, blaise_ast_system_type::FLOAT, float>
{
   friend sanelli::memory;
   blaise_ast_expression_float_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, float value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_double_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_DOUBLE, blaise_ast_system_type::DOUBLE, double>
{
   friend sanelli::memory;
   blaise_ast_expression_double_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, double value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_char_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_CHAR, blaise_ast_system_type::CHAR, unsigned char>
{
   friend sanelli::memory;
   blaise_ast_expression_char_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, unsigned char value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_string_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_STRING, blaise_ast_system_type::STRING, std::string>
{
   friend sanelli::memory;
   blaise_ast_expression_string_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, std::string value) : blaise_ast_expression_value(reference, value) {}
};
class blaise_ast_expression_boolean_value : public blaise_ast_expression_value<blaise_ast_expression_type::LITERAL_BOOLEAN, blaise_ast_system_type::BOOLEAN, bool>
{
   friend sanelli::memory;
   blaise_ast_expression_boolean_value(const sanelli::token<gasp::blaise::blaise_token_type> &reference, bool value) : blaise_ast_expression_value(reference, value) {}
};

} // namespace gasp::blaise::ast