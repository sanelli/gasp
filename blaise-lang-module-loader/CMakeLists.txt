add_library(gasp-blaise-lang-module-loader STATIC "")
set_target_properties(gasp-blaise-lang-module-loader  PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang-module-loader  gasp-common)
target_link_libraries(gasp-blaise-lang-module-loader  libsanelli)
target_link_libraries(gasp-blaise-lang-module-loader  gasp-blaise-lang)
target_link_libraries(gasp-blaise-lang-module-loader  gasp-blaise-lang-tokenizer)
target_link_libraries(gasp-blaise-lang-module-loader  gasp-blaise-lang-parser)
target_include_directories(gasp-blaise-lang-module-loader  PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
