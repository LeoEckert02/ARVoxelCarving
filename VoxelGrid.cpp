#include "VoxelGrid.h"
#include <fstream>
#include <iostream>

VoxelGrid::VoxelGrid(const BoundingBox& box, int res)
    : boundingBox(box), resolution(res) {
    voxelSize = (boundingBox.maxCorner.x() - boundingBox.minCorner.x()) / resolution;
    voxels.resize(resolution, std::vector<std::vector<bool>>(
                                  resolution, std::vector<bool>(resolution, false)));
}

void VoxelGrid::initializeGridFromBoundingBox() {
    for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
            for (int z = 0; z < resolution; ++z) {
                voxels[x][y][z] = true;
            }
        }
    }
}

bool VoxelGrid::isVoxelOccupied(int x, int y, int z) const {
    if (isValidIndex(x, y, z)) {
        return voxels[x][y][z];
    }
    return false;
}

void VoxelGrid::setVoxelOccupied(int x, int y, int z, bool occupied) {
    if (isValidIndex(x, y, z)) {
        voxels[x][y][z] = occupied;
    }
}

Eigen::Vector3i VoxelGrid::getVoxelIndex(const Eigen::Vector3f& point) const {
    Eigen::Vector3i index;
    index.x() = static_cast<int>((point.x() - boundingBox.minCorner.x()) / voxelSize);
    index.y() = static_cast<int>((point.y() - boundingBox.minCorner.y()) / voxelSize);
    index.z() = static_cast<int>((point.z() - boundingBox.minCorner.z()) / voxelSize);
    return index;
}

bool VoxelGrid::isValidIndex(int x, int y, int z) const {
    return x >= 0 && x < resolution &&
           y >= 0 && y < resolution &&
           z >= 0 && z < resolution;
}

void VoxelGrid::carveVoxels(const std::vector<Eigen::Matrix3f>& intrinsics,
                            const std::vector<Eigen::Matrix4f>& extrinsics,
                            const std::vector<cv::Mat>& silhouettes) {
    for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
            for (int z = 0; z < resolution; ++z) {
                if (!isVoxelOccupied(x, y, z)) continue;

                Eigen::Vector3f voxelCenter = getVoxelCenter(x, y, z);
                bool insideAllSilhouettes = true;

                for (size_t cam = 0; cam < intrinsics.size(); ++cam) {
                    Eigen::Vector4f voxelHomogeneous(voxelCenter.x(), voxelCenter.y(), voxelCenter.z(), 1.0f);
                    Eigen::Vector4f voxelCameraSpace = extrinsics[cam] * voxelHomogeneous;
                    Eigen::Vector3f projected = intrinsics[cam] * voxelCameraSpace.head<3>();
                    int u = static_cast<int>(projected.x() / projected.z());
                    int v = static_cast<int>(projected.y() / projected.z());

                    if (u < 0 || v < 0 || u >= silhouettes[cam].cols || v >= silhouettes[cam].rows ||
                        silhouettes[cam].at<uchar>(v, u) == 0) {
                        insideAllSilhouettes = false;
                        break;
                    }
                }

                if (!insideAllSilhouettes) {
                    setVoxelOccupied(x, y, z, false);
                }
            }
        }
    }
}

void VoxelGrid::exportToPLY(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    int occupiedCount = 0;
    for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
            for (int z = 0; z < resolution; ++z) {
                if (voxels[x][y][z]) {
                    ++occupiedCount;
                }
            }
        }
    }

    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << occupiedCount << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "end_header\n";

    for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
            for (int z = 0; z < resolution; ++z) {
                if (voxels[x][y][z]) {
                    Eigen::Vector3f voxelCenter = boundingBox.minCorner +
                        Eigen::Vector3f(x + 0.5f, y + 0.5f, z + 0.5f) * voxelSize;
                    file << voxelCenter.x() << " " << voxelCenter.y() << " " << voxelCenter.z() << "\n";
                }
            }
        }
    }

    file.close();
    std::cout << "Voxel grid exported to " << filename << "\n";
}


Eigen::Vector3f VoxelGrid::getVoxelCenter(int x, int y, int z) const {
    if (!isValidIndex(x, y, z)) {
        throw std::out_of_range("Invalid voxel indices");
    }
    Eigen::Vector3f voxelCenter;
    voxelCenter.x() = boundingBox.minCorner.x() + (x + 0.5f) * voxelSize;
    voxelCenter.y() = boundingBox.minCorner.y() + (y + 0.5f) * voxelSize;
    voxelCenter.z() = boundingBox.minCorner.z() + (z + 0.5f) * voxelSize;
    return voxelCenter;
}

