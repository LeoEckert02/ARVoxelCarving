#include <iostream>
#include "VoxelGrid.h"
#include "BoundingBox.h"

int main() {
    // Step 1: Define bounding box
    Eigen::Vector3f minCorner(-1.0, -1.0, -1.0); // Define the bounding box
    Eigen::Vector3f maxCorner(1.0, 1.0, 1.0);   // Larger bounding box for diverse testing
    BoundingBox boundingBox(minCorner, maxCorner);

    // Step 2: Define resolutions for each axis
    int resolutionX = 50;
    int resolutionY = 100; // Non-cubic resolution for "long" objects
    int resolutionZ = 25;

    // Step 3: Create voxel grid
    VoxelGrid voxelGrid(boundingBox, resolutionX, resolutionY, resolutionZ);

    // Step 4: Generate synthetic silhouettes
    int imageWidth = 500;
    int imageHeight = 500;
    std::vector<cv::Mat> silhouettes;

    // Create silhouettes for multiple camera views
    for (int i = 0; i < 4; ++i) { // Simulate four camera views
        cv::Mat silhouette = cv::Mat::zeros(imageHeight, imageWidth, CV_8UC1);

        // Draw a circle in each view
        int centerX = imageWidth / 2;
        int centerY = imageHeight / 2;
        int radius = imageWidth / 4;
        cv::circle(silhouette, cv::Point(centerX, centerY), radius, cv::Scalar(255), -1);

        // Save silhouette for debugging
        std::string filename = "synthetic_silhouette_" + std::to_string(i) + ".png";
        cv::imwrite(filename, silhouette);
        silhouettes.push_back(silhouette);
    }

    // Step 5: Define camera intrinsics
    Eigen::Matrix3f intrinsics = Eigen::Matrix3f::Identity();
    intrinsics(0, 0) = 500.0f; // Focal length x
    intrinsics(1, 1) = 500.0f; // Focal length y
    intrinsics(0, 2) = imageWidth / 2.0f;  // Principal point x
    intrinsics(1, 2) = imageHeight / 2.0f; // Principal point y

    // Step 6: Define camera extrinsics
    std::vector<Eigen::Matrix4f> extrinsics;
    for (int i = 0; i < 4; ++i) {
        Eigen::Matrix4f extrinsic = Eigen::Matrix4f::Identity();
        float angle = (i * M_PI / 2.0f); // Rotate 90 degrees per view
        extrinsic(0, 0) = cos(angle);
        extrinsic(0, 2) = sin(angle);
        extrinsic(2, 0) = -sin(angle);
        extrinsic(2, 2) = cos(angle);
        extrinsic(2, 3) = -1.0f; // Move camera back along z-axis
        extrinsics.push_back(extrinsic);
    }

    // Step 7: Initialize voxel grid and carve
    voxelGrid.initializeGridFromBoundingBox();
    std::vector<Eigen::Matrix3f> intrinsicsVec(extrinsics.size(), intrinsics);
    voxelGrid.carveVoxels(intrinsicsVec, extrinsics, silhouettes);

    // Step 8: Export carved voxel grid to PLY
    voxelGrid.exportToPLY("carved_voxel_grid.ply");

    // Step 9: Verify voxel occupancy
    int occupiedVoxels = 0;
    for (int x = 0; x < resolutionX; ++x) {
        for (int y = 0; y < resolutionY; ++y) {
            for (int z = 0; z < resolutionZ; ++z) {
                if (voxelGrid.isVoxelOccupied(x, y, z)) {
                    occupiedVoxels++;
                }
            }
        }
    }

    std::cout << "Number of occupied voxels after carving: " << occupiedVoxels << std::endl;

    return 0;
}
