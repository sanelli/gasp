#include <string>
#include <iostream>
#include <regex>
#include <iomanip>

#include <gasp/common/gasp_error.hpp>
#include <gasp/module/gasp_module_blaise_sample.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::module;

auto sample_empty = R"__(program sample;
begin
end.)__";

auto sample_empty_with_parameters = R"__(program sample(first: integer, second: boolean);
begin
end.)__";

std::string generate_empty_with_return_type_sample(const char *return_type)
{
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
begin
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);

   return sample;
}

std::string generate_return_constant_sample(const char *return_type, const char *value)
{
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
const
   value := {VALUE};
begin
   sample := value;
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, value_regexp, value);

   return sample;
}

std::string generate_sample_expression(const char *return_type, const char *expression)
{
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex expression_regexp("\\{EXPRESSION\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
begin
   sample := {EXPRESSION};
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, expression_regexp, expression);

   return sample;
}

std::string generate_function_constant_sample(const char *return_type, const char *value)
{
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
function test() : {RETURN_TYPE};
const
   value := {VALUE};
begin
   test := value;
end;

begin
   sample := test();
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, value_regexp, value);

   return sample;
}

auto generate_declare_procedure = R"__(program sample;

declare procedure test_second();

procedure test_first();
begin
  test_second();
end;

procedure test_second();
begin
end;

begin
  test_first();
end.)__";

auto generate_declare_procedure_with_params = R"__(program sample;

declare procedure test_second(a:integer,b:float);

procedure test_first(a:integer,b:float);
begin
  test_second(a,b);
end;

procedure test_second(a:integer,b:float);
begin
end;

begin
  test_first(10, 1.2f);
end.)__";

auto generate_declare_function = R"__(program sample : integer;

declare function test_second() : integer;

function test_first() : integer;
begin
  test_first := test_second();
end;

function test_second() : integer;
begin
   test_second := 99;
end;

begin
  sample := test_first();
end.)__";

auto generate_declare_function_with_params = R"__(program sample : integer;

declare function test_second(a: integer, b: integer) : integer;

function test_first(a:integer) : integer;
begin
  test_first := test_second(a,a);
end;

function test_second(a: integer, b: integer) : integer;
begin
   test_second := a + b;
end;

begin
  sample := test_first(8);
end.)__";

std::string generate_binary_operator_sample(const char *operator_type, const char *return_type, const char *binary_operator)
{
   std::regex operator_type_regexp("\\{OPERATOR_TYPE\\}");
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex binary_operator_regexp("\\{OPERATOR\\}");

   std::string sample(R"__(program sample(first: {OPERATOR_TYPE}, second: {OPERATOR_TYPE}): {RETURN_TYPE};
begin
   sample := first {OPERATOR} second;
end.)__");

   sample = std::regex_replace(sample, operator_type_regexp, operator_type);
   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, binary_operator_regexp, binary_operator);

   return sample;
}

std::string generate_unary_operator_sample(const char *operator_type, const char *return_type, const char *binary_operator)
{
   std::regex operator_type_regexp("\\{OPERATOR_TYPE\\}");
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex binary_operator_regexp("\\{OPERATOR\\}");

   std::string sample(R"__(program sample(first: {OPERATOR_TYPE}): {RETURN_TYPE};
begin
   sample := {OPERATOR} first;
end.)__");

   sample = std::regex_replace(sample, operator_type_regexp, operator_type);
   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, binary_operator_regexp, binary_operator);

   return sample;
}

std::string generate_literal_assignment_sample(const char *return_type, const char *literal)
{
   std::regex return_type_regexp("\\{RETURN_TYPE\\}");
   std::regex literal_regexp("\\{LITERAL\\}");

   std::string sample(R"__(program sample: {RETURN_TYPE};
begin
   sample := {LITERAL};
end.)__");

   sample = std::regex_replace(sample, return_type_regexp, return_type);
   sample = std::regex_replace(sample, literal_regexp, literal);

   return sample;
}

std::string generate_cast_sample(const char *from_type, const char *target_type)
{
   std::regex from_type_regexp("\\{FROM_TYPE\\}");
   std::regex target_type_regexp("\\{TARGET_TYPE\\}");

   std::string sample(R"__(program sample(first: {FROM_TYPE}): {TARGET_TYPE};
begin
   sample := cast<{TARGET_TYPE}>(first);
end.)__");

   sample = std::regex_replace(sample, from_type_regexp, from_type);
   sample = std::regex_replace(sample, target_type_regexp, target_type);

   return sample;
}

auto sample_ternary_expression = R"__(program sample(first: boolean);
begin
   sample := if first then 1 else 2;
end.)__";

auto sample_statement_if_then = R"__(program sample(first: boolean): integer;
begin
   sample := 1;
   if (first) then 
      sample := 2;
end.)__";

auto sample_statement_if_then_else = R"__(program sample(first: boolean): integer;
begin
   sample := 1;
   if first then 
      sample := 2;
   else 
      sample := 3;
end.)__";

auto sample_statement_for = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   for index from 1 to first begin
      sample := sample + index;
   end
end.)__";

auto sample_statement_for_step = R"__(program sample(first: integer, second: integer): integer;
var
   index: integer;
begin
   sample := 0;
   for index from 1 to first step second begin
      sample := sample + index;
   end
end.)__";

auto sample_statement_empty = R"__(program sample;
var
begin
 ; ; ; ; ;
end.)__";

auto sample_statement_while = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   while index <= first begin
      sample := sample + index;
      index := index + 1;
   end;
end.)__";

auto sample_statement_do_while = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   do begin
      sample := sample + index;
      index := index + 1;
   end while index <= first;
end.)__";

auto sample_statement_repeat_until = R"__(program sample(first: integer): integer;
var
   index: integer;
begin
   sample := 0;
   index := 1;
   repeat begin
      sample := sample + index;
      index := index + 1;
   end until index > first;
end.)__";

std::string generate_function_expression_sample(const char *type, const char *postfix = nullptr)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex postfix_regexp("\\{POSTFIX\\}");

   std::string sample(R"__(program sample(first: {TYPE}): {TYPE};

function duplicate(p: {TYPE}) : {TYPE};
begin
   duplicate := 2{POSTFIX} * p;
end;

begin
   sample := duplicate(first);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   if (postfix != nullptr)
      sample = std::regex_replace(sample, postfix_regexp, postfix);
   else
      sample = std::regex_replace(sample, postfix_regexp, "");
   return sample;
}

std::string generate_function_expression_2_sample(const char *type, const char *op)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex op_regexp("\\{OP\\}");

   std::string sample(R"__(program sample(first: {TYPE}, second: {TYPE}): {TYPE};

function execute(p1: {TYPE}, p2: {TYPE}) : {TYPE};
begin
   execute := p1 {OP} p2;
end;

begin
   sample := execute(first, second);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, op_regexp, op);

   return sample;
}

auto sample_statement_function = R"__(program sample(first: integer): integer;

function execute(p1: integer) : integer;
begin
   execute := p1 + p1 + p1;
end;

begin
   execute(first);
end.)__";

auto sample_algorithm_fibonacci = R"__(program fibonacci(input: integer) : integer;

function fibo(i : integer): integer;
begin
   fibo := 1;
   if i > 1 then
      fibo := fibo(i-2) + fibo(i-1);
end;

begin
   fibonacci := fibo(input);
end.)__";

auto sample_algorithm_isprime = R"__(program isprime(input: integer) : boolean;
var 
   limit : integer;
   index : integer;
begin
   limit := input / 2;
   index := 2;
   isprime := true;
   while isprime and (index <= limit) begin
      isprime := (input % index) <> 0;
      index := index + 1;
   end;
end.)__";

std::string generate_array_load_and_store(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};
var
   numbers: array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      numbers[index] := cast<{TYPE}>(index);
   end
   sample := cast<{TYPE}>(0);
   for index from 0 to ({SIZE}-1) begin
      sample := sample + numbers[index];
   end;
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_array_load_and_store_non_numeric(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};
var
   numbers: array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      numbers[index] := cast<{TYPE}>(index);
   end
   sample := numbers[{SIZE}-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_array_assignemt(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};
var
   a1, a2: array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      a1[index] := cast<{TYPE}>(index);
   end
   a2 := a1;
   sample := a2[{SIZE}-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_array_unbound_assignemt(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};
var
   a1: array<{TYPE}>[{SIZE}];
   a2: array<{TYPE}>[];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      a1[index] := cast<{TYPE}>(index);
   end
   a2 := a1;
   sample := a2[{SIZE}-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_two_dimension_array_load_and_store(const char *type, unsigned int size1, unsigned int size2)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size1_regexp("\\{SIZE_1\\}");
   std::regex size2_regexp("\\{SIZE_2\\}");

   std::string sample(R"__(program sample : {TYPE};
const
  size1 := {SIZE_1};
  size2 := {SIZE_2};
var
   numbers: array<{TYPE}>[{SIZE_1},{SIZE_2}];
   i, j: integer;
begin
   for i from 0 to (size1-1) 
      for j from 0 to (size2-1) begin
         numbers[i,j] := cast<{TYPE}>(i * j);
      end;
   sample := numbers[size1-1, size2-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size1_regexp, std::to_string(size1));
   sample = std::regex_replace(sample, size2_regexp, std::to_string(size2));

   return sample;
}

std::string generate_three_dimension_array_load_and_store(const char *type, unsigned int size1, unsigned int size2, unsigned int size3)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size1_regexp("\\{SIZE_1\\}");
   std::regex size2_regexp("\\{SIZE_2\\}");
   std::regex size3_regexp("\\{SIZE_3\\}");

   std::string sample(R"__(program sample : {TYPE};
const
  size1 := {SIZE_1};
  size2 := {SIZE_2};
  size3 := {SIZE_3};
var
   numbers: array<{TYPE}>[{SIZE_1},{SIZE_2},{SIZE_3}];
   i, j, k: integer;
begin
   for i from 0 to (size1-1) 
      for j from 0 to (size2-1) 
         for k from 0 to (size3-1) begin
            numbers[i,j,k] := cast<{TYPE}>(i * j * k);
         end;
   sample := numbers[size1-1, size2-1, size3-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size1_regexp, std::to_string(size1));
   sample = std::regex_replace(sample, size2_regexp, std::to_string(size2));
   sample = std::regex_replace(sample, size3_regexp, std::to_string(size3));

   return sample;
}

std::string generate_array_passing_unbound(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};

function sum(size: integer, numbers: array<{TYPE}>[]) : {TYPE};
var index: integer;
begin
   sum := cast<{TYPE}>(0);
   for index from 0 to (size-1) begin
      sum := sum + numbers[index];
   end;
end;

var
   numbers: array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      numbers[index] := cast<{TYPE}>(index);
   end
   sample := sum({SIZE}, numbers);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_two_dimension_array_unbound_passing(const char *type, unsigned int size1, unsigned int size2)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size1_regexp("\\{SIZE_1\\}");
   std::regex size2_regexp("\\{SIZE_2\\}");

   std::string sample(R"__(program sample : {TYPE};

procedure fill(a: array<{TYPE}>[,], size1: integer, size2: integer);
var
   i, j: integer;
begin
  for i from 0 to (size1-1) 
      for j from 0 to (size2-1) begin
         a[i,j] := cast<{TYPE}>(i * j);
      end;
end;

const
  size1 := {SIZE_1};
  size2 := {SIZE_2};


var
   numbers: array<{TYPE}>[{SIZE_1},{SIZE_2}];
begin
   fill(numbers, size1, size2);
   sample := numbers[size1-1, size2-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size1_regexp, std::to_string(size1));
   sample = std::regex_replace(sample, size2_regexp, std::to_string(size2));

   return sample;
}

std::string generate_two_dimension_array_bound_passing(const char *type, unsigned int size1, unsigned int size2)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size1_regexp("\\{SIZE_1\\}");
   std::regex size2_regexp("\\{SIZE_2\\}");

   std::string sample(R"__(program sample : {TYPE};

procedure fill(a: array<{TYPE}>[{SIZE_1},{SIZE_2}]);
const
  size1 := {SIZE_1};
  size2 := {SIZE_2};
var
   i, j: integer;
begin
  for i from 0 to (size1-1) 
      for j from 0 to (size2-1) begin
         a[i,j] := cast<{TYPE}>(i * j);
      end;
end;

var
   numbers: array<{TYPE}>[{SIZE_1},{SIZE_2}];
begin
   fill(numbers);
   sample := numbers[{SIZE_1}-1, {SIZE_2}-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size1_regexp, std::to_string(size1));
   sample = std::regex_replace(sample, size2_regexp, std::to_string(size2));

   return sample;
}

std::string generate_array_passing_bound(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};

function sum(size: integer, numbers: array<{TYPE}>[{SIZE}]) : {TYPE};
var index: integer;
begin
   sum := cast<{TYPE}>(0);
   for index from 0 to (size-1) begin
      sum := sum + numbers[index];
   end;
end;

var
   numbers: array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to ({SIZE}-1) begin
      numbers[index] := cast<{TYPE}>(index);
   end
   sample := sum({SIZE}, numbers);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string generate_array_passed_by_reference(const char *type, unsigned int size)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");

   std::string sample(R"__(program sample : {TYPE};

procedure fill(size: integer, numbers: array<{TYPE}>[]);
var index: integer;
begin
   for index from 0 to (size-1) begin
      numbers[index] := cast<{TYPE}>(index);
   end;
end;

var
   numbers: array<{TYPE}>[{SIZE}];
begin
   fill({SIZE}, numbers);
   sample := numbers[{SIZE}-1];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, std::to_string(size));

   return sample;
}

std::string sample_call_from_library = R"__(program sample(value: integer): boolean;
use math;
begin
   sample := is_even(value);
end.
)__";

std::string sample_call_from_library_arrays = R"__(program sample(value: integer): integer;
use arrays;
var 
 a : array<integer>[2];
begin
   a[0] := 0;
   a[1] := 1;
   swap(a, 0, 1);
   sample := a[value];
end.
)__";

std::string generate_native_single_parameter_call(const char *type, const char *use, const char *function)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex use_regexp("\\{USE\\}");
   std::regex function_regexp("\\{FUNCTION\\}");

   std::string sample(R"__(program sample(number : {TYPE}) : {TYPE};
use {USE};
begin
   sample := {FUNCTION}(number);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, use_regexp, use);
   sample = std::regex_replace(sample, function_regexp, function);

   return sample;
}

std::string generate_native_array_call_to_module(const char *type, const char *size, const char *function)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex size_regexp("\\{SIZE\\}");
   std::regex function_regexp("\\{FUNCTION\\}");

   std::string sample(R"__(program sample(output_index:integer) : {TYPE};
use math;
const
  size := {SIZE};
var
   input : array<{TYPE}>[{SIZE}];
   output : array<{TYPE}>[{SIZE}];
   index: integer;
begin
   for index from 0 to size-1 begin
      input[index] := index;
   end;
   {FUNCTION}(input, output, size);
   sample := output[output_index];
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, size_regexp, size);
   sample = std::regex_replace(sample, function_regexp, function);

   return sample;
}

std::string generate_sample_allocate_array_with_math(const char *type)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::string sample = R"__(program sample : {TYPE};
var v : array<{TYPE}>[];
    i : integer;
begin
 v := new<array<{TYPE}>[]>(10);
 for i from 0 to 9 begin
    v[i] := cast<{TYPE}>(i);
 end;
 for i from 0 to 9 begin
   sample := sample + v[i];
 end;
 delete(v);
end.)__";

   sample = std::regex_replace(sample, type_regexp, type);

   return sample;
}

std::string generate_sample_allocate_array(const char *type, const char *value)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample : {TYPE};
var v : array<{TYPE}>[];
    i : integer;
begin
 v := new<array<{TYPE}>[]>(10);
 for i from 0 to 9 begin
    v[i] := cast<{TYPE}>({VALUE});
 end;
 sample := v[9];
 delete(v);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, value_regexp, value);

   return sample;
}

std::string generate_sample_allocate_bidimensional_array(const char *type, const char *value)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample : {TYPE};
var v : array<{TYPE}>[,];
    i ,j: integer;
begin
 v := new<array<{TYPE}>[,]>(10,11);
 for i from 0 to 9 
   for j from 0 to 10 begin
      v[i,j] := cast<{TYPE}>({VALUE});
   end;
 sample := v[9,10];
 delete(v);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, value_regexp, value);

   return sample;
}

std::string generate_sample_allocate_array_and_pass_as_parameter(const char *type)
{
   std::regex type_regexp("\\{TYPE\\}");
   std::string sample = R"__(program sample(size : integer) : {TYPE};

procedure initialize(values: array<{TYPE}>[], size: integer);
var i : integer;
begin
   for i from 0 to (size-1) begin
      values[i] := cast<{TYPE}>(i);
    end;
end;

function sum(values: array<{TYPE}>[], size: integer) : {TYPE};
var i : integer;
begin
  sum := cast<{TYPE}>(0);
  for i from 0 to (size-1) begin
   sum := sum + values[i];
 end;
end;

var v : array<{TYPE}>[];
begin
 v := new<array<{TYPE}>[]>(size);
 initialize(v, size);
 sample := sum(v, size);
 delete(v);
end.)__";

   sample = std::regex_replace(sample, type_regexp, type);

   return sample;
}

std::string genetare_string_test_from_string(const char* operation, const char* value, const char* type)
{
 std::regex type_regexp("\\{TYPE\\}");
 std::regex operation_regexp("\\{OPERATION\\}");
 std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample : {TYPE};
use strings;
begin
   sample := {OPERATION}({VALUE});
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, operation_regexp, operation);
   sample = std::regex_replace(sample, value_regexp, value);
   
   return sample;
}

std::string genetare_string_test_to_string(const char* value, const char* type)
{
 std::regex type_regexp("\\{TYPE\\}");
 std::regex value_regexp("\\{VALUE\\}");

   std::string sample(R"__(program sample : array<char>[];
use strings;
var
   str: array<char>[128];
   value: {TYPE};
begin
   value := cast<{TYPE}>({VALUE});
   sample := to_string(str, value);
end.)__");

   sample = std::regex_replace(sample, type_regexp, type);
   sample = std::regex_replace(sample, value_regexp, value);
   
   return sample;
}

gasp_module_blaise_sample::gasp_module_blaise_sample()
{
   _samples["empty"] = {sample_empty, "", "0"};
   _samples["empty-return-byte"] = {generate_empty_with_return_type_sample("byte"), "", "0"};
   _samples["empty-return-short"] = {generate_empty_with_return_type_sample("short"), "", "0"};
   _samples["empty-return-integer"] = {generate_empty_with_return_type_sample("integer"), "", "0"};
   _samples["empty-return-long"] = {generate_empty_with_return_type_sample("long"), "", "0"};
   _samples["empty-return-float"] = {generate_empty_with_return_type_sample("float"), "", "0.000000"};
   _samples["empty-return-boolean"] = {generate_empty_with_return_type_sample("boolean"), "", "false"};
   _samples["empty-return-double"] = {generate_empty_with_return_type_sample("double"), "", "0.000000"};
   _samples["empty-return-char"] = {generate_empty_with_return_type_sample("char"), "", "\\u00"};
   _samples["empty-parameters-1"] = {sample_empty_with_parameters, "7 true", "0"};

   _samples["empty-const-byte"] = {generate_return_constant_sample("byte", "10b"), "", "10"};
   _samples["empty-const-short"] = {generate_return_constant_sample("short", "10s"), "", "10"};
   _samples["empty-const-integer"] = {generate_return_constant_sample("integer", "10"), "", "10"};
   _samples["empty-const-long"] = {generate_return_constant_sample("long", "10l"), "", "10"};
   _samples["empty-const-float"] = {generate_return_constant_sample("float", "10.00f"), "", "10.000000"};
   _samples["empty-const-double"] = {generate_return_constant_sample("double", "10.00"), "", "10.000000"};
   _samples["empty-const-char"] = {generate_return_constant_sample("char", "'A'"), "", "A"};
   _samples["empty-const-boolean"] = {generate_return_constant_sample("boolean", "true"), "", "true"};

   _samples["empty-const-subroutine-byte"] = {generate_function_constant_sample("byte", "10b"), "", "10"};
   _samples["empty-const-subroutine-short"] = {generate_function_constant_sample("short", "10s"), "", "10"};
   _samples["empty-const-subroutine-integer"] = {generate_function_constant_sample("integer", "10"), "", "10"};
   _samples["empty-const-subroutine-long"] = {generate_function_constant_sample("long", "10l"), "", "10"};
   _samples["empty-const-subroutine-float"] = {generate_function_constant_sample("float", "10.00f"), "", "10.000000"};
   _samples["empty-const-subroutine-double"] = {generate_function_constant_sample("double", "10.00"), "", "10.000000"};
   _samples["empty-const-subroutine-char"] = {generate_function_constant_sample("char", "'A'"), "", "A"};
   _samples["empty-const-subroutine-boolean"] = {generate_function_constant_sample("boolean", "true"), "", "true"};

   _samples["empty-subroutine-declare-procedure-no-params"] = {generate_declare_procedure, "", "0"};
   _samples["empty-subroutine-declare-procedure-with-params"] = {generate_declare_procedure_with_params, "", "0"};
   _samples["empty-subroutine-declare-function-no-params"] = {generate_declare_function, "", "99"};
   _samples["empty-subroutine-declare-function-with-params"] = {generate_declare_function_with_params, "", "16"};

   _samples["expression-precedence-math-integer-01"] = {generate_sample_expression("integer", "10 + 10 - 10"), "", "10"};
   _samples["expression-precedence-math-integer-02"] = {generate_sample_expression("integer", "20 + -10"), "", "10"};
   _samples["expression-precedence-math-integer-03"] = {generate_sample_expression("integer", "10 + 30 * 0"), "", "10"};
   _samples["expression-precedence-math-integer-04"] = {generate_sample_expression("integer", "10 + 0 * 30"), "", "10"};
   _samples["expression-precedence-math-integer-05"] = {generate_sample_expression("integer", "30 * 0 + 10"), "", "10"};
   _samples["expression-precedence-math-integer-06"] = {generate_sample_expression("integer", "0 * 30 + 10"), "", "10"};
   _samples["expression-precedence-math-integer-07"] = {generate_sample_expression("integer", "9 + 30 / 3"), "", "19"};
   _samples["expression-precedence-math-integer-08"] = {generate_sample_expression("integer", "30 / 3 + 9"), "", "19"};
   _samples["expression-precedence-math-integer-09"] = {generate_sample_expression("integer", "6 * 5 / 3"), "", "10"};
   _samples["expression-precedence-math-integer-10"] = {generate_sample_expression("integer", "2 * (5+4)"), "", "18"};
   _samples["expression-precedence-math-integer-11"] = {generate_sample_expression("integer", "(5+4)*2"), "", "18"};

   _samples["expression-precedence-boolean-1"] = {generate_sample_expression("boolean", "false and true or true"), "", "true"};
   _samples["expression-precedence-boolean-2"] = {generate_sample_expression("boolean", "true or true and false"), "", "true"};
   _samples["expression-precedence-boolean-4"] = {generate_sample_expression("boolean", "false or not true"), "", "false"};
   _samples["expression-precedence-boolean-5"] = {generate_sample_expression("boolean", "false or not false"), "", "true"};

   _samples["expression-math-byte-sum"] = {generate_binary_operator_sample("byte", "byte", "+"), "3 4", "7"};
   _samples["expression-math-byte-subtract"] = {generate_binary_operator_sample("byte", "byte", "-"), "3 4", "-1"};
   _samples["expression-math-byte-multiply"] = {generate_binary_operator_sample("byte", "byte", "*"), "3 4", "12"};
   _samples["expression-math-byte-divide"] = {generate_binary_operator_sample("byte", "byte", "/"), "15 5", "3"};
   _samples["expression-math-byte-remainder"] = {generate_binary_operator_sample("byte", "byte", "%"), "6 4", "2"};
   _samples["expression-math-byte-negate-1"] = {generate_unary_operator_sample("byte", "byte", "-"), "4", "-4"};
   _samples["expression-math-byte-negate-2"] = {generate_unary_operator_sample("byte", "byte", "-"), "-4", "4"};
   _samples["expression-math-byte-negate-3"] = {generate_unary_operator_sample("byte", "byte", "-"), "0", "0"};
   _samples["expression-math-short-sum"] = {generate_binary_operator_sample("short", "short", "+"), "3 4", "7"};
   _samples["expression-math-short-subtract"] = {generate_binary_operator_sample("short", "short", "-"), "3 4", "-1"};
   _samples["expression-math-short-multiply"] = {generate_binary_operator_sample("short", "short", "*"), "3 4", "12"};
   _samples["expression-math-short-divide"] = {generate_binary_operator_sample("short", "short", "/"), "15 5", "3"};
   _samples["expression-math-short-remainder"] = {generate_binary_operator_sample("short", "short", "%"), "6 4", "2"};
   _samples["expression-math-short-negate-1"] = {generate_unary_operator_sample("short", "short", "-"), "4", "-4"};
   _samples["expression-math-short-negate-2"] = {generate_unary_operator_sample("short", "short", "-"), "-4", "4"};
   _samples["expression-math-short-negate-3"] = {generate_unary_operator_sample("short", "short", "-"), "0", "0"};
   _samples["expression-math-integer-sum"] = {generate_binary_operator_sample("integer", "integer", "+"), "3 4", "7"};
   _samples["expression-math-integer-subtract"] = {generate_binary_operator_sample("integer", "integer", "-"), "3 4", "-1"};
   _samples["expression-math-integer-multiply"] = {generate_binary_operator_sample("integer", "integer", "*"), "3 4", "12"};
   _samples["expression-math-integer-divide"] = {generate_binary_operator_sample("integer", "integer", "/"), "15 5", "3"};
   _samples["expression-math-integer-remainder"] = {generate_binary_operator_sample("integer", "integer", "%"), "6 4", "2"};
   _samples["expression-math-integer-negate-1"] = {generate_unary_operator_sample("integer", "integer", "-"), "4", "-4"};
   _samples["expression-math-integer-negate-2"] = {generate_unary_operator_sample("integer", "integer", "-"), "-4", "4"};
   _samples["expression-math-integer-negate-3"] = {generate_unary_operator_sample("integer", "integer", "-"), "0", "0"};
   _samples["expression-math-long-sum"] = {generate_binary_operator_sample("long", "long", "+"), "3 4", "7"};
   _samples["expression-math-long-subtract"] = {generate_binary_operator_sample("long", "long", "-"), "3 4", "-1"};
   _samples["expression-math-long-multiply"] = {generate_binary_operator_sample("long", "long", "*"), "3 4", "12"};
   _samples["expression-math-long-divide"] = {generate_binary_operator_sample("long", "long", "/"), "15 5", "3"};
   _samples["expression-math-long-remainder"] = {generate_binary_operator_sample("long", "long", "%"), "6 4", "2"};
   _samples["expression-math-long-negate-1"] = {generate_unary_operator_sample("long", "long", "-"), "4", "-4"};
   _samples["expression-math-long-negate-2"] = {generate_unary_operator_sample("long", "long", "-"), "-4", "4"};
   _samples["expression-math-long-negate-3"] = {generate_unary_operator_sample("long", "long", "-"), "0", "0"};
   _samples["expression-math-float-sum"] = {generate_binary_operator_sample("float", "float", "+"), "3.5 4.5", "8.000000"};
   _samples["expression-math-float-subtract"] = {generate_binary_operator_sample("float", "float", "-"), "3.5 4.5", "-1.000000"};
   _samples["expression-math-float-multiply"] = {generate_binary_operator_sample("float", "float", "*"), "3.5 4.5", "15.750000"};
   _samples["expression-math-float-divide"] = {generate_binary_operator_sample("float", "float", "/"), "5.0 2.0", "2.500000"};
   _samples["expression-math-float-negate-1"] = {generate_unary_operator_sample("float", "float", "-"), "4.000000", "-4.000000"};
   _samples["expression-math-float-negate-2"] = {generate_unary_operator_sample("float", "float", "-"), "-4.000000", "4.000000"};
   _samples["expression-math-float-negate-3"] = {generate_unary_operator_sample("float", "float", "-"), "0.000000", "-0.000000"};
   _samples["expression-math-double-sum"] = {generate_binary_operator_sample("double", "double", "+"), "3.5 4.5", "8.000000"};
   _samples["expression-math-double-subtract"] = {generate_binary_operator_sample("double", "double", "-"), "3.5 4.5", "-1.000000"};
   _samples["expression-math-double-multiply"] = {generate_binary_operator_sample("double", "double", "*"), "3.5 4.5", "15.750000"};
   _samples["expression-math-double-divide"] = {generate_binary_operator_sample("double", "double", "/"), "5.0 2.0", "2.500000"};
   _samples["expression-math-double-negate-1"] = {generate_unary_operator_sample("double", "double", "-"), "4.000000", "-4.000000"};
   _samples["expression-math-double-negate-2"] = {generate_unary_operator_sample("double", "double", "-"), "-4.000000", "4.000000"};
   _samples["expression-math-double-negate-3"] = {generate_unary_operator_sample("double", "double", "-"), "0.000000", "-0.000000"};

   _samples["expression-boolean-and-1"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "true true", "true"};
   _samples["expression-boolean-and-2"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "true false", "false"};
   _samples["expression-boolean-and-3"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "false true", "false"};
   _samples["expression-boolean-and-4"] = {generate_binary_operator_sample("boolean", "boolean", "and"), "false false", "false"};
   _samples["expression-boolean-or-1"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "true true", "true"};
   _samples["expression-boolean-or-2"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "true false", "true"};
   _samples["expression-boolean-or-3"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "false true", "true"};
   _samples["expression-boolean-or-4"] = {generate_binary_operator_sample("boolean", "boolean", "or"), "false false", "false"};
   _samples["expression-boolean-not-1"] = {generate_unary_operator_sample("boolean", "boolean", "not"), "false", "true"};
   _samples["expression-boolean-not-2"] = {generate_unary_operator_sample("boolean", "boolean", "not"), "true", "false"};

   _samples["expression-compare-byte-eq-1"] = {generate_binary_operator_sample("byte", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-byte-eq-2"] = {generate_binary_operator_sample("byte", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-byte-eq-3"] = {generate_binary_operator_sample("byte", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-byte-neq-1"] = {generate_binary_operator_sample("byte", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-byte-neq-2"] = {generate_binary_operator_sample("byte", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-byte-neq-3"] = {generate_binary_operator_sample("byte", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-byte-gt-1"] = {generate_binary_operator_sample("byte", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-byte-gt-2"] = {generate_binary_operator_sample("byte", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-byte-gt-3"] = {generate_binary_operator_sample("byte", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-byte-lt-1"] = {generate_binary_operator_sample("byte", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-byte-lt-2"] = {generate_binary_operator_sample("byte", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-byte-lt-3"] = {generate_binary_operator_sample("byte", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-byte-lte-1"] = {generate_binary_operator_sample("byte", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-byte-lte-2"] = {generate_binary_operator_sample("byte", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-byte-lte-3"] = {generate_binary_operator_sample("byte", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-byte-gte-1"] = {generate_binary_operator_sample("byte", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-byte-gte-2"] = {generate_binary_operator_sample("byte", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-byte-gte-3"] = {generate_binary_operator_sample("byte", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-short-eq-1"] = {generate_binary_operator_sample("short", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-short-eq-2"] = {generate_binary_operator_sample("short", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-short-eq-3"] = {generate_binary_operator_sample("short", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-short-neq-1"] = {generate_binary_operator_sample("short", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-short-neq-2"] = {generate_binary_operator_sample("short", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-short-neq-3"] = {generate_binary_operator_sample("short", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-short-gt-1"] = {generate_binary_operator_sample("short", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-short-gt-2"] = {generate_binary_operator_sample("short", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-short-gt-3"] = {generate_binary_operator_sample("short", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-short-lt-1"] = {generate_binary_operator_sample("short", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-short-lt-2"] = {generate_binary_operator_sample("short", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-short-lt-3"] = {generate_binary_operator_sample("short", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-short-lte-1"] = {generate_binary_operator_sample("short", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-short-lte-2"] = {generate_binary_operator_sample("short", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-short-lte-3"] = {generate_binary_operator_sample("short", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-short-gte-1"] = {generate_binary_operator_sample("short", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-short-gte-2"] = {generate_binary_operator_sample("short", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-short-gte-3"] = {generate_binary_operator_sample("short", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-integer-eq-1"] = {generate_binary_operator_sample("integer", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-integer-eq-2"] = {generate_binary_operator_sample("integer", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-integer-eq-3"] = {generate_binary_operator_sample("integer", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-integer-neq-1"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-integer-neq-2"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-integer-neq-3"] = {generate_binary_operator_sample("integer", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-integer-gt-1"] = {generate_binary_operator_sample("integer", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-integer-gt-2"] = {generate_binary_operator_sample("integer", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-integer-gt-3"] = {generate_binary_operator_sample("integer", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-integer-lt-1"] = {generate_binary_operator_sample("integer", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-integer-lt-2"] = {generate_binary_operator_sample("integer", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-integer-lt-3"] = {generate_binary_operator_sample("integer", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-integer-lte-1"] = {generate_binary_operator_sample("integer", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-integer-lte-2"] = {generate_binary_operator_sample("integer", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-integer-lte-3"] = {generate_binary_operator_sample("integer", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-integer-gte-1"] = {generate_binary_operator_sample("integer", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-integer-gte-2"] = {generate_binary_operator_sample("integer", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-integer-gte-3"] = {generate_binary_operator_sample("integer", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-long-eq-1"] = {generate_binary_operator_sample("long", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-long-eq-2"] = {generate_binary_operator_sample("long", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-long-eq-3"] = {generate_binary_operator_sample("long", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-long-neq-1"] = {generate_binary_operator_sample("long", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-long-neq-2"] = {generate_binary_operator_sample("long", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-long-neq-3"] = {generate_binary_operator_sample("long", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-long-gt-1"] = {generate_binary_operator_sample("long", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-long-gt-2"] = {generate_binary_operator_sample("long", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-long-gt-3"] = {generate_binary_operator_sample("long", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-long-lt-1"] = {generate_binary_operator_sample("long", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-long-lt-2"] = {generate_binary_operator_sample("long", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-long-lt-3"] = {generate_binary_operator_sample("long", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-long-lte-1"] = {generate_binary_operator_sample("long", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-long-lte-2"] = {generate_binary_operator_sample("long", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-long-lte-3"] = {generate_binary_operator_sample("long", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-long-gte-1"] = {generate_binary_operator_sample("long", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-long-gte-2"] = {generate_binary_operator_sample("long", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-long-gte-3"] = {generate_binary_operator_sample("long", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-float-eq-1"] = {generate_binary_operator_sample("float", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-float-eq-2"] = {generate_binary_operator_sample("float", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-float-eq-3"] = {generate_binary_operator_sample("float", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-float-neq-1"] = {generate_binary_operator_sample("float", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-float-neq-2"] = {generate_binary_operator_sample("float", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-float-neq-3"] = {generate_binary_operator_sample("float", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-float-gt-1"] = {generate_binary_operator_sample("float", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-float-gt-2"] = {generate_binary_operator_sample("float", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-float-gt-3"] = {generate_binary_operator_sample("float", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-float-lt-1"] = {generate_binary_operator_sample("float", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-float-lt-2"] = {generate_binary_operator_sample("float", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-float-lt-3"] = {generate_binary_operator_sample("float", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-float-lte-1"] = {generate_binary_operator_sample("float", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-float-lte-2"] = {generate_binary_operator_sample("float", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-float-lte-3"] = {generate_binary_operator_sample("float", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-float-gte-1"] = {generate_binary_operator_sample("float", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-float-gte-2"] = {generate_binary_operator_sample("float", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-float-gte-3"] = {generate_binary_operator_sample("float", "boolean", ">="), "5 4", "true"};
   _samples["expression-compare-double-eq-1"] = {generate_binary_operator_sample("double", "boolean", "=="), "5 5", "true"};
   _samples["expression-compare-double-eq-2"] = {generate_binary_operator_sample("double", "boolean", "=="), "4 5", "false"};
   _samples["expression-compare-double-eq-3"] = {generate_binary_operator_sample("double", "boolean", "=="), "5 4", "false"};
   _samples["expression-compare-double-neq-1"] = {generate_binary_operator_sample("double", "boolean", "<>"), "4 5", "true"};
   _samples["expression-compare-double-neq-2"] = {generate_binary_operator_sample("double", "boolean", "<>"), "5 5", "false"};
   _samples["expression-compare-double-neq-3"] = {generate_binary_operator_sample("double", "boolean", "<>"), "5 4", "true"};
   _samples["expression-compare-double-gt-1"] = {generate_binary_operator_sample("double", "boolean", ">"), "6 5", "true"};
   _samples["expression-compare-double-gt-2"] = {generate_binary_operator_sample("double", "boolean", ">"), "4 5", "false"};
   _samples["expression-compare-double-gt-3"] = {generate_binary_operator_sample("double", "boolean", ">"), "4 4", "false"};
   _samples["expression-compare-double-lt-1"] = {generate_binary_operator_sample("double", "boolean", "<"), "4 5", "true"};
   _samples["expression-compare-double-lt-2"] = {generate_binary_operator_sample("double", "boolean", "<"), "6 5", "false"};
   _samples["expression-compare-double-lt-3"] = {generate_binary_operator_sample("double", "boolean", "<"), "5 5", "false"};
   _samples["expression-compare-double-lte-1"] = {generate_binary_operator_sample("double", "boolean", "<="), "4 5", "true"};
   _samples["expression-compare-double-lte-2"] = {generate_binary_operator_sample("double", "boolean", "<="), "4 4", "true"};
   _samples["expression-compare-double-lte-3"] = {generate_binary_operator_sample("double", "boolean", "<="), "5 4", "false"};
   _samples["expression-compare-double-gte-1"] = {generate_binary_operator_sample("double", "boolean", ">="), "4 5", "false"};
   _samples["expression-compare-double-gte-2"] = {generate_binary_operator_sample("double", "boolean", ">="), "4 4", "true"};
   _samples["expression-compare-double-gte-3"] = {generate_binary_operator_sample("double", "boolean", ">="), "5 4", "true"};

   _samples["expression-ternary-1"] = {sample_ternary_expression, "true", "1"};
   _samples["expression-ternary-2"] = {sample_ternary_expression, "false", "2"};

   _samples["expression-cast-byte-to-byte"] = {generate_cast_sample("byte", "byte"), "1", "1"};
   _samples["expression-cast-byte-to-short"] = {generate_cast_sample("byte", "short"), "1", "1"};
   _samples["expression-cast-byte-to-integer"] = {generate_cast_sample("byte", "integer"), "1", "1"};
   _samples["expression-cast-byte-to-long"] = {generate_cast_sample("byte", "long"), "1", "1"};
   _samples["expression-cast-byte-to-float"] = {generate_cast_sample("byte", "float"), "1", "1.000000"};
   _samples["expression-cast-byte-to-double"] = {generate_cast_sample("byte", "double"), "1", "1.000000"};
   _samples["expression-cast-byte-to-boolean-1"] = {generate_cast_sample("byte", "boolean"), "1", "true"};
   _samples["expression-cast-byte-to-boolean-2"] = {generate_cast_sample("byte", "boolean"), "0", "false"};
   _samples["expression-cast-byte-to-char"] = {generate_cast_sample("byte", "char"), "65", "A"};
   _samples["expression-cast-short-to-byte"] = {generate_cast_sample("short", "byte"), "1", "1"};
   _samples["expression-cast-short-to-short"] = {generate_cast_sample("short", "short"), "1", "1"};
   _samples["expression-cast-short-to-integer"] = {generate_cast_sample("short", "integer"), "1", "1"};
   _samples["expression-cast-short-to-long"] = {generate_cast_sample("short", "long"), "1", "1"};
   _samples["expression-cast-short-to-float"] = {generate_cast_sample("short", "float"), "1", "1.000000"};
   _samples["expression-cast-short-to-double"] = {generate_cast_sample("short", "double"), "1", "1.000000"};
   _samples["expression-cast-short-to-boolean-1"] = {generate_cast_sample("short", "boolean"), "1", "true"};
   _samples["expression-cast-short-to-boolean-2"] = {generate_cast_sample("short", "boolean"), "0", "false"};
   _samples["expression-cast-short-to-char"] = {generate_cast_sample("short", "char"), "65", "A"};
   _samples["expression-cast-integer-to-byte"] = {generate_cast_sample("integer", "byte"), "1", "1"};
   _samples["expression-cast-integer-to-short"] = {generate_cast_sample("integer", "short"), "1", "1"};
   _samples["expression-cast-integer-to-integer"] = {generate_cast_sample("integer", "integer"), "1", "1"};
   _samples["expression-cast-integer-to-long"] = {generate_cast_sample("integer", "long"), "1", "1"};
   _samples["expression-cast-integer-to-float"] = {generate_cast_sample("integer", "float"), "1", "1.000000"};
   _samples["expression-cast-integer-to-double"] = {generate_cast_sample("integer", "double"), "1", "1.000000"};
   _samples["expression-cast-integer-to-boolean-1"] = {generate_cast_sample("integer", "boolean"), "1", "true"};
   _samples["expression-cast-integer-to-boolean-2"] = {generate_cast_sample("integer", "boolean"), "0", "false"};
   _samples["expression-cast-integer-to-char"] = {generate_cast_sample("integer", "char"), "65", "A"};
   _samples["expression-cast-long-to-byte"] = {generate_cast_sample("long", "byte"), "1", "1"};
   _samples["expression-cast-long-to-short"] = {generate_cast_sample("long", "short"), "1", "1"};
   _samples["expression-cast-long-to-integer"] = {generate_cast_sample("long", "integer"), "1", "1"};
   _samples["expression-cast-long-to-long"] = {generate_cast_sample("long", "long"), "1", "1"};
   _samples["expression-cast-long-to-float"] = {generate_cast_sample("long", "float"), "1", "1.000000"};
   _samples["expression-cast-long-to-double"] = {generate_cast_sample("long", "double"), "1", "1.000000"};
   _samples["expression-cast-float-to-byte"] = {generate_cast_sample("float", "byte"), "1.00", "1"};
   _samples["expression-cast-float-to-short"] = {generate_cast_sample("float", "short"), "1.00", "1"};
   _samples["expression-cast-float-to-integer"] = {generate_cast_sample("float", "integer"), "1.00", "1"};
   _samples["expression-cast-float-to-long"] = {generate_cast_sample("float", "long"), "1.00", "1"};
   _samples["expression-cast-float-to-double"] = {generate_cast_sample("float", "double"), "1.00", "1.000000"};
   _samples["expression-cast-double-to-byte"] = {generate_cast_sample("double", "byte"), "1.00", "1"};
   _samples["expression-cast-double-to-short"] = {generate_cast_sample("double", "short"), "1.00", "1"};
   _samples["expression-cast-double-to-integer"] = {generate_cast_sample("double", "integer"), "1.00", "1"};
   _samples["expression-cast-double-to-long"] = {generate_cast_sample("double", "long"), "1.00", "1"};
   _samples["expression-cast-double-to-float"] = {generate_cast_sample("double", "float"), "1.00", "1.000000"};
   _samples["expression-cast-char-to-byte"] = {generate_cast_sample("char", "byte"), "A", "65"};
   _samples["expression-cast-char-to-short"] = {generate_cast_sample("char", "short"), "A", "65"};
   _samples["expression-cast-char-to-integer"] = {generate_cast_sample("char", "integer"), "A", "65"};
   _samples["expression-cast-boolean-to-byte-1"] = {generate_cast_sample("boolean", "byte"), "true", "1"};
   _samples["expression-cast-boolean-to-byte-2"] = {generate_cast_sample("boolean", "byte"), "false", "0"};
   _samples["expression-cast-boolean-to-short-1"] = {generate_cast_sample("boolean", "short"), "true", "1"};
   _samples["expression-cast-boolean-to-short-2"] = {generate_cast_sample("boolean", "short"), "false", "0"};
   _samples["expression-cast-boolean-to-integer-1"] = {generate_cast_sample("boolean", "integer"), "true", "1"};
   _samples["expression-cast-boolean-to-integer-2"] = {generate_cast_sample("boolean", "integer"), "false", "0"};

   _samples["assignemt-array-byte"] = {generate_array_assignemt("byte", 10), "", "9"};
   _samples["assignemt-array-short"] = {generate_array_assignemt("short", 10), "", "9"};
   _samples["assignemt-array-integer"] = {generate_array_assignemt("integer", 10), "", "9"};
   _samples["assignemt-array-long"] = {generate_array_assignemt("long", 10), "", "9"};
   _samples["assignemt-array-float"] = {generate_array_assignemt("float", 10), "", "9.000000"};
   _samples["assignemt-array-double"] = {generate_array_assignemt("double", 10), "", "9.000000"};
   _samples["assignemt-array-char"] = {generate_array_assignemt("char", 66), "", "A"};
   _samples["assignemt-array-boolean"] = {generate_array_assignemt("boolean", 10), "", "true"};

   _samples["assignemt-array-unbound-byte"] = {generate_array_unbound_assignemt("byte", 10), "", "9"};
   _samples["assignemt-array-unbound-short"] = {generate_array_unbound_assignemt("short", 10), "", "9"};
   _samples["assignemt-array-unbound-integer"] = {generate_array_unbound_assignemt("integer", 10), "", "9"};
   _samples["assignemt-array-unbound-long"] = {generate_array_unbound_assignemt("long", 10), "", "9"};
   _samples["assignemt-array-unbound-float"] = {generate_array_unbound_assignemt("float", 10), "", "9.000000"};
   _samples["assignemt-array-unbound-double"] = {generate_array_unbound_assignemt("double", 10), "", "9.000000"};
   _samples["assignemt-array-unbound-char"] = {generate_array_unbound_assignemt("char", 66), "", "A"};
   _samples["assignemt-array-unbound-boolean"] = {generate_array_unbound_assignemt("boolean", 10), "", "true"};

   _samples["expression-array-byte"] = {generate_array_load_and_store("byte", 10), "", "45"};
   _samples["expression-array-short"] = {generate_array_load_and_store("short", 10), "", "45"};
   _samples["expression-array-integer"] = {generate_array_load_and_store("integer", 10), "", "45"};
   _samples["expression-array-long"] = {generate_array_load_and_store("long", 10), "", "45"};
   _samples["expression-array-float"] = {generate_array_load_and_store("float", 10), "", "45.000000"};
   _samples["expression-array-double"] = {generate_array_load_and_store("double", 10), "", "45.000000"};
   _samples["expression-array-char"] = {generate_array_load_and_store_non_numeric("char", 66), "", "A"};
   _samples["expression-array-boolean"] = {generate_array_load_and_store_non_numeric("boolean", 10), "", "true"};

   _samples["expression-array-allocate-byte-1"] = {generate_sample_allocate_array_with_math("byte"), "", "45"};
   _samples["expression-array-allocate-byte-2"] = {generate_sample_allocate_array("byte", "1"), "", "1"};
   _samples["expression-array-allocate-short-1"] = {generate_sample_allocate_array_with_math("short"), "", "45"};
   _samples["expression-array-allocate-short-2"] = {generate_sample_allocate_array("short", "1"), "", "1"};
   _samples["expression-array-allocate-integer-1"] = {generate_sample_allocate_array_with_math("integer"), "", "45"};
   _samples["expression-array-allocate-integer-2"] = {generate_sample_allocate_array("integer", "1"), "", "1"};
   _samples["expression-array-allocate-long-1"] = {generate_sample_allocate_array_with_math("long"), "", "45"};
   _samples["expression-array-allocate-long-2"] = {generate_sample_allocate_array("long", "1"), "", "1"};
   _samples["expression-array-allocate-boolean"] = {generate_sample_allocate_array("boolean", "true"), "", "true"};
   _samples["expression-array-allocate-char"] = {generate_sample_allocate_array("char", "'X'"), "", "X"};
   _samples["expression-array-allocate-float-1"] = {generate_sample_allocate_array_with_math("float"), "", "45.000000"};
   _samples["expression-array-allocate-float-2"] = {generate_sample_allocate_array("float", "1.0f"), "", "1.000000"};
   _samples["expression-array-allocate-double-1"] = {generate_sample_allocate_array_with_math("double"), "", "45.000000"};
   _samples["expression-array-allocate-double-2"] = {generate_sample_allocate_array("double", "1.0"), "", "1.000000"};

   _samples["expression-bidimensional-array-byte-1"] = {generate_two_dimension_array_load_and_store("byte", 5, 6), "", "20"};
   _samples["expression-bidimensional-array-byte-2"] = {generate_two_dimension_array_load_and_store("byte", 6, 5), "", "20"};
   _samples["expression-bidimensional-array-short-1"] = {generate_two_dimension_array_load_and_store("short", 5, 6), "", "20"};
   _samples["expression-bidimensional-array-short-2"] = {generate_two_dimension_array_load_and_store("short", 6, 5), "", "20"};
   _samples["expression-bidimensional-array-integer-1"] = {generate_two_dimension_array_load_and_store("integer", 5, 6), "", "20"};
   _samples["expression-bidimensional-array-integer-2"] = {generate_two_dimension_array_load_and_store("integer", 6, 5), "", "20"};
   _samples["expression-bidimensional-array-long-1"] = {generate_two_dimension_array_load_and_store("long", 5, 6), "", "20"};
   _samples["expression-bidimensional-array-long-2"] = {generate_two_dimension_array_load_and_store("long", 6, 5), "", "20"};
   _samples["expression-bidimensional-array-float-1"] = {generate_two_dimension_array_load_and_store("float", 5, 6), "", "20.000000"};
   _samples["expression-bidimensional-array-float-2"] = {generate_two_dimension_array_load_and_store("float", 6, 5), "", "20.000000"};
   _samples["expression-bidimensional-array-double-1"] = {generate_two_dimension_array_load_and_store("double", 5, 6), "", "20.000000"};
   _samples["expression-bidimensional-array-double-2"] = {generate_two_dimension_array_load_and_store("double", 6, 5), "", "20.000000"};
   _samples["expression-bidimensional-array-char-1"] = {generate_two_dimension_array_load_and_store("char", 14, 6), "", "A"};
   _samples["expression-bidimensional-array-char-2"] = {generate_two_dimension_array_load_and_store("char", 6, 14), "", "A"};
   _samples["expression-bidimensional-array-boolean-1"] = {generate_two_dimension_array_load_and_store("boolean", 3, 4), "", "true"};
   _samples["expression-bidimensional-array-boolean-2"] = {generate_two_dimension_array_load_and_store("boolean", 4, 3), "", "true"};

   _samples["expression-tridimensional-array-byte-1"] = {generate_three_dimension_array_load_and_store("byte", 5, 3, 4), "", "24"};
   _samples["expression-tridimensional-array-byte-2"] = {generate_three_dimension_array_load_and_store("byte", 5, 4, 3), "", "24"};
   _samples["expression-tridimensional-array-byte-3"] = {generate_three_dimension_array_load_and_store("byte", 3, 5, 4), "", "24"};
   _samples["expression-tridimensional-array-byte-4"] = {generate_three_dimension_array_load_and_store("byte", 3, 4, 5), "", "24"};
   _samples["expression-tridimensional-array-byte-5"] = {generate_three_dimension_array_load_and_store("byte", 4, 5, 3), "", "24"};
   _samples["expression-tridimensional-array-byte-6"] = {generate_three_dimension_array_load_and_store("byte", 4, 3, 5), "", "24"};
   _samples["expression-tridimensional-array-short-1"] = {generate_three_dimension_array_load_and_store("short", 5, 3, 4), "", "24"};
   _samples["expression-tridimensional-array-short-2"] = {generate_three_dimension_array_load_and_store("short", 5, 4, 3), "", "24"};
   _samples["expression-tridimensional-array-short-3"] = {generate_three_dimension_array_load_and_store("short", 3, 5, 4), "", "24"};
   _samples["expression-tridimensional-array-short-4"] = {generate_three_dimension_array_load_and_store("short", 3, 4, 5), "", "24"};
   _samples["expression-tridimensional-array-short-5"] = {generate_three_dimension_array_load_and_store("short", 4, 5, 3), "", "24"};
   _samples["expression-tridimensional-array-short-6"] = {generate_three_dimension_array_load_and_store("short", 4, 3, 5), "", "24"};
   _samples["expression-tridimensional-array-integer-1"] = {generate_three_dimension_array_load_and_store("integer", 5, 3, 4), "", "24"};
   _samples["expression-tridimensional-array-integer-2"] = {generate_three_dimension_array_load_and_store("integer", 5, 4, 3), "", "24"};
   _samples["expression-tridimensional-array-integer-3"] = {generate_three_dimension_array_load_and_store("integer", 3, 5, 4), "", "24"};
   _samples["expression-tridimensional-array-integer-4"] = {generate_three_dimension_array_load_and_store("integer", 3, 4, 5), "", "24"};
   _samples["expression-tridimensional-array-integer-5"] = {generate_three_dimension_array_load_and_store("integer", 4, 5, 3), "", "24"};
   _samples["expression-tridimensional-array-integer-6"] = {generate_three_dimension_array_load_and_store("integer", 4, 3, 5), "", "24"};
   _samples["expression-tridimensional-array-long-1"] = {generate_three_dimension_array_load_and_store("long", 5, 3, 4), "", "24"};
   _samples["expression-tridimensional-array-long-2"] = {generate_three_dimension_array_load_and_store("long", 5, 4, 3), "", "24"};
   _samples["expression-tridimensional-array-long-3"] = {generate_three_dimension_array_load_and_store("long", 3, 5, 4), "", "24"};
   _samples["expression-tridimensional-array-long-4"] = {generate_three_dimension_array_load_and_store("long", 3, 4, 5), "", "24"};
   _samples["expression-tridimensional-array-long-5"] = {generate_three_dimension_array_load_and_store("long", 4, 5, 3), "", "24"};
   _samples["expression-tridimensional-array-long-6"] = {generate_three_dimension_array_load_and_store("long", 4, 3, 5), "", "24"};
   _samples["expression-tridimensional-array-float-1"] = {generate_three_dimension_array_load_and_store("float", 5, 3, 4), "", "24.000000"};
   _samples["expression-tridimensional-array-float-2"] = {generate_three_dimension_array_load_and_store("float", 5, 4, 3), "", "24.000000"};
   _samples["expression-tridimensional-array-float-3"] = {generate_three_dimension_array_load_and_store("float", 3, 5, 4), "", "24.000000"};
   _samples["expression-tridimensional-array-float-4"] = {generate_three_dimension_array_load_and_store("float", 3, 4, 5), "", "24.000000"};
   _samples["expression-tridimensional-array-float-5"] = {generate_three_dimension_array_load_and_store("float", 4, 5, 3), "", "24.000000"};
   _samples["expression-tridimensional-array-float-6"] = {generate_three_dimension_array_load_and_store("float", 4, 3, 5), "", "24.000000"};
   _samples["expression-tridimensional-array-double-1"] = {generate_three_dimension_array_load_and_store("double", 5, 3, 4), "", "24.000000"};
   _samples["expression-tridimensional-array-double-2"] = {generate_three_dimension_array_load_and_store("double", 5, 4, 3), "", "24.000000"};
   _samples["expression-tridimensional-array-double-3"] = {generate_three_dimension_array_load_and_store("double", 3, 5, 4), "", "24.000000"};
   _samples["expression-tridimensional-array-double-4"] = {generate_three_dimension_array_load_and_store("double", 3, 4, 5), "", "24.000000"};
   _samples["expression-tridimensional-array-double-5"] = {generate_three_dimension_array_load_and_store("double", 4, 5, 3), "", "24.000000"};
   _samples["expression-tridimensional-array-double-6"] = {generate_three_dimension_array_load_and_store("double", 4, 3, 5), "", "24.000000"};
   _samples["expression-tridimensional-array-char-1"] = {generate_three_dimension_array_load_and_store("char", 5, 3, 4), "", "\\u18"};
   _samples["expression-tridimensional-array-char-2"] = {generate_three_dimension_array_load_and_store("char", 5, 4, 3), "", "\\u18"};
   _samples["expression-tridimensional-array-char-3"] = {generate_three_dimension_array_load_and_store("char", 3, 5, 4), "", "\\u18"};
   _samples["expression-tridimensional-array-char-4"] = {generate_three_dimension_array_load_and_store("char", 3, 4, 5), "", "\\u18"};
   _samples["expression-tridimensional-array-char-5"] = {generate_three_dimension_array_load_and_store("char", 4, 5, 3), "", "\\u18"};
   _samples["expression-tridimensional-array-char-6"] = {generate_three_dimension_array_load_and_store("char", 4, 3, 5), "", "\\u18"};
   _samples["expression-tridimensional-array-boolean-1"] = {generate_three_dimension_array_load_and_store("boolean", 5, 3, 4), "", "true"};
   _samples["expression-tridimensional-array-boolean-2"] = {generate_three_dimension_array_load_and_store("boolean", 5, 4, 3), "", "true"};
   _samples["expression-tridimensional-array-boolean-3"] = {generate_three_dimension_array_load_and_store("boolean", 3, 5, 4), "", "true"};
   _samples["expression-tridimensional-array-boolean-4"] = {generate_three_dimension_array_load_and_store("boolean", 3, 4, 5), "", "true"};
   _samples["expression-tridimensional-array-boolean-5"] = {generate_three_dimension_array_load_and_store("boolean", 4, 5, 3), "", "true"};
   _samples["expression-tridimensional-array-boolean-6"] = {generate_three_dimension_array_load_and_store("boolean", 4, 3, 5), "", "true"};

   _samples["expression-bidimensional-array-allocate-byte"] = {generate_sample_allocate_bidimensional_array("byte", "77"), "", "77"};
   _samples["expression-bidimensional-array-allocate-short"] = {generate_sample_allocate_bidimensional_array("short", "77"), "", "77"};
   _samples["expression-bidimensional-array-allocate-integer"] = {generate_sample_allocate_bidimensional_array("integer", "77"), "", "77"};
   _samples["expression-bidimensional-array-allocate-long"] = {generate_sample_allocate_bidimensional_array("long", "77"), "", "77"};
   _samples["expression-bidimensional-array-allocate-float"] = {generate_sample_allocate_bidimensional_array("float", "77"), "", "77.000000"};
   _samples["expression-bidimensional-array-allocate-double"] = {generate_sample_allocate_bidimensional_array("double", "77"), "", "77.000000"};
   _samples["expression-bidimensional-array-allocate-boolean"] = {generate_sample_allocate_bidimensional_array("boolean", "true"), "", "true"};
   _samples["expression-bidimensional-array-allocate-char"] = {generate_sample_allocate_bidimensional_array("char", "'X'"), "", "X"};

   _samples["expression-function-call-duplicate-byte"] = {generate_function_expression_sample("byte", "b"), "1", "2"};
   _samples["expression-function-call-duplicate-short"] = {generate_function_expression_sample("short", "s"), "1", "2"};
   _samples["expression-function-call-duplicate-integer"] = {generate_function_expression_sample("integer"), "1", "2"};
   _samples["expression-function-call-duplicate-long"] = {generate_function_expression_sample("long"), "1", "2"};
   _samples["expression-function-call-duplicate-double"] = {generate_function_expression_sample("double"), "1", "2.000000"};
   _samples["expression-function-call-duplicate-float"] = {generate_function_expression_sample("float"), "1", "2.000000"};
   _samples["expression-function-call-sum-byte"] = {generate_function_expression_2_sample("byte", "+"), "1 2", "3"};
   _samples["expression-function-call-subtract-byte"] = {generate_function_expression_2_sample("byte", "-"), "1 2", "-1"};
   _samples["expression-function-call-multuply-byte"] = {generate_function_expression_2_sample("byte", "*"), "1 2", "2"};
   _samples["expression-function-call-divide-byte"] = {generate_function_expression_2_sample("byte", "/"), "1 2", "0"};
   _samples["expression-function-call-remainder-byte"] = {generate_function_expression_2_sample("byte", "%"), "1 2", "1"};
   _samples["expression-function-call-sum-short"] = {generate_function_expression_2_sample("short", "+"), "1 2", "3"};
   _samples["expression-function-call-subtract-short"] = {generate_function_expression_2_sample("short", "-"), "1 2", "-1"};
   _samples["expression-function-call-multuply-short"] = {generate_function_expression_2_sample("short", "*"), "1 2", "2"};
   _samples["expression-function-call-divide-short"] = {generate_function_expression_2_sample("short", "/"), "1 2", "0"};
   _samples["expression-function-call-remainder-short"] = {generate_function_expression_2_sample("short", "%"), "1 2", "1"};
   _samples["expression-function-call-sum-integer"] = {generate_function_expression_2_sample("integer", "+"), "1 2", "3"};
   _samples["expression-function-call-subtract-integer"] = {generate_function_expression_2_sample("integer", "-"), "1 2", "-1"};
   _samples["expression-function-call-multuply-integer"] = {generate_function_expression_2_sample("integer", "*"), "1 2", "2"};
   _samples["expression-function-call-divide-integer"] = {generate_function_expression_2_sample("integer", "/"), "1 2", "0"};
   _samples["expression-function-call-remainder-integer"] = {generate_function_expression_2_sample("integer", "%"), "1 2", "1"};
   _samples["expression-function-call-sum-long"] = {generate_function_expression_2_sample("long", "+"), "1 2", "3"};
   _samples["expression-function-call-subtract-long"] = {generate_function_expression_2_sample("long", "-"), "1 2", "-1"};
   _samples["expression-function-call-multuply-long"] = {generate_function_expression_2_sample("long", "*"), "1 2", "2"};
   _samples["expression-function-call-divide-long"] = {generate_function_expression_2_sample("long", "/"), "1 2", "0"};
   _samples["expression-function-call-remainder-long"] = {generate_function_expression_2_sample("long", "%"), "1 2", "1"};
   _samples["expression-function-call-from-library-1"] = {sample_call_from_library, "2", "true"};
   _samples["expression-function-call-from-library-2"] = {sample_call_from_library, "3", "false"};
   _samples["expression-function-call-from-library-arrays-swap-1"] = {sample_call_from_library_arrays, "0", "1"};
   _samples["expression-function-call-from-library-arrays-swap-2"] = {sample_call_from_library_arrays, "1", "0"};
   _samples["expression-function-call-native-sqrt-byte"] = {generate_native_single_parameter_call("byte", "math", "sqrt"), "4", "2"};

   _samples["expression-function-call-array-byte-unbound"] = {generate_array_passing_unbound("byte", 10), "", "45"};
   _samples["expression-function-call-array-short-unbound"] = {generate_array_passing_unbound("short", 101), "", "5050"};
   _samples["expression-function-call-array-integer-unbound"] = {generate_array_passing_unbound("integer", 101), "", "5050"};
   _samples["expression-function-call-array-long-unbound"] = {generate_array_passing_unbound("long", 101), "", "5050"};
   _samples["expression-function-call-array-float-unbound"] = {generate_array_passing_unbound("float", 10), "", "45.000000"};
   _samples["expression-function-call-array-double-unbound"] = {generate_array_passing_unbound("double", 10), "", "45.000000"};
   _samples["expression-function-call-array-byte-bound"] = {generate_array_passing_bound("byte", 10), "", "45"};
   _samples["expression-function-call-array-short-bound"] = {generate_array_passing_bound("short", 10), "", "45"};
   _samples["expression-function-call-array-integer-bound"] = {generate_array_passing_bound("integer", 10), "", "45"};
   _samples["expression-function-call-array-long-bound"] = {generate_array_passing_bound("long", 10), "", "45"};
   _samples["expression-function-call-array-float-bound"] = {generate_array_passing_bound("float", 10), "", "45.000000"};
   _samples["expression-function-call-array-double-bound"] = {generate_array_passing_bound("double", 10), "", "45.000000"};
   _samples["expression-function-call-array-byte-byref"] = {generate_array_passed_by_reference("byte", 10), "", "9"};
   _samples["expression-function-call-array-short-byref"] = {generate_array_passed_by_reference("short", 10), "", "9"};
   _samples["expression-function-call-array-integer-byref"] = {generate_array_passed_by_reference("integer", 10), "", "9"};
   _samples["expression-function-call-array-long-byref"] = {generate_array_passed_by_reference("long", 10), "", "9"};
   _samples["expression-function-call-array-float-byref"] = {generate_array_passed_by_reference("float", 10), "", "9.000000"};
   _samples["expression-function-call-array-double-byref"] = {generate_array_passed_by_reference("double", 10), "", "9.000000"};
   _samples["expression-function-call-array-char-byref"] = {generate_array_passed_by_reference("char", 66), "", "A"};
   _samples["expression-function-call-array-boolean-byref"] = {generate_array_passed_by_reference("boolean", 10), "", "true"};
   _samples["expression-function-call-array-byte-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("byte"), "10", "45"};
   _samples["expression-function-call-array-short-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("short"), "10", "45"};
   _samples["expression-function-call-array-integer-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("integer"), "10", "45"};
   _samples["expression-function-call-array-long-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("long"), "10", "45"};
   _samples["expression-function-call-array-float-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("float"), "10", "45.000000"};
   _samples["expression-function-call-array-double-allocated"] = {generate_sample_allocate_array_and_pass_as_parameter("double"), "10", "45.000000"};

   _samples["expression-function-call-bidimensional-array-byte-unbound"] = {generate_two_dimension_array_unbound_passing("byte", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-short-unbound"] = {generate_two_dimension_array_unbound_passing("short", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-integer-unbound"] = {generate_two_dimension_array_unbound_passing("integer", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-long-unbound"] = {generate_two_dimension_array_unbound_passing("long", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-float-unbound"] = {generate_two_dimension_array_unbound_passing("float", 5,7), "", "24.000000"};
   _samples["expression-function-call-bidimensional-array-double-unbound"] = {generate_two_dimension_array_unbound_passing("double", 5,7), "", "24.000000"};
   _samples["expression-function-call-bidimensional-array-byte-bound"] = {generate_two_dimension_array_bound_passing("byte", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-short-bound"] = {generate_two_dimension_array_bound_passing("short", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-integer-bound"] = {generate_two_dimension_array_bound_passing("integer", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-long-bound"] = {generate_two_dimension_array_bound_passing("long", 5,7), "", "24"};
   _samples["expression-function-call-bidimensional-array-float-bound"] = {generate_two_dimension_array_bound_passing("float", 5,7), "", "24.000000"};
   _samples["expression-function-call-bidimensional-array-double-bound"] = {generate_two_dimension_array_bound_passing("double", 5,7), "", "24.000000"};

   _samples["literal-byte-1"] = {generate_literal_assignment_sample("byte", "11b"), "", "11"};
   _samples["literal-byte-2"] = {generate_literal_assignment_sample("byte", "11B"), "", "11"};
   _samples["literal-short-1"] = {generate_literal_assignment_sample("short", "11s"), "", "11"};
   _samples["literal-short-2"] = {generate_literal_assignment_sample("short", "11S"), "", "11"};
   _samples["literal-integer-binary"] = {generate_literal_assignment_sample("integer", "0b11"), "", "3"};
   _samples["literal-integer-octal"] = {generate_literal_assignment_sample("integer", "0o77"), "", "63"};
   _samples["literal-integer-hexadecimal"] = {generate_literal_assignment_sample("integer", "0xFF"), "", "255"};
   _samples["literal-integer-decimal"] = {generate_literal_assignment_sample("integer", "10"), "", "10"};
   _samples["literal-long-1"] = {generate_literal_assignment_sample("long", "11l"), "", "11"};
   _samples["literal-long-2"] = {generate_literal_assignment_sample("long", "11L"), "", "11"};
   _samples["literal-char-1"] = {generate_literal_assignment_sample("char", "'A'"), "", "A"};
   _samples["literal-char-2"] = {generate_literal_assignment_sample("char", "'\\n'"), "", "\\u0a"};
   _samples["literal-char-3"] = {generate_literal_assignment_sample("char", "'\\''"), "", "'"};
   _samples["literal-char-4"] = {generate_literal_assignment_sample("char", "'\\u41'"), "", "A"};
   _samples["literal-boolean-true"] = {generate_literal_assignment_sample("boolean", "true"), "", "true"};
   _samples["literal-boolean-false"] = {generate_literal_assignment_sample("boolean", "false"), "", "false"};
   _samples["literal-double-1"] = {generate_literal_assignment_sample("double", "1.5"), "", "1.500000"};
   _samples["literal-double-2"] = {generate_literal_assignment_sample("double", "1.5d"), "", "1.500000"};
   _samples["literal-double-2"] = {generate_literal_assignment_sample("double", "1.5D"), "", "1.500000"};
   _samples["literal-float"] = {generate_literal_assignment_sample("float", "1.5f"), "", "1.500000"};
   _samples["literal-float-2"] = {generate_literal_assignment_sample("float", "1.5F"), "", "1.500000"};
   _samples["literal-string-1"] = {generate_literal_assignment_sample("array<char>[]", "\"Hello world\""), "", "Hello world\0"};
   _samples["literal-string-2"] = {generate_literal_assignment_sample("array<char>[]", "\"Hello\\nworld\""), "", "Hello\nworld\0"};
   _samples["literal-string-3"] = {generate_literal_assignment_sample("array<char>[]", "\"Hello\\\"world\""), "", "Hello\"world\0"};

   _samples["statement-empty"] = {sample_statement_empty, "", "0"};
   _samples["statement-if-then-true"] = {sample_statement_if_then, "true", "2"};
   _samples["statement-if-then-false"] = {sample_statement_if_then, "false", "1"};
   _samples["statement-if-then-else-true"] = {sample_statement_if_then_else, "true", "2"};
   _samples["statement-if-then-else-false"] = {sample_statement_if_then_else, "false", "3"};
   _samples["statement-for-1"] = {sample_statement_for, "3", "6"};
   _samples["statement-for-2"] = {sample_statement_for, "100", "5050"};
   _samples["statement-for-step-1"] = {sample_statement_for_step, "3 1", "6"};
   _samples["statement-for-step-2"] = {sample_statement_for_step, "3 2", "4"};
   _samples["statement-while-1"] = {sample_statement_while, "3", "6"};
   _samples["statement-while-2"] = {sample_statement_while, "100", "5050"};
   _samples["statement-while-3"] = {sample_statement_while, "0", "0"};
   _samples["statement-do-while-1"] = {sample_statement_do_while, "3", "6"};
   _samples["statement-do-while-2"] = {sample_statement_do_while, "100", "5050"};
   _samples["statement-do-while-3"] = {sample_statement_do_while, "0", "1"};
   _samples["statement-repeat-until-1"] = {sample_statement_repeat_until, "3", "6"};
   _samples["statement-repeat-until-2"] = {sample_statement_repeat_until, "100", "5050"};
   _samples["statement-repeat-until-3"] = {sample_statement_repeat_until, "0", "1"};
   _samples["statement-function"] = {sample_statement_function, "10", "0"};

   _samples["library-math-sqrt-byte"] = {generate_native_single_parameter_call("byte", "math", "sqrt"), "4", "2"};
   _samples["library-math-sqrt-short"] = {generate_native_single_parameter_call("short", "math", "sqrt"), "4", "2"};
   _samples["library-math-sqrt-integer"] = {generate_native_single_parameter_call("integer", "math", "sqrt"), "4", "2"};
   _samples["library-math-sqrt-long"] = {generate_native_single_parameter_call("long", "math", "sqrt"), "4", "2"};
   _samples["library-math-sqrt-float"] = {generate_native_single_parameter_call("float", "math", "sqrt"), "4", "2.000000"};
   _samples["library-math-sqrt-double"] = {generate_native_single_parameter_call("double", "math", "sqrt"), "4", "2.000000"};
   _samples["library-math-log10-byte"] = {generate_native_single_parameter_call("byte", "math", "log10"), "100", "2"};
   _samples["library-math-log10-short"] = {generate_native_single_parameter_call("short", "math", "log10"), "100", "2"};
   _samples["library-math-log10-integer"] = {generate_native_single_parameter_call("integer", "math", "log10"), "100", "2"};
   _samples["library-math-log10-long"] = {generate_native_single_parameter_call("long", "math", "log10"), "100", "2"};
   _samples["library-math-log10-float"] = {generate_native_single_parameter_call("float", "math", "log10"), "100", "2.000000"};
   _samples["library-math-log10-double"] = {generate_native_single_parameter_call("double", "math", "log10"), "100", "2.000000"};
   _samples["library-math-log2-byte"] = {generate_native_single_parameter_call("byte", "math", "log2"), "8", "3"};
   _samples["library-math-log2-short"] = {generate_native_single_parameter_call("short", "math", "log2"), "8", "3"};
   _samples["library-math-log2-integer"] = {generate_native_single_parameter_call("integer", "math", "log2"), "8", "3"};
   _samples["library-math-log2-long"] = {generate_native_single_parameter_call("long", "math", "log2"), "8", "3"};
   _samples["library-math-log2-float"] = {generate_native_single_parameter_call("float", "math", "log2"), "8", "3.000000"};
   _samples["library-math-log2-double"] = {generate_native_single_parameter_call("double", "math", "log2"), "8", "3.000000"};

   _samples["library-arrays-sqrt-float"] = {generate_native_array_call_to_module("float", "10", "sqrt"), "4", "2.000000"};
   _samples["library-arrays-sqrt-double"] = {generate_native_array_call_to_module("double", "10", "sqrt"), "4", "2.000000"};
  
   _samples["library-strings-from_string-byte"] = {genetare_string_test_from_string("str2byte","\"47\"", "byte"), "", "47"};
   _samples["library-strings-from_string-short"] = {genetare_string_test_from_string("str2short","\"47\"", "short"), "", "47"};
   _samples["library-strings-from_string-integer"] = {genetare_string_test_from_string("str2integer","\"47\"", "integer"), "", "47"};
   _samples["library-strings-from_string-long"] = {genetare_string_test_from_string("str2long","\"47\"", "long"), "", "47"};
   _samples["library-strings-from_string-float"] = {genetare_string_test_from_string("str2float","\"47\"", "float"), "", "47.000000"};
   _samples["library-strings-from_string-double"] = {genetare_string_test_from_string("str2double","\"47\"", "double"), "", "47.000000"};
   _samples["library-strings-to_string-byte"] = {genetare_string_test_to_string("47", "byte"), "", "47\0"};
   _samples["library-strings-to_string-short"] = {genetare_string_test_to_string("47", "short"), "", "47\0"};
   _samples["library-strings-to_string-integer"] = {genetare_string_test_to_string("47", "integer"), "", "47\0"};
   _samples["library-strings-to_string-long"] = {genetare_string_test_to_string("47", "long"), "", "47\0"};
   _samples["library-strings-to_string-float"] = {genetare_string_test_to_string("47", "float"), "", "47.000000\0"};
   _samples["library-strings-to_string-double"] = {genetare_string_test_to_string("47", "double"), "", "47.000000\0"};

   _samples["algo-fibonacci-1"] = {sample_algorithm_fibonacci, "0", "1"};
   _samples["algo-fibonacci-2"] = {sample_algorithm_fibonacci, "15", "987"};
   _samples["algo-isprime-1"] = {sample_algorithm_isprime, "15", "false"};
   _samples["algo-isprime-2"] = {sample_algorithm_isprime, "13", "true"};
   _samples["algo-isprime-3"] = {sample_algorithm_isprime, "99", "false"};
   _samples["algo-isprime-4"] = {sample_algorithm_isprime, "101", "true"};
}

std::string gasp_module_blaise_sample::name() const
{
   return "blaise-sample";
}

std::string gasp_module_blaise_sample::description() const
{
   return "Output the desired blaise sample.";
}

const std::string p_help("--help");
const std::string p_help_short("-h");
const std::string p_list("--list");
const std::string p_list_short("-l");
const std::string p_sample_input("--sample-input");
const std::string p_sample_input_short("-si");
const std::string p_sample_output("--sample-output");
const std::string p_sample_output_short("-so");

void gasp_module_blaise_sample::get_sample_names(std::vector<std::string> &samples) const
{
   for (auto it = _samples.begin(); it != _samples.end(); ++it)
      samples.push_back(it->first);
}

std::string gasp_module_blaise_sample::get_sample(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.sample;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}

std::string gasp_module_blaise_sample::get_input(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.input;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}
std::string gasp_module_blaise_sample::get_output(std::string sample) const
{
   auto it = _samples.find(sample);
   if (it != _samples.end())
      return it->second.output;
   throw std::runtime_error(sanelli::make_string("Cannot find sample ", sample, "."));
}

bool gasp_module_blaise_sample::run(int argc, char *argv[])
{
   if (argc < 3)
   {
      std::cerr << "Too few parameters passed. Use the option '" << p_help << "' for more details." << std::endl;
      return false;
   }

   if (argc > 4)
   {
      std::cerr << "Too many parameters passed. Use the option '" << p_help << "' for more details." << std::endl;
      return false;
   }

   std::string arg(argv[2]);
   if (arg == p_help || arg == p_help_short)
   {
      const int WIDTH = 25;
      std::ios_base::fmtflags original_flags(std::cout.flags());
      std::cout << "Command line: " << argv[0] << " " << name() << " [options]|<sample_name>" << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << std::left;
      std::cout << std::setw(WIDTH) << " " + p_help + "/" + p_help_short << std::setw(0) << "Display this help." << std::endl;
      std::cout << std::setw(WIDTH) << " " + p_list + "/" + p_list_short << std::setw(0) << "List all the available samples." << std::endl;
      std::cout << std::setw(WIDTH) << " " + p_sample_input + "/" + p_sample_input_short << std::setw(0) << "Print out sample parameters." << std::endl;
      std::cout << std::setw(WIDTH) << " " + p_sample_output + "/" + p_sample_output_short << std::setw(0) << "Print out sample result given the simple parameters." << std::endl;
      std::cout.flags(original_flags);
      return true;
   }

   if (arg == p_list || arg == p_list_short)
   {
      for (auto it = _samples.begin(); it != _samples.end(); ++it)
         std::cout << it->first << std::endl;
      return true;
   }

   if (arg == p_sample_input || arg == p_sample_input_short)
   {
      if (argc < 4)
      {
         std::cout << "Missing sample format name" << std::endl;
         return false;
      }
      arg = argv[3];
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.input << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }

   if (arg == p_sample_output || arg == p_sample_output_short)
   {
      if (argc < 4)
      {
         std::cout << "Missing sample format name" << std::endl;
         return false;
      }
      arg = argv[3];
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.output << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }

   {
      auto it = _samples.find(arg);
      if (it != _samples.end())
      {
         std::cout << it->second.sample << std::endl;
         return true;
      }
      else
      {
         std::cerr << "Unknown sample '" << arg << "'." << std::endl;
         return false;
      }
   }
}

gasp_module_blaise_sample::~gasp_module_blaise_sample()
{
   _samples.clear();
}
