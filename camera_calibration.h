#ifndef CAMERA_CALIBRATION_H
#define CAMERA_CALIBRATION_H

#include <opencv2/core.hpp>
#include <string>
#include <vector>

bool calibrateCamera(const std::string &folderPath,
                               int checkerboardWidth,
                               int checkerboardHeight,
                               cv::Mat &cameraMatrix,
                               cv::Mat &distCoeffs,
                               double &reprojectionError);

#endif // CAMERA_CALIBRATION_H
