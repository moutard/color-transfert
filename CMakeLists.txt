cmake_minimum_required(VERSION 2.8)

PROJECT(transfert)

find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

set (TRANSFERT_SRC transfert.cpp)
set (TRANSFERT_INC transfert.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(
  transfert
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  main.cpp)

add_executable(
  runTests
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  transfert_tests.cpp)

target_link_libraries(transfert ${OpenCV_LIBS})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)


