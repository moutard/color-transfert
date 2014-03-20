# find the required libraries.
# find_packages set variables that contains where to find all the elements necessary for the compilation of the lib.
find_package(OpenCV REQUIRED ts)
# Handle when library not found.

# Boost_INCLUDE_DIRS created by find_package contains where to find boost *.hpp files.
include_directories (${Boost_INCLUDE_DIRS})

include_directories (${TRANSFERT_SOURCE_DIR}/src)

add_executable(
  tests
  tests.cpp
  test_main.cpp
)

# Configuration de l'édition de lien.
target_link_libraries(tests ${OpenCV_LIBS} transfertlib)

# Preserve the old value if an exterior file already provide some.
# Do not add debug flag -g manually use CMAKE_BUILD_TYPE or -DCMAKE_BUILD_TYPE
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

