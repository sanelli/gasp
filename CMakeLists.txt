cmake_minimum_required(VERSION 3.13)
project(gasp_compiler LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#################################################
# GASP 3D party libraries
#################################################

#################################################
# GASP common
#################################################
add_library(gaspcommon SHARED "")
add_subdirectory(common)
set_target_properties(gaspcommon PROPERTIES LINKER_LANGUAGE CXX)


#################################################
# GASP main executable
#################################################
add_executable(gasp "")
add_subdirectory(compiler)
target_link_libraries(gasp gaspcommon)

#################################################
# GASP core library
#################################################

#################################################
# GASP front-end(s)
#################################################

#################################################
# GASP intermediate language
#################################################

#################################################
# GASP back-end(s)
#################################################

#################################################
# GASP intermediate language optimizer(s)
#################################################

#################################################
# GASP intermediate language analysis
#################################################

#################################################
# GASP enviroment(s)
#################################################
