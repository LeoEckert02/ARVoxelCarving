cmake_minimum_required(VERSION 3.10)

project(ARVoxelCarving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

set(LIBHARU_INCLUDE_DIR "/usr/local/Cellar/libharu/2.4.4/include")
set(LIBHARU_LIBRARY "/usr/local/Cellar/libharu/2.4.4/lib/libhpdf.2.4.4.dylib")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${LIBHARU_INCLUDE_DIR})

add_executable(aruco_marker_generation aruco_marker_generation.cpp)
target_link_libraries(aruco_marker_generation ${OpenCV_LIBS})
target_link_libraries(aruco_marker_generation ${LIBHARU_LIBRARY})

add_executable(aruco_marker_detection aruco_marker_detection.cpp)
target_link_libraries(aruco_marker_detection ${OpenCV_LIBS})

add_executable(camera_calibration camera_calibration.cpp)
target_link_libraries(camera_calibration ${OpenCV_LIBS})

# specify the path to libharu library (if not automatically found)
link_directories("/usr/local/Cellar/libharu/2.4.4/lib")
