#ifndef ARUCO_POSE_H
#define ARUCO_POSE_H

#include <string>
#include <vector>
#include <Eigen/Dense>

std::vector<Eigen::Matrix4f> calculatePoses();

#endif // ARUCO_POSE_H
