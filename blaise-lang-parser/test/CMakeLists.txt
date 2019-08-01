add_subdirectory(blaise_test_sources)

target_sources(gasp-blaise-lang-parser-test
    PRIVATE
        main.cpp
        parser.cpp
)
