#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include "VoxelGrid.h"
#include "BoundingBox.h"
#include "camera_calibration.h"
#include "aruco_marker_detection.h"

class VoxelCarve {
private:
    Eigen::Vector3f minCorner;
    Eigen::Vector3f maxCorner;
    int resolutionX;
    int resolutionY;
    int resolutionZ;
    const int desiredResolutionForLargestDim;
    std::vector<cv::Mat> silhouettes;
    bool performCalibration;

public:
    VoxelCarve(std::vector<cv::Mat> silhouettes, int desiredResolutionForLargestDim, bool performCalibration = false)
        : minCorner(-0.15f, -0.105f, 0.0f), maxCorner(0.15f, 0.105f, 0.2f), silhouettes(silhouettes), desiredResolutionForLargestDim(desiredResolutionForLargestDim), 
        performCalibration(performCalibration) {
            std::cout << "Started Voxel Carving!\n";
        float dx = maxCorner.x() - minCorner.x();
        float dy = maxCorner.y() - minCorner.y();
        float dz = maxCorner.z() - minCorner.z();

        float largestDim = std::max({dx, dy, dz});

        float voxelEdge = largestDim / static_cast<float>(desiredResolutionForLargestDim);

        resolutionX = static_cast<int>(std::round(dx / voxelEdge));
        resolutionY = static_cast<int>(std::round(dy / voxelEdge));
        resolutionZ = static_cast<int>(std::round(dz / voxelEdge));

        std::cout << "dx,dy,dz = " << dx << ", " << dy << ", " << dz << std::endl;
        std::cout << "VoxelEdge = " << voxelEdge << " (meters)" << std::endl;
        std::cout << "ResX, ResY, ResZ = " 
                << resolutionX << ", " 
                << resolutionY << ", " 
                << resolutionZ << std::endl;
        }

    VoxelGrid carve_voxel_grid() {
        BoundingBox boundingBox(minCorner, maxCorner);
        VoxelGrid voxelGrid(boundingBox, resolutionX, resolutionY, resolutionZ);
        
        cv::Mat cameraMatrix, distCoeffs;

        std::cout << "PerformCalibration: "<< performCalibration<<std::endl;

        if (performCalibration) {
            double reprojectionError;

            std::string calibrationFolderPath = "../resources/u_calibration/";
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

        std::vector<Eigen::Matrix4f> extrinsics;
        std::unordered_map<int, std::pair<cv::Mat, cv::Mat>> marker_positions;
        calculatePoses(extrinsics, marker_positions);

        for (auto &extr: extrinsics) {
            Eigen::Matrix3f R = extr.block<3, 3>(0, 0);
            Eigen::Vector3f C = extr.block<3, 1>(0, 3);
            Eigen::Matrix3f R_inv = R.transpose();
            Eigen::Vector3f t = -R_inv * C;
            Eigen::Matrix4f E = Eigen::Matrix4f::Identity();
            E.block<3, 3>(0, 0) = R_inv;
            E.block<3, 1>(0, 3) = t;
            extr = E;
        }

        std::cout << "Extrinsic Matrices (4x4):" << extrinsics.size() << std::endl;
        for (const auto& extrinsic : extrinsics) {
            std::cout << extrinsic << std::endl << std::endl;
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
