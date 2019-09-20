if(NOT TARGET Catch2::Catch2)
   add_subdirectory(catch2)
endif()

if(NOT TARGET libsanelli::libsanelli)
   add_subdirectory(libsanelli)
endif()
