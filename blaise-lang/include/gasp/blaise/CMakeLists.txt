add_subdirectory(impl)

target_sources(gasp-blaise-lang
    PUBLIC
        tokenizer.hpp
        tokens.hpp
        language.hpp
        parser.hpp
)
