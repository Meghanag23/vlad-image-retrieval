// main.cpp
// Entry point for the VLAD image retrieval project.

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

int main() {
    std::cout << "VLAD image retrieval" << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // Quick check that SIFT is available in this OpenCV build.
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
    std::cout << "SIFT created: " << (sift ? "yes" : "no") << std::endl;

    return 0;
}
