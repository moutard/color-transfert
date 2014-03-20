# find the required libraries.
# find_packages set variables that contains where to find all the elements necessary for the compilation of the lib.
find_package(OpenCV REQUIRED ts)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
# Handle when library not found.

# set the TRANSFERT_CPP with the given value.
set(TRANSFERT_CPP transfert.cpp)
set(TRANSFERT_HPP transfert.h)

# Boost_INCLUDE_DIRS created by find_package contains where to find boost *.hpp files.
include_directories (${Boost_INCLUDE_DIRS})

# Create a temporary library that will be used by the executable and by the tests.
# So no need to compile twice and add redondant code in the CMakeLists.
add_library(
  transfertlib
  ${TRANSFERT_HPP}
  ${TRANSFERT_CPP}
)

add_executable(transfert main.cpp)

# Configuration de l'édition de lien.
target_link_libraries(transfert ${OpenCV_LIBS} ${Boost_LIBRARIES} transfertlib)

# Preserve the old value if an exterior file already provide some.
# Do not add debug flag -g manually use CMAKE_BUILD_TYPE or -DCMAKE_BUILD_TYPE
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

#set(CMAKE_BUILD_TYPE Debug)
