cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# Project name
project(ARVoxelCarving)

# Options
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs CACHE PATH "Path to lib folder")
set(Eigen3_DIR ${LIBRARY_DIR}/Eigen/share/eigen3/cmake CACHE PATH "Path to installed Eigen")
# Set C++ flags
set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Boost REQUIRED COMPONENTS filesystem system)

# Define header and source files
set(HEADERS
        mesh/MarchingCubes.h
        mesh/Mesh.h
        mesh/MeshDataStructures.h
        mesh/MeshLoader.h
        mesh/MeshSmoother.h
        mesh/SDF.h
        mesh/PostProcInterface.h
        BoundingBox.h
        VoxelGrid.h
        sam_segmentation_generation.h
        VoxelCarveTest.h
)

set(SOURCES
        mesh/MarchingCubes.cpp
        mesh/Mesh.cpp
        mesh/MeshLoader.cpp
        mesh/MeshSmoother.cpp
        mesh/PostProcInterface.cpp
        BoundingBox.cpp
        sam_segmentation_generation.cpp
        VoxelGrid.cpp
        main.cpp
)


# Add executable
add_executable(ARVoxelCarving ${HEADERS} ${SOURCES})
# Include directories
target_include_directories(ARVoxelCarving PUBLIC ${EIGEN3_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
include_directories(${Python3_INCLUDE_DIRS})

# Link libraries
target_link_libraries(ARVoxelCarving Eigen3::Eigen ${OpenCV_LIBS})
target_link_libraries(ARVoxelCarving ${Python3_LIBRARIES})
target_link_libraries(ARVoxelCarving
        Python3::Python
        Boost::filesystem
        Boost::system
)
target_link_libraries(ARVoxelCarving Python3::Python)

# Linux specific stuff
if (UNIX AND NOT APPLE)
    set_target_properties(ARVoxelCarving PROPERTIES
            BUILD_RPATH ${Python3_LIBRARY_DIRS}
            INSTALL_RPATH ${Python3_LIBRARY_DIRS})
endif ()

# Visual Studio properties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ARVoxelCarving)
set_property(TARGET ARVoxelCarving PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)
