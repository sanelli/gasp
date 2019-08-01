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
# blaise lang
#################################################
add_library(gasp-blaise-lang-tokenizer SHARED "")
set_target_properties(gasp-blaise-lang-tokenizer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang-tokenizer gasp-common)

add_executable(gasp-blaise-lang-tokenizer-test "")
set_target_properties(gasp-blaise-lang-tokenizer-test PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang-tokenizer-test gasp-blaise-lang-tokenizer)
target_link_libraries(gasp-blaise-lang-tokenizer-test gasp-common)
target_link_libraries(gasp-blaise-lang-tokenizer-test Catch2::Catch2)

add_subdirectory(blaise-lang-tokenizer)

add_library(gasp-blaise-lang SHARED "")
set_target_properties(gasp-blaise-lang PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang gasp-common)
target_link_libraries(gasp-blaise-lang gasp-blaise-lang-tokenizer)

add_subdirectory(blaise-lang)

add_library(gasp-blaise-lang-parser SHARED "")
set_target_properties(gasp-blaise-lang-parser PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang-parser gasp-common)
target_link_libraries(gasp-blaise-lang-parser gasp-blaise-lang)
target_link_libraries(gasp-blaise-lang-parser gasp-blaise-lang-tokenizer)

add_executable(gasp-blaise-lang-parser-test "")
set_target_properties(gasp-blaise-lang-parser-test PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-lang-parser-test gasp-blaise-lang-tokenizer)
target_link_libraries(gasp-blaise-lang-parser-test gasp-blaise-lang-parser)
target_link_libraries(gasp-blaise-lang-parser-test gasp-blaise-lang)
target_link_libraries(gasp-blaise-lang-parser-test gasp-common)
target_link_libraries(gasp-blaise-lang-parser-test Catch2::Catch2)

add_subdirectory(blaise-lang-parser)

#################################################
# torricelly lang
#################################################
add_library(gasp-torricelly-lang SHARED "")
set_target_properties(gasp-torricelly-lang PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-torricelly-lang gasp-common)

add_subdirectory(torricelly-lang)

add_library(gasp-torricelly-lang-io SHARED "")
set_target_properties(gasp-torricelly-lang-io PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-torricelly-lang-io gasp-torricelly-lang)
target_link_libraries(gasp-torricelly-lang-io gasp-common)

add_subdirectory(torricelly-lang-io)

#################################################
# blaise-to-torricelly
#################################################
add_library(gasp-blaise-to-torricelly SHARED "")
set_target_properties(gasp-blaise-to-torricelly PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-to-torricelly gasp-common)
target_link_libraries(gasp-blaise-to-torricelly gasp-torricelly-lang)
target_link_libraries(gasp-blaise-to-torricelly gasp-blaise-lang)

add_subdirectory(blaise-to-torricelly)

#################################################
# GASP main executable
#################################################
add_executable(gasp "")
add_subdirectory(compiler)
target_link_libraries(gasp gasp-common)
target_link_libraries(gasp gasp-blaise-lang)
target_link_libraries(gasp gasp-blaise-lang-tokenizer)
target_link_libraries(gasp gasp-blaise-lang-parser)
target_link_libraries(gasp gasp-torricelly-lang)
target_link_libraries(gasp gasp-torricelly-lang-io)
target_link_libraries(gasp gasp-blaise-to-torricelly)

