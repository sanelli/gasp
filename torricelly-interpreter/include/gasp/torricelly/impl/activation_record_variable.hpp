#pragma once

#include <string>
#include <memory>

#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter
{

union torricelly_activation_record_variable_union {
   int _integer;
   char _char;
   bool _boolean;
   float _float;
   double _double;
   std::shared_ptr<void> _pointer;

   torricelly_activation_record_variable_union();
   ~torricelly_activation_record_variable_union();
};

enum class torricelly_activation_record_variable_type
{
   UNDEFINED,
   VOID,
   INTEGER,
   CHAR,
   BOOLEAN,
   FLOAT,
   DOUBLE,
   POINTER
};

std::string to_string(torricelly_activation_record_variable_type type);

class torricelly_activation_record_variable
{
   torricelly_activation_record_variable_type _type;
   torricelly_activation_record_variable_union _value;

   torricelly_activation_record_variable(int i);
   torricelly_activation_record_variable(char c);
   torricelly_activation_record_variable(bool b);
   torricelly_activation_record_variable(float f);
   torricelly_activation_record_variable(double d);
   torricelly_activation_record_variable(std::shared_ptr<void> p);

   void copy_value_from(const torricelly_activation_record_variable &other);

public:
   torricelly_activation_record_variable();
   ~torricelly_activation_record_variable();
   torricelly_activation_record_variable(const torricelly_activation_record_variable &other);
   torricelly_activation_record_variable(torricelly_activation_record_variable &&other);
   torricelly_activation_record_variable &operator=(const torricelly_activation_record_variable &other);
   torricelly_activation_record_variable &operator=(torricelly_activation_record_variable &&other);

   torricelly_activation_record_variable_type type() const;
   bool match(std::shared_ptr<torricelly::torricelly_type> torricelly_type) const;

   int get_integer() const;
   char get_char() const;
   bool get_boolean() const;
   float get_float() const;
   double get_double() const;
   std::shared_ptr<void> get_pointer() const;

   void set_integer(int i);
   void set_char(char c);
   void set_boolean(bool b);
   void set_float(float f);
   void set_double(double d);
   void set_pointer(std::shared_ptr<void> p);

   static torricelly_activation_record_variable make(const torricelly_value &value);

   friend sanelli::memory;
};

} // namespace gasp::torricelly::interpreter