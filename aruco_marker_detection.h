#ifndef ARUCO_POSE_H
#define ARUCO_POSE_H

#include <string>
#include <vector>
#include <Eigen/Dense>

void calculatePoses(std::vector<Eigen::Matrix4f>& extrinsics, std::unordered_map<int, std::pair<cv::Mat, cv::Mat>>& marker_to_world);

#endif // ARUCO_POSE_H
