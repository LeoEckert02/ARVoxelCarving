#include "sam_segmentation_generation.h"
#include <Python.h>
#include <iostream>
#include <opencv2/highgui.hpp>


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


std::vector<cv::Mat> SamSegmentationGenerator::grabSegmentedImages() {
    std::string path = fs::current_path().parent_path().string() + "/resources/segmented_images";

    std::vector<cv::Mat> images;
    for (const auto &entry: fs::directory_iterator(path)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
            if (!img.empty()) {
                std::cout << entry.path() << std::endl;
                std::string windowName = "Image: " + entry.path().filename().string();
                cv::imshow(windowName, img);

                images.push_back(img);
            } else {
                std::cerr << "Warning: Failed to load image " << entry.path() << std::endl;
            }
        }
    }

    if (!images.empty()) {
        std::cout << "Press any key to close all windows..." << std::endl;
        cv::waitKey(0); // Wait for a key press
        cv::destroyAllWindows(); // Close all OpenCV windows
    }

    return images;
}
