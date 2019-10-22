#pragma once

#include <memory>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include <gasp/torricelly/impl/torricelly_type.hpp>

namespace gasp::torricelly
{

union torricelly_value_union {
   int _integer;
   char _char;
   bool _boolean;
   float _float;
   double _double;
   std::string _string_literal;

   torricelly_value_union();
   ~torricelly_value_union();
};

class torricelly_value
{
   torricelly_value_union _value;
   std::shared_ptr<torricelly_type> _type;
   std::shared_ptr<std::vector<torricelly_value>> _array;
   torricelly_value(torricelly_system_type_type system_type, const torricelly_value_union &value);
   torricelly_value(std::shared_ptr<torricelly_array_type> type, torricelly_value initial_value);
   void throw_if_is_not(std::shared_ptr<torricelly_type> check_type) const;
   void copy_value(const torricelly_value_union &value);
   void copy_array(std::shared_ptr<std::vector<torricelly_value>> array);

public:
   torricelly_value(const torricelly_value &other);
   torricelly_value &operator=(const torricelly_value &other);

   std::shared_ptr<torricelly_type> type() const;

   int get_integer() const;
   char get_char() const;
   bool get_boolean() const;
   float get_float() const;
   double get_double() const;
   std::string get_string_literal() const;
   std::shared_ptr<std::vector<torricelly_value>> get_array() const;

   bool match(std::shared_ptr<torricelly_type> type) const;

   static torricelly_value make(int value);
   static torricelly_value make(bool value);
   static torricelly_value make(char value);
   static torricelly_value make(float value);
   static torricelly_value make(double value);
   static torricelly_value make(std::string value);
   static torricelly_value make(std::shared_ptr<torricelly_array_type> type, torricelly_value initial_value);

   static torricelly_value get_default_value(std::shared_ptr<torricelly_type> type);
   static torricelly_value get_value_from_string(const std::string &value, std::shared_ptr<torricelly_type> type);
};

} // namespace gasp::torricelly