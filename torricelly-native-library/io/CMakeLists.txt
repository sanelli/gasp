add_library(torricelly-native-io SHARED "")
set_target_properties(torricelly-native-io PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "io"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(torricelly-native-io gasp-torricelly-interpreter)

add_subdirectory(src)