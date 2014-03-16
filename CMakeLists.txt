cmake_minimum_required(VERSION 2.8)

PROJECT(transfert)
find_package(OpenCV REQUIRED ts)

set (TRANSFERT_SRC transfert.cpp)
set (TRANSFERT_INC transfert.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

add_executable(
  transfert
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  main.cpp)

target_link_libraries(transfert ${OpenCV_LIBS})


add_executable(
  tests
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  tests.cpp
  test_main.cpp)

target_link_libraries(tests ${OpenCV_LIBS})

# Preserve the old value if an exterior file already provide some.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -g")
