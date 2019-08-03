add_library(gasp-torricelly-lang SHARED "")
set_target_properties(gasp-torricelly-lang PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-torricelly-lang gasp-common)
target_link_libraries(gasp-torricelly-lang libsanelli)
target_include_directories(gasp-torricelly-lang PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(test)