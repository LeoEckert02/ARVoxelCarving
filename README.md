# AR Marker + Voxel Carving Project

This project involves 3D object reconstruction using AR markers for camera pose estimation and voxel carving for shape
modeling. The following tutorial provides instructions on how to use the application.

---

## Table of Contents

1. [Overview](#overview)
2. [Installation](#installation)
3. [Usage Instructions](#usage-instructions)
    - [Preprocessing](#preprocessing)
    - [Voxel Carving](#voxel-carving)
    - [Postprocessing](#postprocessing)
4. [File Structure](#file-structure)
5. [Requirements](#requirements)
6. [Authors](#authors)
---

## Overview

The application was created for **TUM IN2354: 3D Scanning & Motion Capture course.**
The c++ application reconstructs a 3D object from multiple images using voxel carving using AR markers. Steps include:

- **Preprocessing**: Camera calibration, pose estimation, and foreground segmentation.
- **Voxel Carving**: Constructing a voxel grid and refining it using silhouettes.
- **Postprocessing**: Converting the voxel grid into a mesh and smoothing it for visualization.

---

## Installation

### Prerequisites

1. Install **CMake** and a C++ compiler (e.g., GCC or MSVC).
2. Ensure **OpenCV** and **Eigen** (optional for acceleration) are installed.
3. Install Python (optional) for some tasks.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/LeoEckert02/ARVoxelCarving.git
   cd ARVoxelCarving
   ```
2. Create a build directory:
   `mkdir build && cd build`

3. Build the application:
   `make`

---

## Usage Instructions

### Preprocessing

1. **Camera Calibration**:
    - **Input**: images of a checkerboard from various angles.
    - Run the calibration module:
      ./voxel_carving --calibrate [input_folder]
    - **Outputs:** intrinsic and extrinsic camera parameters.

2. **Pose Estimation**:
    - Print and place ArUco markers near the object.
    - Run the pose estimation module:
      ./voxel_carving --estimate_pose [calibration_file] [input_folder]

3. **Image Segmentation**:
    - Use green-screen or automatic segmentation tools like SAM.
    - Run the segmentation module:
      ./voxel_carving --segment [input_folder]

### Voxel Carving

1. Initialize the voxel grid:
   ./voxel_carving --initialize_voxel_grid [pose_file] [image_folder]

2. Perform carving:
   ./voxel_carving --carve [voxel_grid_file] [segmented_images]

### Postprocessing

1. Generate the mesh:
   ./voxel_carving --generate_mesh [voxel_grid_file]

2. Smooth the mesh:
   ./voxel_carving --smooth_mesh [mesh_file]

3. Export the mesh:
   ./voxel_carving --export_mesh [mesh_file] --format off

---

## File Structure

```
project/
│
├── src/ # Source code
│ ├── camera_calibration.cpp # Camera calibration
│ ├── pose_estimation.cpp # Pose estimation
│ ├── segmentation.cpp # Image segmentation
│ ├── voxel_carving.cpp # Voxel carving
│ ├── mesh_generation.cpp # Mesh processing
│ └── export.cpp # Export functionalities
│
├── resources/ # Sample data
│ ├── input/ # Input images
│ ├── output/ # Output files (calibration, voxel grids, meshes)
│ └── markers/ # AR marker PDFs
│
├── build/ # Build directory
├── README.md # This file
└── CMakeLists.txt # Build configuration
```

---

## Requirements

### Libraries

- **OpenCV**: AR marker detection and segmentation.
- **Eigen**: Matrix operations.
- **Ceres**: Optional for pose optimization.

### Authors

- **Leo Eckert** - [@LeoEckert02]()
- **Baran Gursoy** - [@barangursoy]()
- **Alexander Brovko** - [@siddharthasaha]()
- **Giorgi Tsartsidze** - [@giokepa]()