#include <string>
#include <map>
#include <iostream>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

struct sample_type { 
   std::string sample;
   std::string input;
   std::string output;
};

std::map<std::string, sample_type> _samples;

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

auto sample_sum_two_integer_parameters = R"__(program sample(first: integer, second: integer);
begin
   sample := first + second;
end.)__";

auto sample_subtract_two_integer_parameters = R"__(program sample(first: integer, second: integer);
begin
   sample := first - second;
end.)__";

auto sample_multiply_two_integer_parameters = R"__(program sample(first: integer, second: integer);
begin
   sample := first * second;
end.)__";

auto sample_divide_two_integer_parameters = R"__(program sample(first: integer, second: integer);
begin
   sample := first / second;
end.)__";

auto sample_remainder_two_integer_parameters = R"__(program sample(first: integer, second: integer);
begin
   sample := first % second;
end.)__";

auto sample_eq_two_integer_parameters = R"__(program sample(first: integer, second: integer): boolean;
begin
   sample := first == second;
end.)__";

auto sample_neq_two_integer_parameters = R"__(program sample(first: integer, second: integer): boolean;
begin
   sample := first <> second;
end.)__";

auto sample_gt_two_integer_parameters = R"__(program sample(first: integer, second: integer): boolean;
begin
   sample := first > second;
end.)__";

auto sample_lt_two_integer_parameters = R"__(program sample(first: integer, second: integer) : boolean;
begin
   sample := first < second;
end.)__";

auto sample_gte_two_integer_parameters = R"__(program sample(first: integer, second: integer): boolean;
begin
   sample := first >= second;
end.)__";

auto sample_lte_two_integer_parameters = R"__(program sample(first: integer, second: integer) : boolean;
begin
   sample := first <= second;
end.)__";

auto sample_sum_two_float_parameters = R"__(program sample(first: float, second: float) : float;
begin
   sample := first + second;
end.)__";

auto sample_subtract_two_float_parameters = R"__(program sample(first: float, second: float) : float;
begin
   sample := first - second;
end.)__";

auto sample_multiply_two_float_parameters = R"__(program sample(first: float, second: float) : float;
begin
   sample := first * second;
end.)__";

auto sample_divide_two_float_parameters = R"__(program sample(first: float, second: float) : float;
begin
   sample := first / second;
end.)__";

auto sample_sum_two_double_parameters = R"__(program sample(first: double, second: double) : double;
begin
   sample := first + second;
end.)__";

auto sample_subtract_two_double_parameters = R"__(program sample(first: double, second: double) : double;
begin
   sample := first - second;
end.)__";

auto sample_multiply_two_double_parameters = R"__(program sample(first: double, second: double) : double;
begin
   sample := first * second;
end.)__";

auto sample_divide_two_double_parameters = R"__(program sample(first: double, second: double) : double;
begin
   sample := first / second;
end.)__";

auto sample_ternary_expression = R"__(program sample(first: boolean);
begin
   sample := if first then 1 else 2;
end.)__";

auto sample_and_two_boolean_parameters = R"__(program sample(first: boolean, second: boolean): boolean;
begin
   sample := first and second;
end.)__";

auto sample_or_two_boolean_parameters = R"__(program sample(first: boolean, second: boolean): boolean;
begin
   sample := first or second;
end.)__";

auto sample_not_boolean_parameter = R"__(program sample(first: boolean): boolean;
begin
   sample := not first;
end.)__";

auto sample_binary_integer = R"__(program sample;
begin
   sample := 0b11;
end.)__";

auto sample_octal_integer = R"__(program sample;
begin
   sample := 0o77;
end.)__";

auto sample_hexadecimal_integer = R"__(program sample;
begin
   sample := 0xFF;
end.)__";

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = {sample_empty, "", "0"} ;
   _samples["empty-return-integer"] = { sample_empty_returning_integer, "", "0" };
   _samples["empty-return-float"] = { sample_empty_returning_float, "",  "0.000000" };
   _samples["empty-return-boolean"] = { sample_empty_returning_boolean, "", "false" };
   _samples["empty-parameters-1"] = { sample_empty_with_parameters, "7 true", "0" };
   
   _samples["expression-math-integer-sum"] = { sample_sum_two_integer_parameters, "3 4", "7"};
   _samples["expression-math-integer-subtract"] = { sample_subtract_two_integer_parameters, "3 4", "-1" };
   _samples["expression-math-integer-multiply"] = { sample_multiply_two_integer_parameters, "3 4", "12" };
   _samples["expression-math-integer-divide"] = { sample_divide_two_integer_parameters, "15 5", "3" };
   _samples["expression-math-integer-remainder"] = { sample_remainder_two_integer_parameters, "6 4", "2" };
   _samples["expression-math-float-sum"] = { sample_sum_two_float_parameters, "3.5 4.5", "8.000000" };
   _samples["expression-math-float-subtract"] = { sample_subtract_two_float_parameters, "3.5 4.5", "-1.000000" };
   _samples["expression-math-float-multiply"] = { sample_multiply_two_float_parameters, "3.5 4.5", "15.750000" };
   _samples["expression-math-float-divide"] = { sample_divide_two_float_parameters, "5.0 2.0", "2.500000" };
   _samples["expression-math-double-sum"] = { sample_sum_two_double_parameters, "3.5 4.5", "8.000000" };
   _samples["expression-math-double-subtract"] = { sample_subtract_two_double_parameters, "3.5 4.5", "-1.000000" };
   _samples["expression-math-double-multiply"] = { sample_multiply_two_double_parameters, "3.5 4.5", "15.750000" };
   _samples["expression-math-double-divide"] = { sample_divide_two_double_parameters, "5.0 2.0", "2.500000" };

   _samples["expression-boolean-and-1"] = { sample_and_two_boolean_parameters, "true true", "true" };
   _samples["expression-boolean-and-2"] = { sample_and_two_boolean_parameters, "true false", "false" };
   _samples["expression-boolean-and-3"] = { sample_and_two_boolean_parameters, "false true", "false" };
   _samples["expression-boolean-and-4"] = { sample_and_two_boolean_parameters, "false false", "false" };
   _samples["expression-boolean-or-1"] = { sample_or_two_boolean_parameters, "true true", "true" };
   _samples["expression-boolean-or-2"] = { sample_or_two_boolean_parameters, "true false", "true" };
   _samples["expression-boolean-or-3"] = { sample_or_two_boolean_parameters, "false true", "true" };
   _samples["expression-boolean-or-4"] = { sample_or_two_boolean_parameters, "false false", "false" };
   _samples["expression-boolean-not-1"] = { sample_not_boolean_parameter, "false", "true" };
   _samples["expression-boolean-not-2"] = { sample_not_boolean_parameter,  "true", "false" };

   _samples["expression-compare-integer-eq-1"] = { sample_eq_two_integer_parameters, "5 5", "true" };
   _samples["expression-compare-integer-eq-2"] = { sample_eq_two_integer_parameters, "4 5", "false" };
   _samples["expression-compare-integer-eq-3"] = { sample_eq_two_integer_parameters, "5 4", "false" };
   _samples["expression-compare-integer-neq-1"] = { sample_neq_two_integer_parameters, "4 5", "true" };
   _samples["expression-compare-integer-neq-2"] = { sample_neq_two_integer_parameters, "5 5", "false" };
   _samples["expression-compare-integer-neq-3"] = { sample_neq_two_integer_parameters, "5 4", "true" };
   _samples["expression-compare-integer-gt-1"] = { sample_gt_two_integer_parameters, "6 5", "true" };
   _samples["expression-compare-integer-gt-2"] = { sample_gt_two_integer_parameters, "4 5", "false" };
   _samples["expression-compare-integer-gt-3"] = { sample_gt_two_integer_parameters, "4 4", "false" };
   _samples["expression-compare-integer-lt-1"] = { sample_lt_two_integer_parameters, "4 5", "true" };
   _samples["expression-compare-integer-lt-2"] = { sample_lt_two_integer_parameters, "6 5", "false" };
   _samples["expression-compare-integer-lt-3"] = { sample_lt_two_integer_parameters, "5 5", "false" };
   _samples["expression-compare-integer-lte-1"] = { sample_lte_two_integer_parameters, "4 5", "true" };
   _samples["expression-compare-integer-lte-2"] = { sample_lte_two_integer_parameters, "4 4", "true" };
   _samples["expression-compare-integer-lte-3"] =  {sample_lte_two_integer_parameters, "5 4", "false" };
   _samples["expression-compare-integer-gte-1"] = { sample_gte_two_integer_parameters, "4 5", "false" };
   _samples["expression-compare-integer-gte-2"] = { sample_gte_two_integer_parameters, "4 4", "true" };
   _samples["expression-compare-integer-gte-3"] = { sample_gte_two_integer_parameters, "5 4", "true" };
 
   _samples["expression-ternary-1"] = { sample_ternary_expression, "true", "1" };
   _samples["expression-ternary-2"] = { sample_ternary_expression, "false", "2" };

   _samples["literal-integer-binary"] = { sample_binary_integer, "", "3" };
   _samples["literal-integer-octal"] = { sample_octal_integer, "", "63" };
   _samples["literal-integer-hexadecimal"] = { sample_hexadecimal_integer, "", "255" };
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
