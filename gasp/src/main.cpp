#include <map>
#include <memory>
#include <stdexcept>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>

#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_tokenize.hpp>
#include <gasp/module/gasp_module_compile.hpp>
#include <gasp/module/gasp_module_execute.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>
#include <gasp/module/gasp_module_debug.hpp>

// TODO: Add error codes when I throw errors

void print_available_modules(const std::map<std::string, std::unique_ptr<gasp::module::gasp_module>> &modules)
{
   for (auto it = modules.begin(); it != modules.end(); ++it)
   {
      std::cerr << "   * " << it->first << " : " << it->second->description() << std::endl;
   }
}

int main(int argc, char *argv[])
{
   SANELLI_INSTALL_DEBUGGER("blaise-parser");
   SANELLI_INSTALL_DEBUGGER("sanelli-parser");
   SANELLI_INSTALL_DEBUGGER("blaise-lang");
   SANELLI_INSTALL_DEBUGGER("sanelli-tokenizer");
   SANELLI_INSTALL_DEBUGGER("torricelly-lang");
   SANELLI_INSTALL_DEBUGGER("blaise-to-torricelly");
   SANELLI_INSTALL_DEBUGGER("torricelly-interpreter");

   std::map<std::string, std::unique_ptr<gasp::module::gasp_module>> modules;

   auto tokenize_module = std::make_unique<gasp::module::gasp_module_tokenize>();
   auto compile_module = std::make_unique<gasp::module::gasp_module_compile>();
   auto execute_module = std::make_unique<gasp::module::gasp_module_execute>();
   auto blaise_sample_module = std::make_unique<gasp::module::gasp_module_blaise_sample>();
   auto debug_module = std::make_unique<gasp::module::gasp_module_debug>();

   modules[tokenize_module->name()] = std::move(tokenize_module);
   modules[compile_module->name()] = std::move(compile_module);
   modules[execute_module->name()] = std::move(execute_module);
   modules[debug_module->name()] = std::move(debug_module);
   modules[blaise_sample_module->name()] = std::move(blaise_sample_module);

   if (argc < 2)
   {
      std::cerr << "No module selected. Available modules are: " << std::endl;
      print_available_modules(modules);
      return EXIT_FAILURE;
   }

   auto expected_module = std::string(argv[1]);
   std::unique_ptr<gasp::module::gasp_module> module = nullptr;
   for (auto it = modules.begin(); it != modules.end(); ++it)
   {
      if (it->first == expected_module)
      {
         module = std::move(it->second);
         break;
      }
   }

   // Either I found the module or not I do not need to waste
   // memory with something I will never user.
   modules.clear();

   if (module == nullptr)
   {
      std::cerr << "Cannot find module '" << expected_module << "'. Available modules are:" << std::endl;
      print_available_modules(modules);
      return EXIT_FAILURE;
   }

   try
   {
      auto success = module->run(argc, argv);
      return success ? EXIT_SUCCESS : EXIT_FAILURE;
   }
   catch (const gasp::common::gasp_error &e)
   {
      std::cerr << "Error while executing module '" << module->name() << "': " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch (const std::exception &e)
   {
      std::cerr << "Unexpected error while executing module '" << module->name() << "': " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch (...)
   {
      std::cerr << "An unexpected error occurred. No futher details available." << std::endl;
      return EXIT_FAILURE;
   }
}