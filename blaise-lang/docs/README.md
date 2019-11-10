# Blaise
Blaise is a simil-Pascal high leven languages anmed after the French mathematician, physicist, inventor and writer [Blaise Pascal](https://en.wikipedia.org/wiki/Blaise_Pasca)].

## Capabilites
This is a list of not exaustive supported features:
- Program (`program`) structure very similar to Pascal
  - A program can define input parameters and a return value;
  - A program can require external module (`module`) via the `use` directive;
  - A program and a module can define subroutines (`procedure` and `function`);
  - Program and subroutines can define variables with the `var` directive;
  - Non-array variables are passed by values while arrays are passed by reference;
- Available native types: 
  - `byte`: 8 bit signed integral type;
  - `short`: 16 bt signed integral type;
  - `integer`: 32 bit signed integral type;
  - `long`: 64 bit signed integral type;
  - `float`: 32 bit single-precision signed floating point type;
  - `double`: 64 bit duble-precision signed floating point type;
  - `char`: 8 bit unsigned character type;
- Arrays
  - Single dimension arrays (e.g. `array<integer>[10]`);
  - Unbounded arrays (e.g. `array<integer>`);
  - Array allocation (`new`) and de-allocation (`delete`);
- Math
  - All usual mathematical operator are available (`+`, `-`, `*`, `/`);
  - Reminder operator (`%`) is available for `byte`, `short`, `integer` and `long` variables
- Expressions
  - Numeric, boolean and char expressions are supported;
  - Ability to cast between different types;
  - Ability to cash between different types;
  - Ternary operator `if <expression> then <expression> else <expression>`;
  - Usual operator precedence;
- Statement
  - compund statement delimited by `begin` and `end`;
  - `if` statement;
  - `while` statement;
  - `do-while` statement;
  - `repeat-until` statement;
  - `for` statement;
- Native
  - Define `native` subroutines
  - Compilation
  - Compiling a program will compile all required modules with the `use` directive;

## Grammar
The [grammar](grammar.md) is similar to the Pascal one but it is not exactly the same.

## Available library modules
A list of available modules that can be used can be found [here](../../torricelly-native-library/docs/readme.md).

## Samples
Here some samples.
More samples can be found executing the following instruction

```zsh
./gasp blaise-sample --list
./gasp blaise-sample empty
```

Programs can be execute directly like following:

```zsh
./gasp execute -if blaise -i FILE.blaise
```

Program can be compiled and then execute like following:

```zsh
./gasp compile -if blaise -i FILE.blaise -of torricelly-binary -o FILE.tb
./gasp execute -i FILE.tb PARAM1 PARAM2 PARAM3
```

Here somes samples:

```
program sample;
begin
end.
```

```
program isprime(input: integer) : boolean;
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
end.
```

```
program fibonacci(input: integer) : integer;

function fibo(i : integer): integer;
begin
   fibo := 1;
   if i > 1 then
      fibo := fibo(i-2) + fibo(i-1);
end;

begin
   fibonacci := fibo(input);
end.
```

```
program sample(value: integer): boolean;
use math;
begin
   sample := is_even(value);
end.
```

```
program sample : short;

procedure fill(size: integer, numbers: array<short>);
var index: integer;
begin
   for index from 0 to (size-1) begin
      numbers[index] := cast<short>(index);
   end;
end;

var
   numbers: array<short>[10];
begin
   fill(10, numbers);
   sample := numbers[9];
end.
```

More samples can be found running the following command
```zsh
./gasp blaise-sample --list
```

## Debugging
Blaise code can be dubugged once converted into torricelly code.
- Run the debugger
```zsh
./gasp debug
```
- Load the program you want debug
```
> blaise-load FILE.blaise PARAM1 PARAM2
```
- Debug the code
```
> step
> locals
> stack
> ip
> code
```

More details on debugging can be found [here](../../torricelly-debugger/docs/debugger.md).

## What's next
Some features I plan to add in the (near?) future are listed in the [issues](https://github.com/sanelli/gasp/issues) page of this project:
- Support for `string` type
- Support for user defined types
- More library modules
- Support for templating/generic
- Support for multi-dimensional arrays
- `for-each` loop statement
- `switch` statement
- `switch` expression
- Wide (UTF-8) character support with a new type (`wchar` and `wstring`)
- `break` and `continue` keywords
- Bitwise operators (potentially via library functions)
- Constants

