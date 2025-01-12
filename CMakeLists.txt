cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# Project name
project(fp_meshessing)

# Options
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs CACHE PATH "Path to lib folder")
set(Eigen3_DIR ${LIBRARY_DIR}/Eigen/share/eigen3/cmake CACHE PATH "Path to installed Eigen")
# Set C++ flags
set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)

# Define header and source files
set(HEADERS
    mesh/MarchingCubes.h
    mesh/Mesh.h
    mesh/MeshDataStructures.h
    mesh/MeshLoader.h
    mesh/MeshSmoother.h
    mesh/SDF.h
    mesh/PostProcInterface.h
)

set(SOURCES
    mesh/MarchingCubes.cpp
    mesh/Mesh.cpp
    mesh/MeshLoader.cpp
    mesh/MeshSmoother.cpp
    mesh/PostProcInterface.cpp
    main.cpp
)

add_executable(fp_meshessing ${HEADERS} ${SOURCES})
target_include_directories(fp_meshessing PUBLIC ${EIGEN3_INCLUDE_DIR})
target_link_libraries(fp_meshessing Eigen3::Eigen)

# Visual Studio properties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fp_meshessing)
set_property(TARGET fp_meshessing PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)