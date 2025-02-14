#include "sam_segmentation_generation.h"
#include <Python.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


std::vector<cv::Mat> SamSegmentationGenerator::grabSegmentedImages(const SegmentationParams &params) {
    std::string path = fs::current_path().parent_path().string() + "/resources/u_segmented_output";
    std::vector<cv::Mat> images;

    std::cout << params.pythonVersion << std::endl;

    // Check if directory is empty
    if (fs::is_empty(path)) {
        run_segmentation_script(params);
    }
    // Call this to run the segmentation script

    std::vector<std::string> filePaths;
    for (const auto &entry: fs::directory_iterator(path)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            filePaths.push_back(entry.path().string());
        }
    }

    std::sort(filePaths.begin(), filePaths.end());

    for (const auto &filePath: filePaths) {
        cv::Mat img = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        if (!img.empty()) {
            std::cout << "Loaded image: " << filePath << std::endl;

            if (params.show_results) {
                cv::imshow("Original BGRA", img);
            }

            cv::Mat mask;
            if (img.channels() == 4) {
                std::vector<cv::Mat> bgraChannels(4);
                cv::split(img, bgraChannels);

                mask = bgraChannels[3];
            } else if (img.channels() == 3) {
                cv::Mat gray;
                cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
                cv::threshold(gray, mask, 10, 255, cv::THRESH_BINARY);
            } else {
                mask = img;
            }

            if (params.show_results) {
                cv::imshow("Mask", mask);
            }

            images.push_back(mask);
        } else {
            std::cerr << "Warning: Failed to load image " << filePath << std::endl;
        }
    }

    if (!images.empty() && params.show_results) {
        std::cout << "Press any key to close all windows..." << std::endl;
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    return images;
}


void SamSegmentationGenerator::run_segmentation_script(const SegmentationParams &params) {
    std::string command = "python3 ../Segmentation/segment_main.py";

    command += " " + std::string(params.needs_input ? "True" : "False");
    command += " " + std::string(params.needs_bounding_box ? "True" : "False");
    command += " " + std::string(params.show_results ? "True" : "False");

    int status = std::system(command.c_str());

    if (status != 0) {
        std::cerr << "Error: Python script execution failed with status " << status << std::endl;
    }
    std::cout << "Python script executed successfully!" << std::endl;
    // DEPRECATED CODE

//    Py_Initialize();
//
//    PyRun_SimpleString("import sys");
//    std::string addPathCommand = std::string("sys.path.append('") + params.segmentationDir + "')";
//    PyRun_SimpleString(addPathCommand.c_str());
//    PyRun_SimpleString("print(sys.path)");
//    PyRun_SimpleString("import numpy; print(numpy.__file__)");
//
//    if (fs::exists(params.segmentationDir + "/" + params.venvName)) {
//        enable_virtual_environment(params);
//    } else {
//        std::cout <<
//                "This is the first run. (will take a while) Creating virtual environment and installing dependencies..."
//                << std::endl;
//        create_setup_venv(params);
//        enable_virtual_environment(params);
//    }


//    // Load the Python script
//    const char *scriptName = "segment_main";
//    PyObject *pModule = PyImport_ImportModule(scriptName);
//    if (pModule == nullptr) {
//        PyErr_Print();
//        std::cerr << "Error: Failed to import Python script." << std::endl;
//    }
//
//    Py_Finalize();
}

// DEPRECATED CODE

void SamSegmentationGenerator::create_setup_venv(const SegmentationParams &params) {
    try {
        const std::string venv_path = params.segmentationDir + "/" + params.venvName;
        const std::string requirements_path = params.segmentationDir + "/requirements.txt";

        std::string create_venv_command = "import venv; venv.create('" + venv_path + "', with_pip=True)";
        PyRun_SimpleString(create_venv_command.c_str());

        std::string install_requirements_command =
                "import subprocess\n"
                "subprocess.check_call([r'" + venv_path + "/bin/python', '-m', 'pip', 'install', '-r', r'" +
                requirements_path + "'])";
        PyRun_SimpleString(install_requirements_command.c_str());

        std::cout << "Virtual environment created and dependencies installed successfully!" << std::endl;
    } catch (...) {
        PyErr_Print();
        std::cerr << "An error occurred while creating the virtual environment or installing dependencies." <<
                  std::endl;
    }
}

void SamSegmentationGenerator::enable_virtual_environment(const SegmentationParams &params) {
    std::string venv_path = params.segmentationDir + "/" + params.venvName;
    try {
        std::string site_packages = venv_path + "/lib/" + params.pythonVersion + "/site-packages";
        if (!fs::exists(site_packages)) {
            throw std::runtime_error("site-packages not found at: " + site_packages);
        }

        std::string add_site_packages =
                "import sys\n"
                "sys.path.insert(0, r'" + site_packages + "')";
        PyRun_SimpleString(add_site_packages.c_str());

        std::string set_env_vars =
                "import os\n"
                "os.environ['PYTHONHOME'] = r'" + venv_path + "'\n"
                                                              "os.environ['PYTHONPATH'] = r'" + site_packages + "'";
        PyRun_SimpleString(set_env_vars.c_str());

        std::cout << "Virtual environment enabled successfully." << std::endl;
    } catch (const std::exception &e) {
        PyErr_Print();
        std::cerr << "Error enabling virtual environment: " << e.what() << std::endl;
    }
}