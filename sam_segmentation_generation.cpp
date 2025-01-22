#include "sam_segmentation_generation.h"
#include <Python.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


// void run_python_script(const std::string &scriptDir_path, const std::string &script_path) {
//     Py_Initialize();
//
//     if (!Py_IsInitialized()) {
//         std::cerr << "Error: Python interpreter initialization failed." << std::endl;
//         return;
//     }
//
//     std::string python_code = "import sys\nsys.path.append('" + scriptDir_path + "')\n";
//     PyRun_SimpleString(python_code.c_str());
//
//     std::string requirements_path = scriptDir_path + "/requirements.txt";
//     std::string install_deps_code =
//             "import subprocess\n"
//             "try:\n"
//             "    subprocess.run(['pip', 'install', '-r', '" + requirements_path + "'], check=True)\n"
//             "except Exception as e:\n"
//             "    print(f'Error installing dependencies: {e}')\n";
//     PyRun_SimpleString(install_deps_code.c_str());
//
//     FILE *file = fopen(script_path.c_str(), "r");
//     if (file == nullptr) {
//         std::cerr << "Error: Could not open Python script file." << std::endl;
//         Py_Finalize();
//         return;
//     }
//
//     // Run the Python script
//     int result = PyRun_SimpleFile(file, script_path.c_str());
//     fclose(file);
//
//     if (result == 0) {
//         std::cout << "Python script executed successfully." << std::endl;
//     } else {
//         std::cerr << "Error: Failed to run the Python script." << std::endl;
//     }
//
//     // Clean up the Python interpreter
//     Py_Finalize();
// }


std::vector<cv::Mat> SamSegmentationGenerator::grabSegmentedImages(bool showImages) {
    std::string path = fs::current_path().parent_path().string() + "/resources/segmented_images";
    std::vector<cv::Mat> images;

    // 1) Gather file paths in a vector
    std::vector<std::string> filePaths;
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            filePaths.push_back(entry.path().string());
        }
    }

    // 2) Sort file paths alphabetically
    std::sort(filePaths.begin(), filePaths.end());

    // 3) Load each file in sorted order
    for (const auto &filePath : filePaths) {
        // Load with alpha if present
        cv::Mat img = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        if (!img.empty()) {
            std::cout << "Loaded image: " << filePath << std::endl;

            // Optional: visualize original BGRA
            if (showImages) {
                cv::imshow("Original BGRA", img);
            }

            // Now create a single-channel mask from alpha
            cv::Mat mask;
            if (img.channels() == 4) {
                // Split into B, G, R, A
                std::vector<cv::Mat> bgraChannels(4);
                cv::split(img, bgraChannels);
                
                // Alpha channel is bgraChannels[3]
                mask = bgraChannels[3]; // this is 8-bit alpha

                // If alpha is not strictly 0 or 255, threshold it:
                // cv::threshold(mask, mask, 128, 255, cv::THRESH_BINARY);

            } else if (img.channels() == 3) {
                // No alpha channel. Possibly a normal BGR. 
                // If you truly have a black background with color object,
                // you might do BGR->gray->threshold:
                cv::Mat gray;
                cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
                cv::threshold(gray, mask, 10, 255, cv::THRESH_BINARY);

            } else {
                // Possibly 1-channel or something else
                // (If 1-channel, assume it's already the mask)
                mask = img;
            }

            // Optional: Show the resulting mask
            if (showImages) {
                cv::imshow("Mask", mask);
            }

            // Example: You might want to save out each mask for debugging
            // cv::imwrite("Mask_Debug.png", mask);

            images.push_back(mask);

        } else {
            std::cerr << "Warning: Failed to load image " << filePath << std::endl;
        }
    }

    // 4) If requested, wait to show images
    if (!images.empty() && showImages) {
        std::cout << "Press any key to close all windows..." << std::endl;
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    return images;
}

