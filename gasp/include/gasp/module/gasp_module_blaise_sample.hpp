#pragma once

#include <string>
#include <vector>
#include <map>

#include <gasp/module/gasp_module.hpp>

namespace gasp::module
{

class gasp_module_blaise_test;

struct gasp_module_blaise_sample_type
{
   std::string sample;
   std::string input;
   std::string output;
};

class gasp_module_blaise_sample : public gasp_module
{
   std::map<std::string, gasp_module_blaise_sample_type> _samples;

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