// HolidaysDataset.cpp

#include "HolidaysDataset.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace fs = std::filesystem;

static bool isJpeg(const fs::path& p) {
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return ext == ".jpg" || ext == ".jpeg";
}

bool HolidaysDataset::load(const std::string& imageDir, const std::string& rotationFile) {
    images_.clear();
    rotations_.clear();

    if (!fs::is_directory(imageDir)) {
        std::cerr << "Image folder not found: " << imageDir << std::endl;
        return false;
    }

    for (const auto& entry : fs::directory_iterator(imageDir)) {
        if (!entry.is_regular_file() || !isJpeg(entry.path())) continue;

        std::string stem = entry.path().stem().string();
        if (stem.empty() || !std::all_of(stem.begin(), stem.end(), ::isdigit)) {
            std::cerr << "Skipping file with non-numeric name: " << entry.path() << std::endl;
            continue;
        }

        ImageEntry img;
        img.id = std::stoi(stem);
        img.name = entry.path().filename().string();
        img.path = entry.path().string();
        images_.push_back(img);
    }

    std::sort(images_.begin(), images_.end(),
              [](const ImageEntry& a, const ImageEntry& b) { return a.id < b.id; });

    if (!rotationFile.empty()) {
        std::ifstream in(rotationFile);
        if (!in) {
            std::cerr << "Could not open rotation file: " << rotationFile << std::endl;
            return false;
        }
        std::string name;
        int angle = 0;
        while (in >> name >> angle) {
            if (angle == 90 || angle == 180 || angle == 270) rotations_[name] = angle;
        }
    }

    return !images_.empty();
}

std::vector<int> HolidaysDataset::queryIndices() const {
    std::vector<int> result;
    for (int i = 0; i < size(); ++i) {
        if (images_[i].isQuery()) result.push_back(i);
    }
    return result;
}

std::vector<int> HolidaysDataset::relevantFor(int queryIndex) const {
    std::vector<int> result;
    int group = images_.at(queryIndex).group();
    for (int i = 0; i < size(); ++i) {
        if (i != queryIndex && images_[i].group() == group) result.push_back(i);
    }
    return result;
}

cv::Mat HolidaysDataset::loadImage(int index, int maxSize) const {
    const ImageEntry& img = images_.at(index);

    // IMREAD_GRAYSCALE also applies the EXIF orientation tag if the file has one.
    cv::Mat gray = cv::imread(img.path, cv::IMREAD_GRAYSCALE);
    if (gray.empty()) return gray;

    auto rot = rotations_.find(img.name);
    if (rot != rotations_.end()) {
        if (rot->second == 90) cv::rotate(gray, gray, cv::ROTATE_90_CLOCKWISE);
        else if (rot->second == 180) cv::rotate(gray, gray, cv::ROTATE_180);
        else if (rot->second == 270) cv::rotate(gray, gray, cv::ROTATE_90_COUNTERCLOCKWISE);
    }

    int longer = std::max(gray.rows, gray.cols);
    if (maxSize > 0 && longer > maxSize) {
        double scale = static_cast<double>(maxSize) / longer;
        cv::resize(gray, gray, cv::Size(), scale, scale, cv::INTER_AREA);
    }
    return gray;
}
