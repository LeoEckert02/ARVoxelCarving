#include <filesystem>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
  int CHECKERBOARD[2]{10, 7}; // 10x7 checkerboard (inner corners)

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

  std::string folderPath = "../resources/camera_calibration/images/";
  std::vector<std::string> imagePaths;

  // Collect image paths from the directory
  for (const auto &entry :
       std::filesystem::directory_iterator(folderPath)) {
    imagePaths.push_back(entry.path().string());
  }

  cv::Mat frame, gray;
  std::vector<cv::Point2f> corners;

  // Process each image
  for (const std::string &imagePath : imagePaths) {
    frame = cv::imread(imagePath);

    if (frame.empty()) {
      std::cout << "Error: Could not load image " << imagePath << std::endl;
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
      cv::imshow("Chessboard Detection", frame);
      cv::waitKey(500); // Pause to visualize each detection
    } else {
      std::cout << "Chessboard not found in " << imagePath << std::endl;
    }
  }

  cv::destroyAllWindows();

  // Perform camera calibration
  cv::Mat cameraMatrix, distCoeffs, R, T;
  std::vector<cv::Mat> rvecs, tvecs;

  if (!imgpoints.empty()) {
    double error = cv::calibrateCamera(objpoints, imgpoints, gray.size(),
                                       cameraMatrix, distCoeffs, rvecs, tvecs);

    // Output the camera calibration data
    std::cout << "Camera Matrix: " << std::endl << cameraMatrix << std::endl;
    std::cout << "Distortion Coefficients: " << std::endl << distCoeffs << std::endl;
    std::cout << "Overall Re-projection Error: " << error << std::endl;

    // Save camera calibration to file
    cv::FileStorage fs("camera_calib.yml", cv::FileStorage::WRITE);
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs.release();
    std::cout << "Calibration parameters saved to 'camera_calib.yml'." << std::endl;
  } else {
    std::cout << "Not enough valid images for calibration!" << std::endl;
  }

  return 0;
}
