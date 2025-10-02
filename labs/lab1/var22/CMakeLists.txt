cmake_minimum_required(VERSION 3.10)
project(lab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

include_directories(include)
add_library(child_lib src/child.cpp)
add_library(parent_lib src/parent.cpp)

add_executable(child child.cpp)
target_link_libraries(child child_lib)

add_executable(main main.cpp)
target_link_libraries(main child_lib parent_lib)