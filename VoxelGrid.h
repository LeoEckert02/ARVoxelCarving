#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include <Eigen/Dense>
#include <vector>
#include "BoundingBox.h"
#include <opencv2/opencv.hpp>

class VoxelGrid {
public:
    VoxelGrid(const BoundingBox& box, int resX, int resY, int resZ);
    void initializeGridFromBoundingBox();
    bool isVoxelOccupied(int x, int y, int z) const;
    void setVoxelOccupied(int x, int y, int z, bool occupied);
    Eigen::Vector3i getVoxelIndex(const Eigen::Vector3f& point) const;
    Eigen::Vector3f getVoxelCenter(int x, int y, int z) const;
    bool isValidIndex(int x, int y, int z) const;
    void carveVoxels(const std::vector<Eigen::Matrix3f>& intrinsics,
                     const std::vector<Eigen::Matrix4f>& extrinsics,
                     const std::vector<cv::Mat>& silhouettes);
    void exportToPLY(const std::string& filename) const;
    Eigen::MatrixXf cvToEigen(const cv::Mat& cvMat);
    std::vector<std::vector<std::vector<bool>>> getVoxels() const;
    BoundingBox getBoundingBox() const;

private:
    BoundingBox boundingBox;
    int resolutionX, resolutionY, resolutionZ;
    float voxelSizeX, voxelSizeY, voxelSizeZ;
    std::vector<std::vector<std::vector<bool>>> voxels;
};

#endif  // VOXEL_GRID_H
