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
![bird_input](https://github.com/user-attachments/assets/73fb9651-3794-45ab-a179-b343ac8173af)
![bird_segmentation](https://github.com/user-attachments/assets/84f11c63-26b1-492b-b01a-2dd133952739)
- **Voxel Carving**: Constructing a voxel grid and refining it using silhouettes.
![bird_voxel_grid_h (1)](https://github.com/user-attachments/assets/3645d82d-5a6c-48d8-b2af-21a50e172d9b)
![bird_mesh (1)](https://github.com/user-attachments/assets/df206d69-f8c7-4377-b7f1-452a0f493c15)
- **Postprocessing**: Converting the voxel grid into a mesh and smoothing it for visualization.
![image](https://github.com/user-attachments/assets/b4f82fb2-1067-44a3-9413-02f75f86b1f6)
---

## Installation

### Prerequisites

1. Docker installed on WSL2/Linux/Windows (**WARNING** if using windows, please use manual segmentation
   defined in section **alternative methods**).
2. Download the Trained Model
   from [here](https://dl.fbaipublicfiles.com/segment_anything_2/092824/sam2.1_hiera_large.pt)
3. Move the model to `Segmentation/sam2.1_model` directory. **(Important)** this step is for setting
   up the segmentation module.
4. Remove files from `resources/u_segmented_output` directory. **(Important)** this step is for setting up the
   segmentation module.
5. Images you want to use for the segmentation should be placed in the `resources/u_input` directory.
6. Try to order your images, so the program can index the images and camera intrinsics/extrinsics correctly.
7. Make sure you have sufficient disk space for the Docker image.
8. If you are using **WSL** memory might create an issue when you choose high resolutions for voxel carving. You might
   need to adjust it in the Docker settings.

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
    or
   ```bash
    ./ARVoxelCarving {grid resolution size}
    ```
---

## Usage Instructions

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

### Camera Calibration Instructions 

1. Print the [checkerboard pattern](./resources/camera_calibration/Checkerboard-A4-25mm-10x7.pdf) on a sheet of paper.  
2. Take **10–20 photos** of the checkerboard from various angles. Ensure good lighting and sharp focus for the best results.  
3. Place the captured images in the following directory: `/resources/u_calibration`

**cmd Parameter** can be used to adjust the camera resolution. The default is 300.
for example `./ARVoxelCarving 300` will set the resolution to 300, and will calculate other dimensions to be cubic
voxels (You can also just use `./ARVoxelCarving` without parameters, it will use the default value).

After the voxel carving stage, the post-processing interface will appear in the console, providing step-by-step guidance. You will have the option to adjust mesh smoothing parameters, with the Inverse Gaussian smoothing kernel being the recommended choice. While optimal smoothing settings depend on the target object, the interface suggests default values for reasonable results.

### Overall Results

The idea is that the application will run the segmentation module, and then it will run the voxel carving module.
The output will be a **mesh file** in `build` folder that you can visualize in any 3D viewer.
---

### Alternative methods

The problem is that running python from C++ can have some issues, regarding all kinds of permissions/dependencies. So,
we have provided an alternative method to run the segmentation module. You can run the segmentation module separately
by following these steps:

1. run the following command:

```bash
python Segmentation/segment_main.py
```

or

```bash
python3 Segmentation/segment_main.py
```

if this causes issues, please make sure you have `requirements.txt` installed (docker should have installed it by
default).

2. Go into `main.cpp` and add 1 line of code:

```cpp
const SegmentationParams params;
params.use_automatic_python_script = false; // This line
auto silhouettes = SamSegmentationGenerator::grabSegmentedImages(params);
```

3. Compile and run the application

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

## Utils

### Known Issues

at some point the docker might give you this error:

```bash
Read timeout
```

This suggest that the docker is not running properly, you can try to restart the docker, or rebuild the docker image.
This is due to if you are running the docker on WSL2, the docker might not have enough memory to run the application.

Another error while automatic segmentation:

```bash
qt.qpa.xcb: could not connect to display 
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "/usr/local/lib/python3.12/site-packages/cv2/qt/plugins" even though it was found.
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: xcb.
```

simple solution to this, is just reinstalling the docker image.

```bash
docker rmi 3dsmc --force // removes the image
docker system prune // removes all unused/untagged images  
```

### Libraries

- **OpenCV**: AR marker detection and segmentation.
- **Eigen**: Matrix operations.
- **Ceres**: Optional for pose optimization.

### Authors

- **Leo Eckert** - [@LeoEckert02]()
- **Baran Gursoy** - [@barangursoy]()
- **Alexander Brovko** - [@XandlerQ]()
- **Giorgi Tsartsidze** - [@giokepa]()
