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

auto sample_empty_returning_integer = R"__(program sample : integer;
begin
end.)__";

auto sample_empty_returning_float = R"__(program sample : float;
begin
end.)__";

auto sample_empty_returning_boolean = R"__(program sample : boolean;
begin
end.)__";

auto sample_empty_with_parameters = R"__(program sample(first: integer, second: boolean);
begin
end.)__";

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

std::string generate_assignment_operator_sample(const char *operator_type, const char *return_type, const char *literal)
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

auto sample_ternary_expression = R"__(program sample(first: boolean);
begin
   sample := if first then 1 else 2;
end.)__";

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = {sample_empty, "", "0"};
   _samples["empty-return-integer"] = {sample_empty_returning_integer, "", "0"};
   _samples["empty-return-float"] = {sample_empty_returning_float, "", "0.000000"};
   _samples["empty-return-boolean"] = {sample_empty_returning_boolean, "", "false"};
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
   _samples["expression-math-float-negate-3"] = {generate_unary_operator_sample("float", "float", "-"), "0.000000", "0.000000"};
   _samples["expression-math-double-sum"] = {generate_binary_operator_sample("double", "double", "+"), "3.5 4.5", "8.000000"};
   _samples["expression-math-double-subtract"] = {generate_binary_operator_sample("double", "double", "-"), "3.5 4.5", "-1.000000"};
   _samples["expression-math-double-multiply"] = {generate_binary_operator_sample("double", "double", "*"), "3.5 4.5", "15.750000"};
   _samples["expression-math-double-divide"] = {generate_binary_operator_sample("double", "double", "/"), "5.0 2.0", "2.500000"};
   _samples["expression-math-double-negate-1"] = {generate_unary_operator_sample("double", "double", "-"), "4.000000", "-4.000000"};
   _samples["expression-math-double-negate-2"] = {generate_unary_operator_sample("double", "double", "-"), "-4.000000", "4.000000"};
   _samples["expression-math-double-negate-3"] = {generate_unary_operator_sample("double", "double", "-"), "0.000000", "0.000000"};

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

   _samples["expression-ternary-1"] = {sample_ternary_expression, "true", "1"};
   _samples["expression-ternary-2"] = {sample_ternary_expression, "false", "2"};

   _samples["literal-integer-binary"] = {generate_assignment_operator_sample("integer", "integer", "0b11"), "", "3"};
   _samples["literal-integer-octal"] = {generate_assignment_operator_sample("integer", "integer", "0o77"), "", "63"};
   _samples["literal-integer-hexadecimal"] = {generate_assignment_operator_sample("integer", "integer", "0xFF"), "", "255"};
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
const std::string p_sample_parameter("--sample-parameter");
const std::string p_sample_parameter_short("-sp");
const std::string p_sample_return("--sample-return");
const std::string p_sample_return_short("-sr");

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

   if (arg == p_sample_parameter || arg == p_sample_parameter_short)
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

   if (arg == p_sample_return || arg == p_sample_return_short)
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
