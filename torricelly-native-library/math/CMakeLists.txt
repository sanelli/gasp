add_library(gasp-torricelly-native-math SHARED "")
set_target_properties(gasp-torricelly-native-math PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "math"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(gasp-torricelly-native-math gasp-torricelly-interpreter)

add_custom_command(TARGET gasp-torricelly-native-math POST_BUILD COMMAND 
   ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/torricelly-native-library/math/src/math.mblaise ${CMAKE_BINARY_DIR}/gasp/library)

add_subdirectory(src)
