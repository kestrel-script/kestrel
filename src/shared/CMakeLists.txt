set(
  SHARED_SRCS 
  class.cpp
  instruction_array.cpp
  value.cpp
  module.cpp
  func.cpp
)

add_library(shared STATIC ${SHARED_SRCS})

add_executable(instruction_array_test test/instruction_array.cpp)
target_link_libraries(instruction_array_test PRIVATE shared)