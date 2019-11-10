# gasp
A set of tools inside a single executable that allowd to compile high-level languages ([`Blaise`](blaise-lang/docs/README.md), [`brainfuck`](https://gist.github.com/roachhd/dce54bec8ba55fb17d3a)) into a low lever stack-based language ([`torricelly`](torricelly-lang/docs/README.md)) that can be interpreted. High-level languages can be interpreted (and debugged) on the fly.

## Available modules
List of available modules can be found by typing `gasp` in a command line.
The available modules at present are:

| Module name | Description |
| --- | --- |
| blaise-sample | Output the desired blaise sample. |
| blaise-test | Run tests on all the the available balise tests |
| compile | Compile the input source file into the desired format. |
| debug | Debug the torricelly code. |
| dump | Convert a binary representation of torricelly binary format into a textual representation. |
| execute | Execute the input source file using an interpreter. |
| tokenize | Output the tokenized version of the input file. |

Each command support the `--help` flag.

## Low level language
The low level language used is called [`torricelly`](torricelly-lang/docs/README.md) in honour of the Italian physicist and mathematician [Evangelista Torricelli](https://en.wikipedia.org/wiki/Evangelista_Torricelli). 

## Supported high-level languages
At present two language are supported by the front-end:
- [`Blaise`](blaise-lang/docs/README.md)
- [`brainfuck`](https://gist.github.com/roachhd/dce54bec8ba55fb17d3a)

## Testing
Some projects have some tests folder with some test code but most of the testing is done by running:

```zsh
./gasp blaise-test
./gasp bf-test
```

## External libraries
I am trying to avoid the use of any external libraries. Only exception are:
- [libsanelli](https://github.com/sanelli/libsanelli)
- [Catch2](https://github.com/catchorg/Catch2)

## Future of this project
Here a (non exhaustive) list of features I would love adding to this project (not in any particular object):
- Convert torricelly code into binary executable code via LLVM project
- Add a code analyzer for `torricelly` (check dead code and read-only variable)
- Add an optimizer for `torricelly` code
- Add a profiler for `torricelly` code
- Add more libraries for `torricelly` and `Blaise` code (file, net, ...)
- Add support for `string` type in `torricelly` and `Blaise`
- Add support for user types in `torricelly` and `Blaise`
- Add the ability to convert Java bytecode into torricelly
- Port it to other platforms (Windows, Linux, ...)
