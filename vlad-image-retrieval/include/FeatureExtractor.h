// FeatureExtractor.h
// Computes SIFT descriptors for an image, with an optional RootSIFT step.

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

class FeatureExtractor {
public:
    // useRootSift: convert descriptors to RootSIFT (Arandjelovic & Zisserman, CVPR 2012).
    // maxFeatures: keep at most this many keypoints per image (0 = no limit).
    explicit FeatureExtractor(bool useRootSift = false, int maxFeatures = 0);

    // Returns an N x 128 CV_32F matrix, one row per keypoint.
    cv::Mat extract(const cv::Mat& grayImage) const;

    // RootSIFT: L1-normalize each row, then take the square root of every value.
    // After this, each row has L2 norm 1, and Euclidean distance between rows
    // matches the Hellinger kernel on the original SIFT descriptors.
    static void toRootSift(cv::Mat& descriptors);

    bool usesRootSift() const { return useRootSift_; }

private:
    bool useRootSift_;
    cv::Ptr<cv::SIFT> sift_;
};
