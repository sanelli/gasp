add_library(gasp-torricelly-native-matrices SHARED "")
set_target_properties(gasp-torricelly-native-matrices PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "matrices"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-matrices gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-matrices POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/matrices/src/matrices.mblaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
