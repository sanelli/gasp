add_library(gasp-torricelly-interpreter STATIC "")
set_target_properties(gasp-torricelly-interpreter PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-torricelly-interpreter gasp-common)
target_link_libraries(gasp-torricelly-interpreter libsanelli)
target_link_libraries(gasp-torricelly-interpreter gasp-torricelly-lang)
target_link_libraries(gasp-torricelly-interpreter gasp-torricelly-lang-io)
target_include_directories(gasp-torricelly-interpreter PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(test)