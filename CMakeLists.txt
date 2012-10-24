
cmake_minimum_required (VERSION 2.8)

project (ep3 CXX C)
include_directories (src)

set (
  EP3_SRC
  src/main.cxx
  src/ep3.cxx
  src/ep3.h
  src/network.cxx
  src/network.h
  src/router.cxx
  src/router.h
)

add_executable (ep3 ${EP3_SRC})

set_target_properties (
  ep3
  PROPERTIES COMPILE_FLAGS
  "-ansi -Wall -g -O0"
)

