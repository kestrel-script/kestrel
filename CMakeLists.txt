cmake_minimum_required(VERSION 3.4...3.18)
project(kestrel)

set(CMAKE_CXX_FLAGS "-std=c++14 -O3")
# set(CMAKE_BUILD_TYPE Debug)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

find_package(Threads)

include_directories(include)
include_directories(src)

add_subdirectory(src/shared)
add_subdirectory(src/compile)
add_subdirectory(src/runtime)

set(
  SRCS 
  src/dumb.cpp
  )

add_library(kestrel STATIC ${SRCS})
link_libraries(kestrel compile)
link_libraries(kestrel runtime)

add_executable(test src/test/main.cpp)
link_libraries(test PRIVATE kestrel)

