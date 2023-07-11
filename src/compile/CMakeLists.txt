set(
  COMPILE_SRCS 
  expression.cpp
  compiler.cpp
  
  statement.cpp
  statements/if.cpp
  statements/function.cpp
  statements/return.cpp
  statements/import.cpp
  )

add_library(compile STATIC ${COMPILE_SRCS})
target_link_libraries(compile PRIVATE shared)