// FeatureCache.h
// Saves and loads descriptor matrices so SIFT only has to run once per image.
//
// Format (binary, little-endian): int rows, int cols, int type, then the raw data.
// Binary is used instead of cv::FileStorage here because text files would be
// very large and slow for ~1,500 images with thousands of descriptors each.

#pragma once

#include <string>

#include <opencv2/core.hpp>

namespace FeatureCache {

bool save(const std::string& path, const cv::Mat& descriptors);

// Returns an empty matrix if the file is missing or unreadable.
cv::Mat load(const std::string& path);

}  // namespace FeatureCache
