add_library(gasp-blaise-to-torricelly SHARED "")
set_target_properties(gasp-blaise-to-torricelly PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-blaise-to-torricelly gasp-common)
target_link_libraries(gasp-blaise-to-torricelly gasp-torricelly-lang)
target_link_libraries(gasp-blaise-to-torricelly gasp-blaise-lang)

target_include_directories(gasp-blaise-to-torricelly PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(test)