add_subdirectory(impl)

target_sources(gasp-common
    PUBLIC
        tokenizer.hpp
        string.hpp
        output.hpp
        internal_error.hpp
        gasp_error.hpp
        parser.hpp
        debug.hpp
)
