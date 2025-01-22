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
    int resolutionX = 100;
    int resolutionY = 100;
    int resolutionZ = 100;
    std::vector<cv::Mat> silhouettes;
    bool performCalibration;

public:
    VoxelCarveTest(std::vector<cv::Mat> silhouettes, bool performCalibration = false)
        : minCorner(-0.2f, -0.2f, -0.2f), maxCorner(0.2f, 0.2f, 0.2f), silhouettes(silhouettes), performCalibration(performCalibration) {
            std::cout << "Started to Voxel Carving!\n";
        }

    VoxelGrid Test() {
        BoundingBox boundingBox(minCorner, maxCorner);
        VoxelGrid voxelGrid(boundingBox, resolutionX, resolutionY, resolutionZ);
        
        cv::Mat cameraMatrix, distCoeffs;

        std::cout << "PerformCalibration: "<< performCalibration<<std::endl;

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
