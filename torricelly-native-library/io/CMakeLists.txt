add_library(gasp-torricelly-native-io SHARED "")
set_target_properties(gasp-torricelly-native-io PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "io"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-io gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-io POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/io/src/io.blaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
