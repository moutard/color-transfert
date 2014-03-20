# do not compile if cmake version lower.
cmake_minimum_required(VERSION 2.8)

# define the name of the project. here "transfert"
project(transfert)

add_subdirectory(src)
add_subdirectory(tests)
