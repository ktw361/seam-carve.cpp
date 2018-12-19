project(carvecpp)
cmake_minimum_required(VERSION 3.1)

add_compile_options(
    "-std=c++11"
    #"-O1"
    "-g"
)

# Find OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    Message("OpenMp found")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB INC_HEADERS
    "inc/*.h"
    "inc/tqdm/*.h"
    "inc/Eigen/*"
)
file(GLOB SRC
    "*.cpp"
)
file(GLOB HEADERS
    "*.h"
)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${INC_HEADERS} ${SRC} ${HEADERS})
#add_executable(${PROJECT_NAME} main.cpp image.cpp image.h)
