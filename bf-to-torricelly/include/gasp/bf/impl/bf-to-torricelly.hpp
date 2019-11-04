#pragma once

#include <memory>
#include <istream>
#include <vector>
#include <utility>
#include <string>

#include <gasp/torricelly/torricelly.hpp>

namespace gasp::bf
{

class bf_to_torricelly_translator
{
   std::vector<std::pair<unsigned int, unsigned int>> _labels;

public:
   std::shared_ptr<gasp::torricelly::torricelly_module> translate(std::string module_name, std::istream &input);
};

} // namespace gasp::bf