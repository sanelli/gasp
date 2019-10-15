
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>

#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>
#include <gasp/module/gasp_module_blaise_test.hpp>

#include <gasp/common/gasp_error.hpp>
#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::module;
using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;
using namespace gasp::torricelly::interpreter;

// https://stackoverflow.com/a/9158263/1468832
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const std::string p_help("--help");
const std::string p_help_short("-h");
const std::string p_execute("--execute");
const std::string p_execute_short("-x");

gasp_module_blaise_test::gasp_module_blaise_test(std::shared_ptr<gasp_module_blaise_sample> module_samples)
    : _module_samples(module_samples) {}

std::string gasp_module_blaise_test::name() const { return "blaise-test"; }

std::string gasp_module_blaise_test::description() const { return "Run tests on all the the available balise tests"; }

bool gasp_module_blaise_test::run(int argc, char *argv[])
{
   auto success = true;
   switch (argc)
   {
   case 2:
      success = run_tests();
      break;
   case 3:
   {
      std::string arg(argv[2]);
      if (arg == p_help || arg == p_help_short)
      {
         print_usage(argv);
      }
      else if (arg == p_execute || arg == p_execute_short)
      {
         success = run_tests();
      }
      else
      {
         std::cerr << "Ivalid parameters." << std::endl;
         print_usage(argv);
      }
   }
   break;
   default:
      std::cerr << "Ivalid parameters." << std::endl;
      print_usage(argv);
      success = false;
      break;
   }
   return success;
}

gasp_module_blaise_test::~gasp_module_blaise_test()
{
   _module_samples = nullptr;
}

void gasp_module_blaise_test::print_usage(char *argv[]) const
{
   std::cerr << "Command line: " << argv[0] << " " << name() << " [options]" << std::endl;
   std::cerr << "Options:" << std::endl;
   std::cerr << "\t" << p_help << "/" << p_help_short << ": Display help and usage." << std::endl;
   std::cerr << "\t" << p_execute << "/" << p_execute_short << ": Run the test on all the samples (default)." << std::endl;
}

bool gasp_module_blaise_test::run_tests() const
{
   unsigned success = 0;
   unsigned failure = 0;

   std::vector<std::string> samples;
   _module_samples->get_sample_names(samples);

   auto wsize = samples.size() > 0 ? (unsigned int)std::ceil(std::log10(samples.size())) : 0;
   size_t max_sample_name_size = 0;
   for (const std::string &sample : samples) max_sample_name_size = std::max(max_sample_name_size, sample.size());

   std::cout << std::setprecision(4);

   auto index = 0;
   for (const std::string &sample : samples)
   {
      std::cout << std::right << std::setw(wsize) << (++index) << std::setw(0) << std::left
            << ") " << std::setw(max_sample_name_size) << sample << std::setw(0) << " ";
      std::chrono::milliseconds elapsed_time;
      auto result = run_test(sample, elapsed_time);
      if (result)
      {
         std::cout  << GREEN << "[OK] " << RESET << BLUE << "[" << elapsed_time.count() << " ms]" << RESET << std::endl;
         ++success;
      }
      else
      {
         std::cout << RED << "[FAIL]" << RESET << std::endl;
         ++failure;
      }
   }

   auto total = success + failure;
   if (total == 0)
   {
      std::cout << "No test run." << std::endl;
   }
   else
   {
      std::cout << std::endl << "Result:" << std::endl;
      std::cout << GREEN << "   Success: " << success << "/" << total << " (" << (success * 100.00f / total) << "%)" << RESET << std::endl;
      std::cout << RED << "   Failure: " << failure << "/" << total << " (" << (failure * 100.00f / total) << "%)" << RESET << std::endl;
   }

   return failure == 0;
}

bool gasp_module_blaise_test::run_test(std::string sample, std::chrono::milliseconds& elapsed_time) const
{
   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;
   std::vector<std::shared_ptr<torricelly_module>> modules;

   try
   {
      std::stringstream input_stream(_module_samples->get_sample(sample));
      std::istringstream parameters_stream{_module_samples->get_input(sample)};
      std::vector<std::string> parameters{
          std::istream_iterator<std::string>{parameters_stream},
          std::istream_iterator<std::string>{}};

      auto begin = std::chrono::high_resolution_clock::now();
      tokenizer.tokenize(input_stream, context);
      parser.parse(context);
      translator translator(context.module());
      translator.execute(modules);
      auto interpreter = make_torricelly_interpreter(
          modules.at(0),
          [parameters](unsigned int index) { return index < parameters.size() ? parameters.at(index) : ""; });
      interpreter->initialize();
      interpreter->run();
      auto end = std::chrono::high_resolution_clock::now();

      elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);

      if (interpreter->status() == torricelly_interpreter_status::FINISHED)
      {
         auto return_value = interpreter->return_value();
         // Assuming at present we are only getting back integer (will fixx this soon I hope)
         return gasp::torricelly::interpreter::to_string(return_value) == _module_samples->get_output(sample);
      }
      else
      {
         return false;
      }

      return interpreter->status() == torricelly_interpreter_status::FINISHED;
   }
   catch (const sanelli::tokenizer_error &error)
   {
      return false;
   }
   catch (const sanelli::parser_error &error)
   {
      return false;
   }
   catch (const gasp::blaise::ast::blaise_ast_error &error)
   {
      return false;
   }
   catch (const gasp::common::gasp_internal_error &error)
   {
      return false;
   }
   catch (const gasp::blaise_to_torricelly::blaise_to_torricelly_internal_error &error)
   {
      return false;
   }
   catch (const gasp::blaise_to_torricelly::blaise_to_torricelly_error &error)
   {
      return false;
   }
   catch (const gasp::torricelly::interpreter::torricelly_interpreter_error &error)
   {
      return false;
   }
   catch (const gasp::torricelly::interpreter::torricelly_interpreter_execution_error &error)
   {
      return false;
   }
   catch (const std::exception &error)
   {
      return false;
   }
   catch (...)
   {
      return false;
   }

   return true;
}
