#pragma once

#include <string>
#include <opencv2/core/mat.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>


namespace fs = boost::filesystem;


struct SegmentationParams {
public:
    std::string segmentationDir = fs::current_path().parent_path().string() + "/Segmentation";
    std::string venvName = "pythonvenv";
    std::string pythonVersion = "python3.12";
    bool needs_input = false;
    bool needs_bounding_box = false;
    bool show_results = false;
};

class SamSegmentationGenerator {
public:
    static std::vector<cv::Mat> grabSegmentedImages(const SegmentationParams &params = {});

    static void run_segmentation_script(const SegmentationParams &params);

    static void enable_virtual_environment(const SegmentationParams &params);

    static void create_setup_venv(const SegmentationParams &params);
};
