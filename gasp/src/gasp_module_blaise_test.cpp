
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

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
      run_tests();
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
         run_tests();
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

void gasp_module_blaise_test::run_tests() const
{
   unsigned success = 0;
   unsigned failure = 0;

   std::vector<std::string> samples;
   _module_samples->get_sample_names(samples);

   for (const std::string &sample : samples)
   {
      std::cout << "Running '" << sample << "'... ";
      std::chrono::milliseconds elapsed_time;
      auto result = run_test(sample, elapsed_time);
      if (result)
      {
         std::cout << "OK [" << elapsed_time.count() << " ms]" << std::endl;
         ++success;
      }
      else
      {
         std::cout << "FAIL" << std::endl;
         ++failure;
      }
   }

   auto total = success + failure;
   if (total == 0)
   {
      std::cout << "No test run" << std::endl;
   }
   else
   {
      std::cout << "Success: " << success << "/" << total << "(" << (success * 100.00f / total) << "%)" << std::endl;
      std::cout << "Failure: " << failure << "/" << total << "(" << (failure * 100.00f / total) << "%)" << std::endl;
   }
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
         return std::to_string(return_value.get_integer()) == _module_samples->get_output(sample);
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
