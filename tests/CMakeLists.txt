project(ownDesLibraryTests C)

# Includes
include_directories(include ../lib/include)

# Tests executables
add_executable(des_simple_test algorithmic/des_simple_test.c utils/utils.c)

# Tests dependencies
target_link_libraries(des_simple_test ownDes)
