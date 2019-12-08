add_library(gasp-torricelly-native-strings SHARED "")
set_target_properties(gasp-torricelly-native-strings PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "strings"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-strings gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-strings POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/strings/src/strings.mblaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
