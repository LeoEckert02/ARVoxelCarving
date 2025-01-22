#include "camera_calibration.h"
#include <filesystem>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

bool calibrateCamera(const std::string &folderPath,
                               int checkerboardWidth,
                               int checkerboardHeight,
                               cv::Mat &cameraMatrix,
                               cv::Mat &distCoeffs,
                               double &reprojectionError,
                               bool showImage) {
    // Checkerboard dimensions
    int CHECKERBOARD[2]{checkerboardWidth, checkerboardHeight};

    // Vectors to store 3D points in real world and 2D points in image plane
    std::vector<std::vector<cv::Point3f>> objpoints;
    std::vector<std::vector<cv::Point2f>> imgpoints;

    // Define world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for (int i = 0; i < CHECKERBOARD[1]; i++) {
        for (int j = 0; j < CHECKERBOARD[0]; j++) {
            objp.push_back(cv::Point3f(j, i, 0));
        }
    }

    std::vector<std::string> imagePaths;
    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        imagePaths.push_back(entry.path().string());
    }

    if (imagePaths.empty()) {
        std::cerr << "Error: No images found in directory " << folderPath << std::endl;
        return false;
    }

    cv::Mat frame, gray;
    std::vector<cv::Point2f> corners;

    for (const std::string &imagePath : imagePaths) {
        frame = cv::imread(imagePath);

        if (frame.empty()) {
            std::cerr << "Error: Could not load image " << imagePath << std::endl;
            continue;
        }

        // Convert to grayscale
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Find the chessboard corners
        bool found = cv::findChessboardCorners(
            gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corners,
            cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK |
                cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            // Refine corner locations
            cv::cornerSubPix(
                gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30,
                                 0.001));

            imgpoints.push_back(corners);
            objpoints.push_back(objp);

            // Draw and display the corners
            cv::drawChessboardCorners(
                frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corners, found);
            
            if(showImage){
                cv::imshow("Chessboard Detection", frame);
            }
            cv::waitKey(1000); // Pause to visualize each detection
        } else {
            std::cout << "Chessboard not found in " << imagePath << std::endl;
        }
    }

    cv::destroyAllWindows();

    if (!imgpoints.empty()) {
        reprojectionError = cv::calibrateCamera(objpoints, imgpoints, gray.size(),
                                                cameraMatrix, distCoeffs,
                                                cv::noArray(), cv::noArray());

        // Save camera calibration to file
        cv::FileStorage fs("camera_calib.yml", cv::FileStorage::WRITE);
        fs << "cameraMatrix" << cameraMatrix;
        fs << "distCoeffs" << distCoeffs;
        fs.release();

        std::cout << "Calibration parameters saved to 'camera_calib.yml'."
                  << std::endl;
        return true;
    } else {
        std::cerr << "Error: Not enough valid images for calibration!" << std::endl;
        return false;
    }
}
