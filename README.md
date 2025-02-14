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

1. Docker installed on WSL2/Linux/Windows.
2. Download the Trained Model
   from [here](https://dl.fbaipublicfiles.com/segment_anything_2/092824/sam2.1_hiera_large.pt)
3. Move the model to `Segmentation/sam2.1_model` directory. **(Important)** this step is for setting
   up the segmentation module.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/LeoEckert02/ARVoxelCarving.git
   cd ARVoxelCarving
   ```
2. Build the Docker image:
   ```bash
   docker build -t 3dsmc .
   ``` 
   Windows users can use the following command:
   ```bash
   docker build -t 3dsmc . --platform linux/amd64
   ```
   **Note:** The build process may take a while due to the large image size. \
   **Note:** Look through the docker file it might need adjustment.
3. Run the Docker container:
   ```bash
   docker-compose up
   ```
4. Open localhost in your browser to access the code editor:
    ```bash
    http://localhost:8443
    ```
5. The code editor might need password authentication. The default password is `password`.
6. Open the terminal in the code editor and make sure you are in the `project` directory:
    ```bash
    cd ARVoxelCarving
    ```
7. Run the following command to build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
8. Run the application:
   ```bash
    ./ARVoxelCarving
    ```

---

## Usage Instructions

(TO BE UPDATED)
Using the application is quite easy. As we have described in the Build Instructions, we know how to run the
application. The parameters are set in the `main.cpp` file. The application will run the following steps:

```cpp
const SegmentationParams params;
```

You can set the parameters as you want. Here is the list:

- `params.segmentationDir`: The directory where segmentation happens;
- `params.venvName `: To set the virtual environment name **DEPRECATED**;
- `params.pythonVersion`: To set the python version **DEPRECATED**;
- `params.needs_input`: Using points, this parameter is for the segmentation, so it can segment the images with greater
  accuracy;
- `params.needs_bounding_box`: Using bounding boxes to mark the object, this parameter is for the segmentation, so it
  can
  segment the images with even greater accuracy;
- `params.show_results`: If you want to visualize the output segmented images;

You can leave all of these parameters as defaults, by not changing anything. And it will run with recommended
settings.

### Overall Results

The idea is that the application will run the segmentation module, and then it will run the voxel carving module.
Here is the rundown
---

## File Structure

```
ARVoxelCarving/
│
├── resources/ # All resources (images, models, etc.)
│ ├── aruco_markers/  # ArUco marker images
│ ├── camera_calibration/ # Calibration, voxel grids, meshes
│ ├── u_calibration/ # Calibration images for the camera
│ ├── u_input/ # Input images for the object segmentation (USER INPUT)
│ ├── u_poses/ # Output files (calibration, voxel grids, meshes)
│ └── u_segmented_output/ # Output images from the segmentation module
│
├── build/ # Build directory
├── README.md # This file
├── mesh/ # Mesh module scripts including Marching Cubes implementation
├── Segmentation/ # Segmentation module
├── docker-compose.yaml # Docker configuration
├── Dockerfile # Docker build configuration
├── ....
├── main.cpp # Main application
└── CMakeLists.txt # Build configuration
```

---

## Requirements

### Libraries

- **OpenCV**: AR marker detection and segmentation.
- **Eigen**: Matrix operations.
- **Ceres**: Optional for pose optimization.
- **OpenCV**: For image processing.

### Authors

- **Leo Eckert** - [@LeoEckert02]()
- **Baran Gursoy** - [@barangursoy]()
- **Alexander Brovko** - [@XandlerQ]()
- **Giorgi Tsartsidze** - [@giokepa]()
