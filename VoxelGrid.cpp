#include "VoxelGrid.h"
#include <fstream>
#include <iostream>

VoxelGrid::VoxelGrid(const BoundingBox& box, int resX, int resY, int resZ)
    : boundingBox(box), resolutionX(resX), resolutionY(resY), resolutionZ(resZ) {
    voxelSizeX = (boundingBox.maxCorner.x() - boundingBox.minCorner.x()) / resolutionX;
    voxelSizeY = (boundingBox.maxCorner.y() - boundingBox.minCorner.y()) / resolutionY;
    voxelSizeZ = (boundingBox.maxCorner.z() - boundingBox.minCorner.z()) / resolutionZ;
    voxels.resize(resolutionX, std::vector<std::vector<bool>>(
                                  resolutionY, std::vector<bool>(resolutionZ, false)));
}

void VoxelGrid::initializeGridFromBoundingBox() {
    for (int x = 0; x < resolutionX; ++x) {
        for (int y = 0; y < resolutionY; ++y) {
            for (int z = 0; z < resolutionZ; ++z) {
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
    return {
        static_cast<int>((point.x() - boundingBox.minCorner.x()) / voxelSizeX),
        static_cast<int>((point.y() - boundingBox.minCorner.y()) / voxelSizeY),
        static_cast<int>((point.z() - boundingBox.minCorner.z()) / voxelSizeZ)
    };
}


bool VoxelGrid::isValidIndex(int x, int y, int z) const {
    return x >= 0 && x < resolutionX &&
           y >= 0 && y < resolutionY &&
           z >= 0 && z < resolutionZ;
}

void VoxelGrid::carveVoxels(const std::vector<Eigen::Matrix3f>& intrinsics,
                            const std::vector<Eigen::Matrix4f>& extrinsics,
                            const std::vector<cv::Mat>& silhouettes) {
    for (int x = 0; x < resolutionX; ++x) {
        for (int y = 0; y < resolutionY; ++y) {
            for (int z = 0; z < resolutionZ; ++z) {
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
    for (int x = 0; x < resolutionX; ++x) {
        for (int y = 0; y < resolutionY; ++y) {
            for (int z = 0; z < resolutionZ; ++z) {
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

    for (int x = 0; x < resolutionX; ++x) {
        for (int y = 0; y < resolutionY; ++y) {
            for (int z = 0; z < resolutionZ; ++z) {
                if (voxels[x][y][z]) {
                    Eigen::Vector3f voxelCenter = boundingBox.minCorner +
                        Eigen::Vector3f((x + 0.5f) * voxelSizeX,
                                        (y + 0.5f) * voxelSizeY,
                                        (z + 0.5f) * voxelSizeZ);
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
    return {
        boundingBox.minCorner.x() + (x + 0.5f) * voxelSizeX,
        boundingBox.minCorner.y() + (y + 0.5f) * voxelSizeY,
        boundingBox.minCorner.z() + (z + 0.5f) * voxelSizeZ
    };
}

/*Eigen::MatrixXf cvToEigen(const cv::Mat& cvMat) {
    if (cvMat.empty()) {
        throw std::invalid_argument("Input cv::Mat is empty.");
    }

    if (cvMat.type() != CV_32F && cvMat.type() != CV_64F) {
        throw std::invalid_argument("Only CV_32F or CV_64F type matrices can be converted.");
    }

    Eigen::MatrixXf eigenMat(cvMat.rows, cvMat.cols);

    for (int row = 0; row < cvMat.rows; ++row) {
        for (int col = 0; col < cvMat.cols; ++col) {
            eigenMat(row, col) = static_cast<float>(cvMat.at<double>(row, col));
        }
    }

    return eigenMat;
}*/

std::vector<std::vector<std::vector<bool>>> VoxelGrid::getVoxels() const{
    return voxels;
}

BoundingBox VoxelGrid::getBoundingBox() const{
    return boundingBox;
}