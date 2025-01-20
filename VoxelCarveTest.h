#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "VoxelGrid.h"
#include "BoundingBox.h"

class VoxelCarveTest {
private:
    Eigen::Vector3f minCorner;
    Eigen::Vector3f maxCorner;
    int resolutionX = 300;
    int resolutionY = 300;
    int resolutionZ = 300;
    int imageWidth = 500;
    int imageHeight = 500;

public:
    VoxelCarveTest()
        : minCorner(-1.0f, -1.0f, -1.0f), maxCorner(1.0f, 1.0f, 1.0f) {}

    VoxelGrid Test() {
        BoundingBox boundingBox(minCorner, maxCorner);
        VoxelGrid voxelGrid(boundingBox, resolutionX, resolutionY, resolutionZ);
        std::vector<cv::Mat> silhouettes;

        for (int i = 0; i < 4; ++i) {
            cv::Mat silhouette = cv::Mat::zeros(imageHeight, imageWidth, CV_8UC1);
            int centerX = imageWidth / 2;
            int centerY = imageHeight / 2;
            int radius = imageWidth / 4;
            cv::circle(silhouette, cv::Point(centerX, centerY), radius, cv::Scalar(255), -1);

            std::string filename = "synthetic_silhouette_" + std::to_string(i) + ".png";
            if (!cv::imwrite(filename, silhouette)) {
                std::cerr << "Failed to write silhouette file: " << filename << std::endl;
                continue;
            }
            silhouettes.push_back(silhouette);
        }

        Eigen::Matrix3f intrinsics = Eigen::Matrix3f::Identity();
        intrinsics(0, 0) = 500.0f;
        intrinsics(1, 1) = 500.0f;
        intrinsics(0, 2) = imageWidth / 2.0f;
        intrinsics(1, 2) = imageHeight / 2.0f;

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

        voxelGrid.initializeGridFromBoundingBox();
        std::vector<Eigen::Matrix3f> intrinsicsVec(extrinsics.size(), intrinsics);
        voxelGrid.carveVoxels(intrinsicsVec, extrinsics, silhouettes);
        voxelGrid.exportToPLY("carved_voxel_grid.ply");

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
        return voxelGrid;
    }
};
