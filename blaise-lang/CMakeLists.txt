add_library(gasp-blaise-lang STATIC "")
set_target_properties(gasp-blaise-lang PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang gasp-common)
target_link_libraries(gasp-blaise-lang libsanelli)
target_link_libraries(gasp-blaise-lang gasp-blaise-lang-tokenizer)
target_include_directories(gasp-blaise-lang PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)