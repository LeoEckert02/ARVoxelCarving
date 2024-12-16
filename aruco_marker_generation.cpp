#include <hpdf.h>

#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no,
                   void *user_data) {
  std::cerr << "ERROR: error_no=" << error_no << ", detail_no=" << detail_no
            << std::endl;
  throw std::runtime_error("libharu error");
}

cv::Mat generateArucoMarker(int markerId, int markerSize) {
  cv::Mat marker;
  cv::aruco::Dictionary dictionary =
      cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_50);
  cv::aruco::generateImageMarker(dictionary, markerId, markerSize, marker, 1);
  return marker;
}

void embedMarkerAtPosition(HPDF_Doc pdf, HPDF_Page page, const cv::Mat &marker,
                           float x, float y) {
  // Convert marker to RGB format
  cv::Mat markerRGB;
  cv::cvtColor(marker, markerRGB, cv::COLOR_GRAY2RGB);

  // Create raw image data buffer for libharu
  std::vector<unsigned char> imageData(markerRGB.total() *
                                       markerRGB.elemSize());
  std::memcpy(imageData.data(), markerRGB.data, imageData.size());

  HPDF_Image image =
      HPDF_LoadRawImageFromMem(pdf, imageData.data(), markerRGB.cols,
                               markerRGB.rows, HPDF_CS_DEVICE_RGB, 8);

  if (!image) {
    throw std::runtime_error("Cannot load raw image into PDF.");
  }

  HPDF_Page_DrawImage(page, image, x, y, markerRGB.cols, markerRGB.rows);
}

int main() {
  try {
    const int markerSize = 80;
    std::vector<int> markerIds = {0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<cv::Mat> markers;
    for (int id : markerIds) {
      markers.push_back(generateArucoMarker(id, markerSize));
    }

    HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
    if (!pdf) {
      std::cerr << "ERROR: Cannot create PDF document." << std::endl;
      return 1;
    }

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    float pageWidth = HPDF_Page_GetWidth(page);
    float pageHeight = HPDF_Page_GetHeight(page);

    float margin = 10; // Margin from the edge of the page
    std::vector<std::pair<float, float>> positions = {
        {margin, margin}, // Bottom-left corner
        {margin, pageHeight / 2 - margin - markerSize / 2}, // Left-middle
        {pageWidth - margin - markerSize, margin}, // Bottom-right corner
        {pageWidth - margin - markerSize,
         pageHeight / 2 - margin - markerSize / 2},        // Right-middle
        {pageWidth / 2 - margin - markerSize / 2, margin}, // Bottom-middle
        {margin, pageHeight - margin - markerSize},        // Top-left corner
        {pageWidth - margin - markerSize,
         pageHeight - margin - markerSize}, // Top-right corner
        {pageWidth / 2 - margin - markerSize / 2,
         pageHeight - margin - markerSize} // Top-middle
    };

    for (size_t i = 0; i < markers.size(); ++i) {
      embedMarkerAtPosition(pdf, page, markers[i], positions[i].first,
                            positions[i].second);
    }

    const char *pdfFilename = "aruco_markers_corners.pdf";
    HPDF_SaveToFile(pdf, pdfFilename);
    std::cout << "PDF saved as " << pdfFilename << std::endl;

    HPDF_Free(pdf);
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
