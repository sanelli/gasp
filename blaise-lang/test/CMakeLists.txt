add_subdirectory(blaise_test_sources)

target_sources(gasp-blaise-lang-test
    PRIVATE
        main.cpp
        tokenizer.cpp
        parser.cpp
)
