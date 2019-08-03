add_library(gasp-common SHARED "")
set_target_properties(gasp-common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(gasp-common PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)