#include <filesystem>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <vector>
#include <Eigen/Dense>
#include <algorithm>

// Convert OpenCV rotation and translation to Eigen matrix (float)
Eigen::Matrix4f cvToEigenTransform(const cv::Mat& R, const cv::Mat& t) {
    Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();

    // Put rotation into the matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            transform(i, j) = static_cast<float>(R.at<double>(i, j));
        }
    }

    // Put translation into the matrix
    transform(0, 3) = static_cast<float>(t.at<double>(0));
    transform(1, 3) = static_cast<float>(t.at<double>(1));
    transform(2, 3) = static_cast<float>(t.at<double>(2));

    std::cout << "Transform:\n " << transform << std::endl;

    return transform;
}

// Calculate camera pose from marker pose
void calculateCameraPose(const cv::Mat &R_m2c, const cv::Mat &t_m2c,
                         const cv::Mat &R_m2w, const cv::Mat &t_m2w,
                         cv::Mat &R_c2w, cv::Mat &t_c2w) {
    cv::Mat R_m2w_d, R_m2c_d, t_m2c_d, t_m2w_d;
    R_m2w.convertTo(R_m2w_d, CV_64F);
    R_m2c.convertTo(R_m2c_d, CV_64F);
    t_m2c.convertTo(t_m2c_d, CV_64F);
    t_m2w.convertTo(t_m2w_d, CV_64F);

    R_c2w = R_m2w_d * R_m2c_d.t();
    t_c2w = t_m2w_d - R_c2w * t_m2c_d;
}

// Average rotation matrices
cv::Mat averageRotations(const std::vector<cv::Mat>& rotations) {
    cv::Mat R_avg = cv::Mat::zeros(3, 3, CV_64F);
    for (const auto& R : rotations) {
        R_avg += R;
    }
    R_avg /= rotations.size();

    cv::Mat U, S, Vt;
    cv::SVD::compute(R_avg, S, U, Vt);
    return U * Vt; // Ensure the result is orthogonal
}

// Average translation vectors
cv::Mat averageTranslations(const std::vector<cv::Mat>& translations) {
    cv::Mat t_avg = cv::Mat::zeros(3, 1, CV_64F);
    for (const auto& t : translations) {
        t_avg += t;
    }
    t_avg /= translations.size();
    return t_avg;
}

// Function to calculate camera poses
void calculatePoses(std::vector<Eigen::Matrix4f>& extrinsics,
                    std::unordered_map<int, std::pair<cv::Mat, cv::Mat>>& marker_to_world) {
    cv::Mat initial_R_c2w, initial_t_c2w;

    cv::Mat cameraMatrix, distCoeffs;
    bool useCalibration = true;
    if (useCalibration) {
        cv::FileStorage fs("camera_calib.yml", cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Failed to open camera calibration file. Proceeding without calibration.\n";
            useCalibration = false;
        } else {
            fs["cameraMatrix"] >> cameraMatrix;
            fs["distCoeffs"] >> distCoeffs;
            fs.release();
        }
    }

    std::string folderPath = "../resources/u_input/";
    std::vector<std::string> imagePaths;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        imagePaths.push_back(entry.path().string());
    }

    std::sort(imagePaths.begin(), imagePaths.end());

    const float markerLength = 0.03f; // Measured from printed PDF (3cm x 3cm)
    std::vector<cv::Mat> camera_positions, camera_rotations;

    bool initial_frame_set = false;

    for (const std::string& imagePath : imagePaths) {
        cv::Mat frame = cv::imread(imagePath);
        std::cout << "Doing extrinsics calculation for " << imagePath << std::endl;

        if (frame.empty()) {
            continue;
        }

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_50);
        cv::aruco::ArucoDetector detector(dictionary, detectorParams);
        detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);

        if (!markerIds.empty() && useCalibration) {
            // Average rotation matrix for the world
            cv::Mat avg_R_world = cv::Mat::eye(3, 3, CV_64F);
            // Average translation (centroid) for the world
            cv::Mat avg_t_world = cv::Mat::zeros(3, 1, CV_64F); 
            std::vector<cv::Mat> marker_rotations, marker_translations;

            cv::Mat objPoints(4, 1, CV_32FC3);
            objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength / 2.f, markerLength / 2.f, 0);
            objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength / 2.f, markerLength / 2.f, 0);
            objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength / 2.f, -markerLength / 2.f, 0);
            objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength / 2.f, -markerLength / 2.f, 0);

            size_t nMarkers = markerCorners.size();
            std::vector<cv::Vec3f> rvecs(nMarkers), tvecs(nMarkers);
            for (size_t i = 0; i < nMarkers; i++) {
                cv::solvePnP(objPoints, markerCorners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i));
            }

            std::vector<cv::Mat> R_candidates, t_candidates;
            bool initial_marker_set = false;
            cv::Mat initial_R_m2c, initial_t_m2c;
            for (size_t i = 0; i < markerIds.size(); ++i) {
                int markerId = markerIds[i];

                cv::Mat R_m2c, t_m2c;
                cv::Rodrigues(rvecs[i], R_m2c);
                t_m2c = cv::Mat(tvecs[i]).clone();

                if (!initial_frame_set) {
                    // Collect all rotations and translations for averaging in the first frame
                    marker_rotations.push_back(R_m2c.clone());
                    marker_translations.push_back(t_m2c.clone());
                } else {
                    if (marker_to_world.find(markerId) == marker_to_world.end())
                        continue;

                    cv::Mat R_m2w = marker_to_world[markerId].first;
                    cv::Mat t_m2w = marker_to_world[markerId].second;

                    cv::Mat R_c2w, t_c2w;
                    calculateCameraPose(R_m2c, t_m2c, R_m2w, t_m2w, R_c2w, t_c2w);

                    R_candidates.push_back(R_c2w);
                    t_candidates.push_back(t_c2w);
                }
            }

            
            if (!initial_frame_set && !marker_rotations.empty()) {
                // First frame: calculate average rotation and translation
                avg_R_world = averageRotations(marker_rotations);
                avg_t_world = averageTranslations(marker_translations);

                cv::Mat avg_R_world_32f;
                avg_R_world.convertTo(avg_R_world_32f, CV_32F);
                cv::Mat avg_t_world_32f;
                avg_t_world.convertTo(avg_t_world_32f, CV_32F);

                // Set up the world origin
                for (size_t i = 0; i < markerIds.size(); ++i) {
                    int markerId = markerIds[i];
                    cv::Mat R_m2c = marker_rotations[i];
                    cv::Mat t_m2c = marker_translations[i];

                    // Transform to world origin
                    cv::Mat t_m2w = avg_R_world_32f.t() * (t_m2c - avg_t_world_32f);
                    cv::Mat R_m2w = avg_R_world_32f.t() * R_m2c;

                    // set Z of marker position to 0 manually, as they should always be perfectly level
                    t_m2w.at<float>(2, 0) = 0;

                    marker_to_world[markerId] = {R_m2w.clone(), t_m2w.clone()};
                }

                initial_frame_set = true;
            }

            if (!R_candidates.empty() && !t_candidates.empty()) {
                cv::Mat R_avg = averageRotations(R_candidates);
                cv::Mat t_avg = averageTranslations(t_candidates);

                camera_positions.push_back(t_avg.clone());
                camera_rotations.push_back(R_avg.clone());
            }
        }
    }

    // Print marker_to_world mappings
    std::cout << "Marker to World Mappings:\n";
    for (const auto& [id, transform] : marker_to_world) {
        std::cout << "Marker ID: " << id << "\nRotation:\n" << transform.first
                  << "\nTranslation:\n" << transform.second << "\n";
    }

    for (size_t i = 0; i < camera_positions.size(); ++i) {
        extrinsics.push_back(cvToEigenTransform(camera_rotations[i], camera_positions[i]));
    }
}
