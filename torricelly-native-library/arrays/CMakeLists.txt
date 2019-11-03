add_library(gasp-torricelly-native-arrays SHARED "")
set_target_properties(gasp-torricelly-native-arrays PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "arrays"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-arrays gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-arrays POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/arrays/src/arrays.mblaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
