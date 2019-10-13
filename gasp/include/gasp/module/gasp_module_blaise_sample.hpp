#pragma once

#include <string>
#include <vector>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_blaise_test;

class gasp_module_blaise_sample : public gasp_module
{
   void get_sample_names(std::vector<std::string>& samples) const;
   std::string get_sample(std::string sample) const;
   std::string get_input(std::string sample) const;
   std::string get_output(std::string sample) const;

public:
   gasp_module_blaise_sample();
   std::string name() const override;
   std::string description() const override;
   bool run(int argc, char* argv[]) override;
   virtual ~gasp_module_blaise_sample();

   friend gasp_module_blaise_test;
};

} // namespace gasp::module