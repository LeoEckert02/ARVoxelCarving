#include <filesystem>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
  cv::Mat cameraMatrix, distCoeffs;
  bool useCalibration = true;
  if (useCalibration) {
    cv::FileStorage fs("camera_calib.yml", cv::FileStorage::READ);
    if (!fs.isOpened()) {
      std::cerr << "Failed to open camera calibration file. Proceeding without "
                   "calibration.\n";
      useCalibration = false;
    } else {
      fs["cameraMatrix"] >> cameraMatrix;
      fs["distCoeffs"] >> distCoeffs;
      fs.release();
    }
  }

  std::string folderPath = "../resources/aruco_markers/images/";
  std::vector<std::string> imagePaths;

  for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
    imagePaths.push_back(entry.path().string());
  }

  const float markerLength = 0.03f; // measured from printed pdf (3cm x 3cm)

  for (const std::string &imagePath : imagePaths) {
    cv::Mat frame = cv::imread(imagePath);

    if (frame.empty()) {
      std::cout << "Error: Could not load image " << imagePath << std::endl;
      continue;
    }

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters detectorParams =
        cv::aruco::DetectorParameters();
    cv::aruco::Dictionary dictionary =
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_50);
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);
    detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);

    if (!markerIds.empty()) {

      if (useCalibration) {
        cv::Mat objPoints(4, 1, CV_32FC3);
        objPoints.ptr<cv::Vec3f>(0)[0] =
            cv::Vec3f(-markerLength / 2.f, markerLength / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[1] =
            cv::Vec3f(markerLength / 2.f, markerLength / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[2] =
            cv::Vec3f(markerLength / 2.f, -markerLength / 2.f, 0);
        objPoints.ptr<cv::Vec3f>(0)[3] =
            cv::Vec3f(-markerLength / 2.f, -markerLength / 2.f, 0);

        size_t nMarkers = markerCorners.size();
        // rotation vectors and translation vectors
        std::vector<cv::Vec3f> rvecs(nMarkers), tvecs(nMarkers);
        for (size_t i = 0; i < nMarkers; i++) {
          cv::solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs,
                       rvecs.at(i), tvecs.at(i));
        }

        cv::Mat outputImage = frame.clone();
        for (size_t i = 0; i < markerIds.size(); i++) {
          cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);

          cv::drawFrameAxes(outputImage, cameraMatrix, distCoeffs, rvecs[i],
                            tvecs[i], markerLength * 1.5f, 5);

          std::cout << "Image: " << imagePath << ", Marker ID: " << markerIds[i]
                    << std::endl;
          std::cout << "Rotation Vector: " << rvecs[i] << std::endl;
          std::cout << "Translation Vector: " << tvecs[i] << std::endl;
        }

        cv::imshow("ArUco Detection", outputImage);
        cv::waitKey(5000);
      }
    } else {
      std::cout << "No markers detected in image: " << imagePath << std::endl;
    }
  }

  cv::destroyAllWindows();
  return 0;
}