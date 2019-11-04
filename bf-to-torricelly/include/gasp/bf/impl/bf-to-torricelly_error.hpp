#pragma once

#include <string>
#include <stdexcept>

namespace gasp::bf
{

class bf_to_torricelly_error : public std::domain_error
{
public:
   bf_to_torricelly_error(const std::string& message);
   bf_to_torricelly_error(const char* message);
};

} // namespace gasp::bf