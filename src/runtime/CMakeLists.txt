set(
  RUNTIME_SRCS 
  interpreter.cpp
  runtime.cpp
  core/classes/integer.cpp
  core/classes/string.cpp

  )

add_library(runtime STATIC ${RUNTIME_SRCS})
link_libraries(runtime shared)