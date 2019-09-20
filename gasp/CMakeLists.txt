add_executable(gasp "")

set_target_properties(gasp PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(gasp libsanelli)
target_link_libraries(gasp gasp-common)
target_link_libraries(gasp gasp-blaise-lang)
target_link_libraries(gasp gasp-blaise-lang-tokenizer)
target_link_libraries(gasp gasp-blaise-lang-parser)
target_link_libraries(gasp gasp-torricelly-lang)
target_link_libraries(gasp gasp-torricelly-lang-io)
target_link_libraries(gasp gasp-torricelly-interpreter)
target_link_libraries(gasp gasp-blaise-to-torricelly)

target_include_directories(gasp PUBLIC include)

add_subdirectory(src)
add_subdirectory(include)
