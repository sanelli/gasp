#include <string>
#include <iostream>
#include <regex>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

auto sample_empty = R"__(program sample;
begin
end.)__";

auto sample_empty_with_parameters = R"__(program sample(first: integer, second: boolean);
begin
end.)__";

std::string generate_empty_with_return_type_sample(const char *return_type) { 
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
begin
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);

   return sample;
}

std::string generate_binary_operator_sample(const char *operator_type, const char *return_type, const char *binary_operator)
{
   std::regex operator_type_regexp("\\{OPERATOR_TYPE\\}");
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex binary_operator_regexp("\\{OPERATOR\\}");

   std::string sample(R"__(program sample(first: {OPERATOR_TYPE}, second: {OPERATOR_TYPE}): {RETURN_TYPE};
begin
   sample := first {OPERATOR} second;
end.)__");

   sample = std::regex_replace(sample, operator_type_regexp, operator_type);
   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, binary_operator_regexp, binary_operator);

   return sample;
}

std::string generate_unary_operator_sample(const char *operator_type, const char *return_type, const char *binary_operator)
{
   std::regex operator_type_regexp("\\{OPERATOR_TYPE\\}");
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex binary_operator_regexp("\\{OPERATOR\\}");

   std::string sample(R"__(program sample(first: {OPERATOR_TYPE}): {RETURN_TYPE};
begin
   sample := {OPERATOR} first;
end.)__");

   sample = std::regex_replace(sample, operator_type_regexp, operator_type);
   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, binary_operator_regexp, binary_operator);

   return sample;
}

std::string generate_literal_assignment_sample(const char *operator_type, const char *return_type, const char *literal)
{
   std::regex operator_type_regexp("\\{OPERATOR_TYPE\\}");
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex literal_regexp("\\{LITERAL\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
begin
   sample := {LITERAL};
end.)__");

   sample = std::regex_replace(sample, operator_type_regexp, operator_type);
   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, literal_regexp, literal);

   return sample;
}

std::string generate_cast_sample(const char *from_type, const char *target_type)
{
   std::regex from_type_regexp("\\{FROM_TYPE\\}");
   std::regex target_type_regexp("\\{TARGET_TYPE\\}");

   std::string sample(R"__(program sample(first: {FROM_TYPE}): {TARGET_TYPE};
begin
   sample := cast<{TARGET_TYPE}>(first);
end.)__");

   sample = std::regex_replace(sample, from_type_regexp, from_type);
   sample = std::regex_replace(sample, target_type_regexp, target_type);

   return sample;
}

auto sample_ternary_expression = R"__(program sample(first: boolean);
begin
   sample := if first then 1 else 2;
end.)__";

auto sample_statement_if_then = R"__(program sample(first: boolean): integer;
begin
   sample := 1;
   if (first) then 
      sample := 2;
end.)__";

auto sample_statement_if_then_else = R"__(program sample(first: boolean): integer;
begin
   sample := 1;
   if first then 
      sample := 2;
   else 
      sample := 3;
end.)__";

auto sample_statement_for = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   for index from 1 to first begin
      sample := sample + index;
   end
end.)__";

auto sample_statement_for_step = R"__(program sample(first: integer, second: integer): integer;
var
   index: integer;
begin
   sample := 0;
   for index from 1 to first step second begin
      sample := sample + index;
   end
end.)__";

auto sample_statement_empty = R"__(program sample;
var
begin
 ; ; ; ; ;
end.)__";

auto sample_statement_while = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   while index <= first begin
      sample := sample + index;
      index := index + 1;
   end;
end.)__";

auto sample_statement_do_while = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   do begin
      sample := sample + index;
      index := index + 1;
   end while index <= first;
end.)__";

auto sample_statement_repeat_until = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   repeat begin
      sample := sample + index;
      index := index + 1;
   end until index > first;
end.)__";


std::string generate_function_expression_sample(const char *type)
{
   std::regex type_regexp("\\{TYPE\\}");

   std::string sample(R"__(program sample(first: {TYPE}): {TYPE};

function duplicate(p: {TYPE}) : {TYPE}
begin
   duplicate := 2 * p;
end;

begin
   sample := duplicate(first);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);

   return sample;
}

std::string generate_function_expression_2_sample(const char *type, const char* op)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex op_regexp("\\{OP\\}");

   std::string sample(R"__(program sample(first: {TYPE}, second: {TYPE}): {TYPE};

function execute(p1: {TYPE}, p2: {TYPE}) : {TYPE}
begin
   execute := p1 {OP} p2;
end;

begin
   sample := execute(first, second);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, op_regexp, op);

   return sample;
}

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = {sample_empty, "", "0"};
   _samples["empty-return-integer"] = {generate_empty_with_return_type_sample("integer"), "", "0"};
   _samples["empty-return-float"] = {generate_empty_with_return_type_sample("float"), "", "0.000000"};
   _samples["empty-return-boolean"] = {generate_empty_with_return_type_sample("boolean"), "", "false"};
   _samples["empty-return-double"] = {generate_empty_with_return_type_sample("double"), "", "0.000000"};
   _samples["empty-return-char"] = {generate_empty_with_return_type_sample("char"), "", "\\0"};
   _samples["empty-parameters-1"] = {sample_empty_with_parameters, "7 true", "0"};

   _samples["expression-math-integer-sum"] = {generate_binary_operator_sample("integer", "integer", "+"), "3 4", "7"};
   _samples["expression-math-integer-subtract"] = {generate_binary_operator_sample("integer", "integer", "-"), "3 4", "-1"};
   _samples["expression-math-integer-multiply"] = {generate_binary_operator_sample("integer", "integer", "*"), "3 4", "12"};
   _samples["expression-math-integer-divide"] = {generate_binary_operator_sample("integer", "integer", "/"), "15 5", "3"};
   _samples["expression-math-integer-remainder"] = {generate_binary_operator_sample("integer", "integer", "%"), "6 4", "2"};
   _samples["expression-math-integer-negate-1"] = {generate_unary_operator_sample("integer", "integer", "-"), "4", "-4"};
   _samples["expression-math-integer-negate-2"] = {generate_unary_operator_sample("integer", "integer", "-"), "-4", "4"};
   _samples["expression-math-integer-negate-3"] = {generate_unary_operator_sample("integer", "integer", "-"), "0", "0"};
   _samples["expression-math-float-sum"] = {generate_binary_operator_sample("float", "float", "+"), "3.5 4.5", "8.000000"};
   _samples["expression-math-float-subtract"] = {generate_binary_operator_sample("float", "float", "-"), "3.5 4.5", "-1.000000"};
   _samples["expression-math-float-multiply"] = {generate_binary_operator_sample("float", "float", "*"), "3.5 4.5", "15.750000"};
   _samples["expression-math-float-divide"] = {generate_binary_operator_sample("float", "float", "/"), "5.0 2.0", "2.500000"};
   _samples["expression-math-float-negate-1"] = {generate_unary_operator_sample("float", "float", "-"), "4.000000", "-4.000000"};
   _samples["expression-math-float-negate-2"] = {generate_unary_operator_sample("float", "float", "-"), "-4.000000", "4.000000"};
   _samples["expression-math-float-negate-3"] = {generate_unary_operator_sample("float", "float", "-"), "0.000000", "-0.000000"};
   _samples["expression-math-double-sum"] = {generate_binary_operator_sample("double", "double", "+"), "3.5 4.5", "8.000000"};
   _samples["expression-math-double-subtract"] = {generate_binary_operator_sample("double", "double", "-"), "3.5 4.5", "-1.000000"};
   _samples["expression-math-double-multiply"] = {generate_binary_operator_sample("double", "double", "*"), "3.5 4.5", "15.750000"};
   _samples["expression-math-double-divide"] = {generate_binary_operator_sample("double", "double", "/"), "5.0 2.0", "2.500000"};
   _samples["expression-math-double-negate-1"] = {generate_unary_operator_sample("double", "double", "-"), "4.000000", "-4.000000"};
   _samples["expression-math-double-negate-2"] = {generate_unary_operator_sample("double", "double", "-"), "-4.000000", "4.000000"};
   _samples["expression-math-double-negate-3"] = {generate_unary_operator_sample("double", "double", "-"), "0.000000", "-0.000000"};

   _samples["expression-boolean-and-1"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "true true", "true"};
   _samples["expression-boolean-and-2"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "true false", "false"};
   _samples["expression-boolean-and-3"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "false true", "false"};
   _samples["expression-boolean-and-4"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "false false", "false"};
   _samples["expression-boolean-or-1"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "true true", "true"};
   _samples["expression-boolean-or-2"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "true false", "true"};
   _samples["expression-boolean-or-3"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "false true", "true"};
   _samples["expression-boolean-or-4"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "false false", "false"};
   _samples["expression-boolean-not-1"] = {generate_unary_operator_sample("boolean", "boolean", "not"), "false", "true"};
   _samples["expression-boolean-not-2"] = {generate_unary_operator_sample("boolean", "boolean", "not"), "true", "false"};

   _samples["expression-compare-integer-eq-1"] = {generate_binary_operator_sample("integer", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-integer-eq-2"] = {generate_binary_operator_sample("integer", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-integer-eq-3"] = {generate_binary_operator_sample("integer", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-integer-neq-1"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-integer-neq-2"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-integer-neq-3"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-integer-gt-1"] = {generate_binary_operator_sample("integer", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-integer-gt-2"] = {generate_binary_operator_sample("integer", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-integer-gt-3"] = {generate_binary_operator_sample("integer", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-integer-lt-1"] = {generate_binary_operator_sample("integer", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-integer-lt-2"] = {generate_binary_operator_sample("integer", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-integer-lt-3"] = {generate_binary_operator_sample("integer", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-integer-lte-1"] = {generate_binary_operator_sample("integer", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-integer-lte-2"] = {generate_binary_operator_sample("integer", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-integer-lte-3"] = {generate_binary_operator_sample("integer", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-integer-gte-1"] = {generate_binary_operator_sample("integer", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-integer-gte-2"] = {generate_binary_operator_sample("integer", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-integer-gte-3"] = {generate_binary_operator_sample("integer", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-float-eq-1"] = {generate_binary_operator_sample("float", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-float-eq-2"] = {generate_binary_operator_sample("float", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-float-eq-3"] = {generate_binary_operator_sample("float", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-float-neq-1"] = {generate_binary_operator_sample("float", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-float-neq-2"] = {generate_binary_operator_sample("float", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-float-neq-3"] = {generate_binary_operator_sample("float", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-float-gt-1"] = {generate_binary_operator_sample("float", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-float-gt-2"] = {generate_binary_operator_sample("float", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-float-gt-3"] = {generate_binary_operator_sample("float", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-float-lt-1"] = {generate_binary_operator_sample("float", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-float-lt-2"] = {generate_binary_operator_sample("float", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-float-lt-3"] = {generate_binary_operator_sample("float", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-float-lte-1"] = {generate_binary_operator_sample("float", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-float-lte-2"] = {generate_binary_operator_sample("float", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-float-lte-3"] = {generate_binary_operator_sample("float", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-float-gte-1"] = {generate_binary_operator_sample("float", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-float-gte-2"] = {generate_binary_operator_sample("float", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-float-gte-3"] = {generate_binary_operator_sample("float", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-double-eq-1"] = {generate_binary_operator_sample("double", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-double-eq-2"] = {generate_binary_operator_sample("double", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-double-eq-3"] = {generate_binary_operator_sample("double", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-double-neq-1"] = {generate_binary_operator_sample("double", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-double-neq-2"] = {generate_binary_operator_sample("double", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-double-neq-3"] = {generate_binary_operator_sample("double", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-double-gt-1"] = {generate_binary_operator_sample("double", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-double-gt-2"] = {generate_binary_operator_sample("double", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-double-gt-3"] = {generate_binary_operator_sample("double", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-double-lt-1"] = {generate_binary_operator_sample("double", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-double-lt-2"] = {generate_binary_operator_sample("double", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-double-lt-3"] = {generate_binary_operator_sample("double", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-double-lte-1"] = {generate_binary_operator_sample("double", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-double-lte-2"] = {generate_binary_operator_sample("double", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-double-lte-3"] = {generate_binary_operator_sample("double", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-double-gte-1"] = {generate_binary_operator_sample("double", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-double-gte-2"] = {generate_binary_operator_sample("double", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-double-gte-3"] = {generate_binary_operator_sample("double", "boolean", ">="), "5 4", "true"};

   _samples["expression-ternary-1"] = {sample_ternary_expression, "true", "1"};
   _samples["expression-ternary-2"] = {sample_ternary_expression, "false", "2"};

   _samples["expression-cast-integer-to-float"] = {generate_cast_sample("integer", "float"), "1", "1.000000"};
   _samples["expression-cast-integer-to-double"] = {generate_cast_sample("integer", "double"), "1", "1.000000"};
   _samples["expression-cast-integer-to-boolean-1"] = {generate_cast_sample("integer", "boolean"), "1", "true"};
   _samples["expression-cast-integer-to-boolean-2"] = {generate_cast_sample("integer", "boolean"), "0", "false"};
   _samples["expression-cast-integer-to-char"] = {generate_cast_sample("integer", "char"), "65", "A"};
   _samples["expression-cast-char-to-integer"] = {generate_cast_sample("char", "integer"), "A", "65"};
   _samples["expression-cast-float-to-integer"] = {generate_cast_sample("float", "integer"), "1.00", "1"};
   _samples["expression-cast-float-to-double"] = {generate_cast_sample("float", "double"), "1.00", "1.000000"};
   _samples["expression-cast-double-to-integer"] = {generate_cast_sample("double", "integer"), "1.00", "1"};
   _samples["expression-cast-double-to-float"] = {generate_cast_sample("double", "float"), "1.00", "1.000000"};
   _samples["expression-cast-boolean-to-integer-1"] = {generate_cast_sample("boolean", "integer"), "true", "1"};
   _samples["expression-cast-boolean-to-integer-2"] = {generate_cast_sample("boolean", "integer"), "false", "0"};

   _samples["expression-function-call-duplicate-integer"] = {generate_function_expression_sample("integer"), "1", "2"};
   _samples["expression-function-call-duplicate-double"] = {generate_function_expression_sample("double"), "1", "2.000000"};
   _samples["expression-function-call-duplicate-float"] = {generate_function_expression_sample("float"), "1", "2.000000"};
   _samples["expression-function-call-sum-integer"] = {generate_function_expression_2_sample("integer", "+"), "1 2", "3"};
   _samples["expression-function-call-subtract-integer"] = {generate_function_expression_2_sample("integer", "-"), "1 2", "-1"};
   _samples["expression-function-call-multuply-integer"] = {generate_function_expression_2_sample("integer", "*"), "1 2", "2"};
   _samples["expression-function-call-divide-integer"] = {generate_function_expression_2_sample("integer", "/"), "1 2", "0"};
   _samples["expression-function-call-remainder-integer"] = {generate_function_expression_2_sample("integer", "%"), "1 2", "1"};

   _samples["literal-integer-binary"] = {generate_literal_assignment_sample("integer", "integer", "0b11"), "", "3"};
   _samples["literal-integer-octal"] = {generate_literal_assignment_sample("integer", "integer", "0o77"), "", "63"};
   _samples["literal-integer-hexadecimal"] = {generate_literal_assignment_sample("integer", "integer", "0xFF"), "", "255"};
   _samples["literal-integer-decimal"] = {generate_literal_assignment_sample("integer", "integer", "10"), "", "10"};
   _samples["literal-char"] = {generate_literal_assignment_sample("char", "char", "'A'"), "", "A"};
   _samples["literal-boolean-true"] = {generate_literal_assignment_sample("boolean", "boolean", "true"), "", "true"};
   _samples["literal-boolean-false"] = {generate_literal_assignment_sample("boolean", "boolean", "false"), "", "false"};
   _samples["literal-double"] = {generate_literal_assignment_sample("double", "double", "1.5"), "", "1.500000"};

   _samples["statement-empty"] = {sample_statement_empty, "", "0"};
   _samples["statement-if-then-true"] = {sample_statement_if_then, "true", "2"};
   _samples["statement-if-then-false"] = {sample_statement_if_then, "false", "1"};
   _samples["statement-if-then-else-true"] = {sample_statement_if_then_else, "true", "2"};
   _samples["statement-if-then-else-false"] = {sample_statement_if_then_else, "false", "3"};
   _samples["statement-for-1"] = {sample_statement_for, "3", "6"};
   _samples["statement-for-2"] = {sample_statement_for, "100", "5050"};
   _samples["statement-for-step-1"] = {sample_statement_for_step, "3 1", "6"};
   _samples["statement-for-step-2"] = {sample_statement_for_step, "3 2", "4"};
   _samples["statement-while-1"] = {sample_statement_while, "3", "6"};
   _samples["statement-while-2"] = {sample_statement_while, "100", "5050"};
   _samples["statement-while-3"] = {sample_statement_while, "0", "0"};
   _samples["statement-do-while-1"] = {sample_statement_do_while, "3", "6"};
   _samples["statement-do-while-2"] = {sample_statement_do_while, "100", "5050"};
   _samples["statement-do-while-3"] = {sample_statement_do_while, "0", "1"};
   _samples["statement-repeat-until-1"] = {sample_statement_repeat_until, "3", "6"};
   _samples["statement-repeat-until-2"] = {sample_statement_repeat_until, "100", "5050"};
   _samples["statement-repeat-until-3"] = {sample_statement_repeat_until, "0", "1"};

}

std::string gasp_module_blaise_sample::name() const
{
   return "blaise-sample";
}

std::string gasp_module_blaise_sample::description() const
{
   return "Output the desired blaise sample.";
}

const std::string p_help("--help");
const std::string p_help_short("-h");
const std::string p_list("--list");
const std::string p_list_short("-l");
const std::string p_sample_input("--sample-input");
const std::string p_sample_input_short("-si");
const std::string p_sample_output("--sample-output");
const std::string p_sample_output_short("-so");

void gasp_module_blaise_sample::get_sample_names(std::vector<std::string> &samples) const
{
   for (auto it = _samples.begin(); it != _samples.end(); ++it)
      samples.push_back(it->first);
}

std::string gasp_module_blaise_sample::get_sample(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.sample;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}

std::string gasp_module_blaise_sample::get_input(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.input;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}
std::string gasp_module_blaise_sample::get_output(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.output;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}

bool gasp_module_blaise_sample::run(int argc, char *argv[])
{
   if (argc < 3)
   {
      std::cerr << "Too few parameters passed. Use the option '" << p_help << "' for more details." << std::endl;
      return false;
   }

   if (argc > 4)
   {
      std::cerr << "Too many parameters passed. Use the option '" << p_help << "' for more details." << std::endl;
      return false;
   }

   std::string arg(argv[2]);
   if (arg == p_help || arg == p_help_short)
   {
      std::cout << "Command line: " << argv[0] << " " << name() << " [options]|<sample_name>" << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << "   " << p_help << "/" << p_help_short << ": display this help." << std::endl;
      std::cout << "   " << p_list << "/" << p_list_short << ": list all the available samples." << std::endl;
      std::cout << "   --sample-parameters/-sp <sample_name>: print out sample parameters." << std::endl;
      std::cout << "   --sample-result/-sr <sample_name>: print out sample result given the simple parameters form -sp." << std::endl;
      return true;
   }

   if (arg == p_list || arg == p_list_short)
   {
      for (auto it = _samples.begin(); it != _samples.end(); ++it)
         std::cout << it->first << std::endl;
      return true;
   }

   if (arg == p_sample_input || arg == p_sample_input_short)
   {
      if (argc < 4)
      {
         std::cout << "Missing sample format name" << std::endl;
         return false;
      }
      arg = argv[3];
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.input << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }

   if (arg == p_sample_output || arg == p_sample_output_short)
   {
      if (argc < 4)
      {
         std::cout << "Missing sample format name" << std::endl;
         return false;
      }
      arg = argv[3];
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.output << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }

   {
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.sample << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }
}

gasp_module_blaise_sample::~gasp_module_blaise_sample()
{
   _samples.clear();
}
