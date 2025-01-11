// THIS WAS MY MAIN FOR TESTING VOXEL CARVING

#include <iostream>
#include "VoxelGrid.h"
#include "BoundingBox.h"

int main() {
    // Define bounding box and resolution
    Eigen::Vector3f minCorner(-0.5, -0.5, -0.5);
    Eigen::Vector3f maxCorner(0.5, 0.5, 0.5);
    BoundingBox boundingBox(minCorner, maxCorner);
    int resolution = 50;

    // Create voxel grid
    VoxelGrid voxelGrid(boundingBox, resolution);

    // Create Synthetic silhouettes
    int imageWidth = 500;
    int imageHeight = 500;
    std::vector<cv::Mat> silhouettes;

    // Multiple silhouettes by rotating the camera (Actually creates the same image, not important though since we'll have our own data)
    for (int i = 0; i < 4; ++i) { // Four camera views
        cv::Mat silhouette = cv::Mat::zeros(imageHeight, imageWidth, CV_8UC1);

        // Draw a circle in different orientations
        int centerX = imageWidth / 2;
        int centerY = imageHeight / 2;
        int radius = imageWidth / 4;
        cv::circle(silhouette, cv::Point(centerX, centerY), radius, cv::Scalar(255), -1);

        // Save each silhouette for debugging
        std::string filename = "synthetic_silhouette_" + std::to_string(i) + ".png";
        cv::imwrite(filename, silhouette);
        silhouettes.push_back(silhouette);
    }

    // Camera intrinsics
    Eigen::Matrix3f intrinsics = Eigen::Matrix3f::Identity();
    intrinsics(0, 0) = 500.0f; // Fx
    intrinsics(1, 1) = 500.0f; // Fy
    intrinsics(0, 2) = imageWidth / 2.0f;  // Principal point x
    intrinsics(1, 2) = imageHeight / 2.0f; // Principal point y

    // Multiple camera extrinsics
    std::vector<Eigen::Matrix4f> extrinsics;
    for (int i = 0; i < 4; ++i) {
        Eigen::Matrix4f extrinsic = Eigen::Matrix4f::Identity();
        float angle = (i * M_PI / 2.0f);
        extrinsic(0, 0) = cos(angle);
        extrinsic(0, 2) = sin(angle);
        extrinsic(2, 0) = -sin(angle);
        extrinsic(2, 2) = cos(angle);
        extrinsic(2, 3) = -1.0f;
        extrinsics.push_back(extrinsic);
    }

    // Initialize voxel grid and carve
    voxelGrid.initializeGridFromBoundingBox();
    std::vector<Eigen::Matrix3f> intrinsicsVec(extrinsics.size(), intrinsics);

    voxelGrid.carveVoxels(intrinsicsVec, extrinsics, silhouettes);

    // Export to PLY
    voxelGrid.exportToPLY("carved_voxel_grid.ply");

    // Check voxel occupancy
    int occupiedVoxels = 0;
    for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
            for (int z = 0; z < resolution; ++z) {
                if (voxelGrid.isVoxelOccupied(x, y, z)) {
                    occupiedVoxels++;
                }
            }
        }
    }
    std::cout << "Number of occupied voxels after carving: " << occupiedVoxels << std::endl;

    return 0;
}
