#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "VoxelGrid.h"
#include "BoundingBox.h"
#include "camera_calibration.h"
#include "aruco_marker_detection.h"

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

        bool performCalibration = false;
        cv::Mat cameraMatrix, distCoeffs;

        if (performCalibration) {
            double reprojectionError;

            std::string calibrationFolderPath = "../resources/camera_calibration/images/";
            int checkerboardWidth = 10;
            int checkerboardHeight = 7;

            if (calibrateCamera(calibrationFolderPath, checkerboardWidth, checkerboardHeight,
                                        cameraMatrix, distCoeffs, reprojectionError)) {
                std::cout << "Calibration successful!" << std::endl;
            } else {
                std::cerr << "Calibration failed!" << std::endl;
                return voxelGrid;
            }
        } else {
            // Fetch calibration data from saved file
            cv::FileStorage fs("camera_calib.yml", cv::FileStorage::READ);

            if (!fs.isOpened()) {
                std::cerr << "Error: Could not open 'camera_calib.yml'. Please perform calibration first."
                        << std::endl;
                return voxelGrid;
            }

            fs["cameraMatrix"] >> cameraMatrix;
            fs["distCoeffs"] >> distCoeffs;
            fs.release();
        }

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

        Eigen::Matrix3f intrinsics;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                intrinsics(i, j) = cameraMatrix.at<double>(i, j);
            }
        }
        
        std::cout << "Intrinsic Matrix (3x3):\n" << intrinsics << std::endl;

        std::vector<Eigen::Matrix4f> extrinsics = calculatePoses();

        std::cout << "Extrinsic Matrices (4x4):" << extrinsics.size() << std::endl;
        for (const auto& extrinsic : extrinsics) {
            std::cout << extrinsic << std::endl;
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
