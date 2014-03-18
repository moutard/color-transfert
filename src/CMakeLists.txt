# do not compile if cmake version lower.
cmake_minimum_required(VERSION 2.8)

# define the name of the project. here "transfert"
project(transfert)

add_subdirectory(tests)

# find the required libraries.
find_package(OpenCV REQUIRED ts)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)


# set the TRANSFERT_SRC with the given value.
set(TRANSFERT_SRC transfert.cpp)
set(TRANSFERT_INC transfert.h)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(
  transfert
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  main.cpp)

target_link_libraries(transfert ${OpenCV_LIBS} ${Boost_LIBRARIES})

# Preserve the old value if an exterior file already provide some.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -g")
