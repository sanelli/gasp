#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>

namespace gasp::torricelly {

class torricelly_subroutine {
   std::string _name;
   std::shared_ptr<torricelly::torricelly_type> _return_type;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _parameters;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variables;
   torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);
public:
   std::string name() const;
   std::shared_ptr<torricelly::torricelly_type> return_type() const;
   int add_parameter(std::shared_ptr<torricelly::torricelly_type> type);
   int add_variable(std::shared_ptr<torricelly::torricelly_type> type);

   friend gasp::common::memory;
};

std::shared_ptr<torricelly_subroutine> make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type);

}