add_library(gasp-bf-to-torricelly STATIC "")
set_target_properties(gasp-bf-to-torricelly PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gasp-bf-to-torricelly gasp-common)
target_link_libraries(gasp-bf-to-torricelly gasp-torricelly-lang)

target_include_directories(gasp-bf-to-torricelly PUBLIC include)

add_subdirectory(include)
add_subdirectory(src)
# add_subdirectory(test)