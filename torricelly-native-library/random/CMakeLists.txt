add_library(gasp-torricelly-native-random SHARED "")
set_target_properties(gasp-torricelly-native-random PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "random"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-random gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-random POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/random/src/random.mblaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
