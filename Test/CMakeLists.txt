cmake_minimum_required(VERSION 2.6)
add_compile_options(
    "-std=c++11"
    "-g"
    )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests
    #Image_tests.cpp
    convolution_tests.cpp
    )
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
