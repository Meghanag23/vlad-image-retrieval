// HolidaysDataset.h
// Loads the INRIA Holidays images and knows which images belong together.
//
// Holidays file names encode the ground truth: images 100000.jpg, 100001.jpg,
// 100002.jpg ... show the same scene (group = id / 100), and the first image
// of each group (id % 100 == 0) is the query.

#pragma once

#include <map>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

struct ImageEntry {
    int id = 0;          // numeric file name, e.g. 100000 for 100000.jpg
    std::string name;    // file name, e.g. "100000.jpg"
    std::string path;    // full path to the image

    int group() const { return id / 100; }
    bool isQuery() const { return id % 100 == 0; }
};

class HolidaysDataset {
public:
    // Reads every .jpg in imageDir (sorted by id).
    // rotationFile is optional: each line is "<file name> <angle>", where angle
    // is 90, 180 or 270 degrees clockwise. Use it to fix images stored sideways.
    bool load(const std::string& imageDir, const std::string& rotationFile = "");

    const std::vector<ImageEntry>& images() const { return images_; }
    int size() const { return static_cast<int>(images_.size()); }

    // Indices (into images()) of all query images.
    std::vector<int> queryIndices() const;

    // Indices of the correct matches for a query: same group, query excluded.
    std::vector<int> relevantFor(int queryIndex) const;

    // Loads one image as grayscale, applies its rotation fix, and shrinks it so
    // the longer side is at most maxSize pixels (0 = keep original size).
    cv::Mat loadImage(int index, int maxSize) const;

private:
    std::vector<ImageEntry> images_;
    std::map<std::string, int> rotations_;
};
