// FeatureCache.cpp

#include "FeatureCache.h"

#include <fstream>

namespace FeatureCache {

bool save(const std::string& path, const cv::Mat& descriptors) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    cv::Mat data = descriptors.isContinuous() ? descriptors : descriptors.clone();
    int header[3] = {data.rows, data.cols, data.type()};
    out.write(reinterpret_cast<const char*>(header), sizeof(header));
    out.write(reinterpret_cast<const char*>(data.data),
              static_cast<std::streamsize>(data.total() * data.elemSize()));
    return static_cast<bool>(out);
}

cv::Mat load(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return cv::Mat();

    int header[3] = {0, 0, 0};
    in.read(reinterpret_cast<char*>(header), sizeof(header));
    if (!in || header[0] < 0 || header[1] <= 0) return cv::Mat();

    cv::Mat data(header[0], header[1], header[2]);
    in.read(reinterpret_cast<char*>(data.data),
            static_cast<std::streamsize>(data.total() * data.elemSize()));
    if (!in) return cv::Mat();
    return data;
}

}  // namespace FeatureCache
