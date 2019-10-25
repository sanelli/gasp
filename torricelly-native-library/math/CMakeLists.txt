add_library(torricelly-native-math SHARED "")
set_target_properties(torricelly-native-math PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "math"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(torricelly-native-math gasp-torricelly-interpreter)

add_subdirectory(src)