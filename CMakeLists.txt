cmake_minimum_required(VERSION 3.13)
project(gasp_compiler LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#################################################
# GASP 3D party libraries
#################################################
add_subdirectory(libs)

#################################################
# GASP common
#################################################
add_library(gasp-common SHARED "")
add_subdirectory(common)
set_target_properties(gasp-common PROPERTIES LINKER_LANGUAGE CXX)

#################################################
# GASP common
#################################################
add_library(gasp-blaise-lang SHARED "")
add_subdirectory(blaise-lang)
set_target_properties(gasp-blaise-lang PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang gasp-common)

#################################################
# GASP main executable
#################################################
add_executable(gasp "")
add_subdirectory(compiler)
target_link_libraries(gasp gasp-common)
target_link_libraries(gasp gasp-blaise-lang)

