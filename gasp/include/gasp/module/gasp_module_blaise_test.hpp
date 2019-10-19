#pragma once

#include <memory>
#include <string>
#include <utility>
#include <chrono>

#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

namespace gasp::module
{

class gasp_module_blaise_test : public gasp_module
{
   std::shared_ptr<gasp_module_blaise_sample> _module_samples;

   bool run_tests(std::string regular_expression) const;
   bool run_test(std::string sample, std::chrono::milliseconds& elapsed_time) const;

   void print_usage(char* argv[]) const;

public:
   gasp_module_blaise_test(std::shared_ptr<gasp_module_blaise_sample> module_samples);
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char *argv[]) override;
   ~gasp_module_blaise_test();
};

} // namespace gasp::module