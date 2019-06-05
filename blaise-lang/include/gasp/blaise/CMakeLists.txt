add_subdirectory(impl)

target_sources(gasp-blaise-lang
    PUBLIC
        tokenizer.hpp
        tokens.hpp
        ast.hpp
        parser.hpp
)
