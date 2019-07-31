#pragma once

#include <memory>
#include <string>
#include <gasp/torricelly/impl/torricelly_type.hpp>

namespace gasp::torricelly {

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

class torricelly_value {
   torricelly_value_union _value;
   torricelly_type_type _type;
   torricelly_system_type_type _system_type;
   torricelly_value(torricelly_system_type_type system_type, const torricelly_value_union& value);
   void throw_if_is_not(torricelly_system_type_type system_type) const;
   void copy_value(const torricelly_value_union& value);
public:
   torricelly_value(const torricelly_value& other);
   torricelly_value& operator=(const torricelly_value& other);

   torricelly_type_type type() const;
   torricelly_system_type_type system_type() const;

   bool is(torricelly_system_type_type system_type) const;

   int get_integer() const;
   char get_char() const;
   bool get_boolean() const;
   float get_float() const;
   double get_double() const;
   std::string get_string_literal() const;

   bool match(std::shared_ptr<torricelly_type> type) const;

   static torricelly_value make(int value);
   static torricelly_value make(bool value);
   static torricelly_value make(char value);
   static torricelly_value make(float value);
   static torricelly_value make(double value);
   static torricelly_value make(std::string value);
};


}