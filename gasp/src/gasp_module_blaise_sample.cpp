#include <string>
#include <map>
#include <iostream>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

std::map<std::string, std::string> _samples;
std::map<std::string, std::string> _samples_parameters;
std::map<std::string, std::string> _samples_return_value;

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

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = sample_empty;
   _samples_parameters["empty"] = "";
   _samples_return_value["empty"] = "0";

   _samples["empty-returning-integer"] = sample_empty_returning_integer;
   _samples_parameters["empty-returning-integer"] = "";
   _samples_return_value["empty-returning-integer"] = "0";

   _samples["empty-returning-float"] = sample_empty_returning_float;
   _samples_parameters["empty-returning-float"] = "";
   _samples_return_value["empty-returning-float"] = "0.000000";

   _samples["empty-returning-boolean"] = sample_empty_returning_boolean;
   _samples_parameters["empty-returning-boolean"] = "";
   _samples_return_value["empty-returning-boolean"] = "false";

   _samples["empty-with-parameters"] = sample_empty_with_parameters;
   _samples_parameters["empty-with-parameters"] = "7 true";
   _samples_return_value["empty-with-parameters"] = "0";

   _samples["sum-two-integer-parameters"] = sample_sum_two_integer_parameters;
   _samples_parameters["sum-two-integer-parameters"] = "3 4";
   _samples_return_value["sum-two-integer-parameters"] = "7";

   _samples["subtract-two-integer-parameters"] = sample_subtract_two_integer_parameters;
   _samples_parameters["subtract-two-integer-parameters"] = "3 4";
   _samples_return_value["subtract-two-integer-parameters"] = "-1";

   _samples["multiply-two-integer-parameters"] = sample_multiply_two_integer_parameters;
   _samples_parameters["multiply-two-integer-parameters"] = "3 4";
   _samples_return_value["multiply-two-integer-parameters"] = "12";

   _samples["divide-two-integer-parameters"] = sample_divide_two_integer_parameters;
   _samples_parameters["divide-two-integer-parameters"] = "15 5";
   _samples_return_value["divide-two-integer-parameters"] = "3";

   _samples["compare-two-integer-parameters-eq-1"] = sample_eq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-eq-1"] = "5 5";
   _samples_return_value["compare-two-integer-parameters-eq-1"] = "true";

   _samples["compare-two-integer-parameters-eq-2"] = sample_eq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-eq-2"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-eq-2"] = "false";

   _samples["compare-two-integer-parameters-eq-3"] = sample_eq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-eq-3"] = "5 4";
   _samples_return_value["compare-two-integer-parameters-eq-3"] = "false";

   _samples["compare-two-integer-parameters-neq-1"] = sample_neq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-neq-1"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-neq-1"] = "true";

   _samples["compare-two-integer-parameters-neq-2"] = sample_neq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-neq-2"] = "5 5";
   _samples_return_value["compare-two-integer-parameters-neq-2"] = "false";

   _samples["compare-two-integer-parameters-neq-3"] = sample_neq_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-neq-3"] = "5 4";
   _samples_return_value["compare-two-integer-parameters-neq-3"] = "true";

   _samples["compare-two-integer-parameters-gt-1"] = sample_gt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gt-1"] = "6 5";
   _samples_return_value["compare-two-integer-parameters-gt-1"] = "true";

   _samples["compare-two-integer-parameters-gt-2"] = sample_gt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gt-2"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-gt-2"] = "false";
   
   _samples["compare-two-integer-parameters-gt-3"] = sample_gt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gt-3"] = "4 4";
   _samples_return_value["compare-two-integer-parameters-gt-3"] = "false";

   _samples["compare-two-integer-parameters-lt-1"] = sample_lt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lt-1"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-lt-1"] = "true";

   _samples["compare-two-integer-parameters-lt-2"] = sample_lt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lt-2"] = "6 5";
   _samples_return_value["compare-two-integer-parameters-lt-2"] = "false";

   _samples["compare-two-integer-parameters-lt-3"] = sample_lt_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lt-3"] = "5 5";
   _samples_return_value["compare-two-integer-parameters-lt-3"] = "false";

   _samples["compare-two-integer-parameters-lte-1"] = sample_lte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lte-1"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-lte-1"] = "true";

   _samples["compare-two-integer-parameters-lte-2"] = sample_lte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lte-2"] = "4 4";
   _samples_return_value["compare-two-integer-parameters-lte-2"] = "true";

   _samples["compare-two-integer-parameters-lte-3"] = sample_lte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-lte-3"] = "5 4";
   _samples_return_value["compare-two-integer-parameters-lte-3"] = "false";

   _samples["compare-two-integer-parameters-gte-1"] = sample_gte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gte-1"] = "4 5";
   _samples_return_value["compare-two-integer-parameters-gte-1"] = "false";

   _samples["compare-two-integer-parameters-gte-2"] = sample_gte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gte-2"] = "4 4";
   _samples_return_value["compare-two-integer-parameters-gte-2"] = "true";

   _samples["compare-two-integer-parameters-gte-3"] = sample_gte_two_integer_parameters;
   _samples_parameters["compare-two-integer-parameters-gte-3"] = "5 4";
   _samples_return_value["compare-two-integer-parameters-gte-3"] = "true";

   _samples["sum-two-float-parameters"] = sample_sum_two_float_parameters;
   _samples_parameters["sum-two-float-parameters"] = "3.5 4.5";
   _samples_return_value["sum-two-float-parameters"] = "8.000000";

   _samples["subtract-two-float-parameters"] = sample_subtract_two_float_parameters;
   _samples_parameters["subtract-two-float-parameters"] = "3.5 4.5";
   _samples_return_value["subtract-two-float-parameters"] = "-1.000000";

   _samples["multiply-two-float-parameters"] = sample_multiply_two_float_parameters;
   _samples_parameters["multiply-two-float-parameters"] = "3.5 4.5";
   _samples_return_value["multiply-two-float-parameters"] = "15.750000";

   _samples["divide-two-float-parameters"] = sample_divide_two_float_parameters;
   _samples_parameters["divide-two-float-parameters"] = "5.0 2.0";
   _samples_return_value["divide-two-float-parameters"] = "2.500000";

   _samples["sum-two-double-parameters"] = sample_sum_two_double_parameters;
   _samples_parameters["sum-two-double-parameters"] = "3.5 4.5";
   _samples_return_value["sum-two-double-parameters"] = "8.000000";

   _samples["subtract-two-double-parameters"] = sample_subtract_two_double_parameters;
   _samples_parameters["subtract-two-double-parameters"] = "3.5 4.5";
   _samples_return_value["subtract-two-double-parameters"] = "-1.000000";

   _samples["multiply-two-double-parameters"] = sample_multiply_two_double_parameters;
   _samples_parameters["multiply-two-double-parameters"] = "3.5 4.5";
   _samples_return_value["multiply-two-double-parameters"] = "15.750000";

   _samples["divide-two-double-parameters"] = sample_divide_two_double_parameters;
   _samples_parameters["divide-two-double-parameters"] = "5.0 2.0";
   _samples_return_value["divide-two-double-parameters"] = "2.500000";

   _samples["ternary-expression-1"] = sample_ternary_expression;
   _samples_parameters["ternary-expression-1"] = "true";
   _samples_return_value["ternary-expression-1"] = "1";

   _samples["ternary-expression-2"] = sample_ternary_expression;
   _samples_parameters["ternary-expression-2"] = "false";
   _samples_return_value["ternary-expression-2"] = "2";

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
      return it->second;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}

std::string gasp_module_blaise_sample::get_input(std::string sample) const
{
   auto it = _samples_parameters.find(sample);
   if (it != _samples_parameters.end())
      return it->second;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}
std::string gasp_module_blaise_sample::get_output(std::string sample) const
{
   auto it = _samples_return_value.find(sample);
   if (it != _samples_return_value.end())
      return it->second;
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
      auto it = _samples_parameters.find(arg);
      if (it != _samples_parameters.end())
      {
         std::cout << it->second << std::endl;
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
      auto it = _samples_return_value.find(arg);
      if (it != _samples_return_value.end())
      {
         std::cout << it->second << std::endl;
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
         std::cout << it->second << std::endl;
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
