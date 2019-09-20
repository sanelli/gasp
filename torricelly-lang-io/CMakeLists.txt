add_library(gasp-torricelly-lang-io STATIC "")
set_target_properties(gasp-torricelly-lang-io PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-torricelly-lang-io gasp-torricelly-lang)
target_link_libraries(gasp-torricelly-lang-io gasp-common)
target_include_directories(gasp-torricelly-lang-io PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(test)