// FeatureExtractor.cpp

#include "FeatureExtractor.h"

#include <vector>

FeatureExtractor::FeatureExtractor(bool useRootSift, int maxFeatures)
    : useRootSift_(useRootSift), sift_(cv::SIFT::create(maxFeatures)) {}

cv::Mat FeatureExtractor::extract(const cv::Mat& grayImage) const {
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    sift_->detectAndCompute(grayImage, cv::noArray(), keypoints, descriptors);

    if (descriptors.empty()) return cv::Mat(0, 128, CV_32F);

    descriptors.convertTo(descriptors, CV_32F);
    if (useRootSift_) toRootSift(descriptors);
    return descriptors;
}

void FeatureExtractor::toRootSift(cv::Mat& descriptors) {
    const float eps = 1e-7f;
    for (int r = 0; r < descriptors.rows; ++r) {
        cv::Mat row = descriptors.row(r);
        double l1 = cv::norm(row, cv::NORM_L1);  // SIFT values are non-negative
        row /= (l1 + eps);
        cv::sqrt(row, row);
    }
}
