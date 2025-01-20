cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# Project name
project(fp_post_processing)

# Options
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs CACHE PATH "Path to lib folder")
set(Eigen3_DIR ${LIBRARY_DIR}/Eigen/share/eigen3/cmake CACHE PATH "Path to installed Eigen")
# Set C++ flags
set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)

# Define header and source files
set(HEADERS
    MeshDataStructures.h
    Mesh.h
    MeshLoader.h
    MarchingCubes.h
    MeshSmoother.h
)

set(SOURCES
    Mesh.cpp
    MeshLoader.cpp
    MarchingCubes.cpp
    MeshSmoother.cpp
)

# add_executable(fp_post_processing ${HEADERS} ${SOURCES})
# target_include_directories(fp_post_processing PUBLIC ${EIGEN3_INCLUDE_DIR})
# target_link_libraries(fp_post_processing Eigen3::Eigen)

# # Visual Studio properties
# set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fp_post_processing)
# set_property(TARGET fp_post_processing PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)