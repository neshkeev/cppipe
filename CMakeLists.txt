cmake_minimum_required(VERSION 3.10)

project(cppipe VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cppipe INTERFACE)
target_link_libraries(cppipe INTERFACE include/)

enable_testing()
add_subdirectory(test)

install(FILES include/cppipe.h DESTINATION include)

include(cmake/cppipeInstall.cmake)
