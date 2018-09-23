cmake_minimum_required(VERSION 3.12)
project(ownDes C)

set(CMAKE_C_STANDARD 11)

enable_testing()

# Adding test system parts
add_subdirectory(lib)
add_subdirectory(tests)

# Adding tests
add_test(NAME "DES simple algorithmic test" COMMAND des_simple_test)
