#pragma once

#include <string>
#include <opencv2/core/mat.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>


namespace fs = boost::filesystem;

class SamSegmentationGenerator {
public:
    static std::vector<cv::Mat> grabSegmentedImages(bool showImages = false);
};
