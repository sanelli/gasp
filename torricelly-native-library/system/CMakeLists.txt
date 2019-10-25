add_library(torricelly-native-system SHARED "")
set_target_properties(torricelly-native-system PROPERTIES 
   LINKER_LANGUAGE CXX
   OUTPUT_NAME "system"
   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gasp/library"
   )

target_link_libraries(torricelly-native-system gasp-torricelly-interpreter)

add_subdirectory(src)