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

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = sample_empty;
   _samples_parameters["empty"] = "";
   _samples_return_value["empty"] = "";

   _samples["empty-with-parameters"] = sample_empty_with_parameters;
   _samples_parameters["empty-with-parameters"] = "7 5";
   _samples_return_value["empty-with-parameters"] = "";

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
