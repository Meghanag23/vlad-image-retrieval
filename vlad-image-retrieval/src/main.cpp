// main.cpp
// Entry point for the VLAD image retrieval project.
//
// Modes:
//   selftest                      checks RootSIFT and the feature cache on a synthetic image
//   extract --images DIR --cache DIR [--rootsift] [--max-size N] [--rotations FILE]
//                                 runs SIFT (or RootSIFT) on every image and saves the descriptors
//
// Planned modes: build, search, evaluate.

#include <chrono>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "FeatureCache.h"
#include "FeatureExtractor.h"
#include "HolidaysDataset.h"

namespace fs = std::filesystem;

// ---------- small argument helper ----------
// Turns "--images data/holidays --rootsift" into {"images": "data/holidays", "rootsift": ""}.
static std::map<std::string, std::string> parseArgs(int argc, char** argv, int start) {
    std::map<std::string, std::string> args;
    for (int i = start; i < argc; ++i) {
        std::string key = argv[i];
        if (key.rfind("--", 0) != 0) continue;
        key = key.substr(2);
        if (i + 1 < argc && std::string(argv[i + 1]).rfind("--", 0) != 0) {
            args[key] = argv[++i];
        } else {
            args[key] = "";  // flag with no value
        }
    }
    return args;
}

static void printUsage() {
    std::cout << "Usage:\n"
              << "  vlad_search selftest\n"
              << "  vlad_search extract --images DIR --cache DIR [--rootsift] [--max-size N] [--rotations FILE]\n";
}

// ---------- selftest ----------
// Draws a synthetic image with shapes, extracts features, and checks that
// (1) SIFT finds keypoints, (2) every RootSIFT row has L2 norm 1, and
// (3) the cache saves and loads the exact same matrix.
static int runSelfTest() {
    cv::Mat img(480, 640, CV_8UC1, cv::Scalar(30));
    cv::RNG rng(42);
    for (int i = 0; i < 60; ++i) {
        cv::Point p(rng.uniform(0, 640), rng.uniform(0, 480));
        int r = rng.uniform(5, 40);
        cv::Scalar c(rng.uniform(60, 255));
        if (i % 2 == 0) cv::circle(img, p, r, c, cv::FILLED);
        else cv::rectangle(img, p, p + cv::Point(r, r / 2 + 5), c, cv::FILLED);
    }

    FeatureExtractor sift(false);
    FeatureExtractor rootSift(true);
    cv::Mat d1 = sift.extract(img);
    cv::Mat d2 = rootSift.extract(img);

    bool ok = true;
    std::cout << "SIFT descriptors: " << d1.rows << " x " << d1.cols << std::endl;
    if (d1.rows == 0 || d1.cols != 128) { std::cout << "FAIL: no SIFT descriptors\n"; ok = false; }
    if (d2.rows != d1.rows) { std::cout << "FAIL: RootSIFT row count differs\n"; ok = false; }

    double worst = 0.0;
    for (int r = 0; r < d2.rows; ++r) {
        worst = std::max(worst, std::abs(cv::norm(d2.row(r), cv::NORM_L2) - 1.0));
    }
    std::cout << "RootSIFT max |L2 norm - 1|: " << worst << std::endl;
    if (worst > 1e-3) { std::cout << "FAIL: RootSIFT rows are not unit length\n"; ok = false; }

    std::string tmp = (fs::temp_directory_path() / "vlad_selftest.bin").string();
    FeatureCache::save(tmp, d2);
    cv::Mat back = FeatureCache::load(tmp);
    fs::remove(tmp);
    bool same = back.size() == d2.size() && back.type() == d2.type() && cv::norm(back, d2, cv::NORM_INF) == 0.0;
    std::cout << "Cache round trip identical: " << (same ? "yes" : "no") << std::endl;
    if (!same) ok = false;

    std::cout << (ok ? "SELFTEST PASSED" : "SELFTEST FAILED") << std::endl;
    return ok ? 0 : 1;
}

// ---------- extract ----------
static int runExtract(const std::map<std::string, std::string>& args) {
    if (!args.count("images") || !args.count("cache")) {
        printUsage();
        return 1;
    }
    const std::string imageDir = args.at("images");
    const std::string cacheDir = args.at("cache");
    const bool useRootSift = args.count("rootsift") > 0;
    const int maxSize = args.count("max-size") ? std::stoi(args.at("max-size")) : 1024;
    const std::string rotationFile = args.count("rotations") ? args.at("rotations") : "";

    HolidaysDataset dataset;
    if (!dataset.load(imageDir, rotationFile)) {
        std::cerr << "No images loaded from " << imageDir << std::endl;
        return 1;
    }
    std::cout << "Loaded " << dataset.size() << " images, "
              << dataset.queryIndices().size() << " queries\n"
              << "Descriptor: " << (useRootSift ? "RootSIFT" : "SIFT")
              << ", max image size: " << maxSize << "\n";

    fs::create_directories(cacheDir);
    FeatureExtractor extractor(useRootSift);

    long long totalDescriptors = 0;
    int computed = 0, reused = 0, failed = 0;
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < dataset.size(); ++i) {
        const ImageEntry& img = dataset.images()[i];
        // The file name records the settings, so SIFT and RootSIFT (or different
        // image sizes) never overwrite or reuse each other's cache files.
        std::string cacheName = std::to_string(img.id) + (useRootSift ? "_rootsift_" : "_sift_") +
                                std::to_string(maxSize) + ".bin";
        std::string cachePath = (fs::path(cacheDir) / cacheName).string();

        cv::Mat desc = FeatureCache::load(cachePath);
        if (!desc.empty()) {
            ++reused;
        } else {
            cv::Mat gray = dataset.loadImage(i, maxSize);
            if (gray.empty()) {
                std::cerr << "Could not read " << img.path << std::endl;
                ++failed;
                continue;
            }
            desc = extractor.extract(gray);
            FeatureCache::save(cachePath, desc);
            ++computed;
        }
        totalDescriptors += desc.rows;

        if ((i + 1) % 100 == 0 || i + 1 == dataset.size()) {
            std::cout << "  " << (i + 1) << " / " << dataset.size() << " images\n";
        }
    }

    double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
    int done = computed + reused;
    std::cout << "\nDone in " << seconds << " s\n"
              << "  computed: " << computed << ", loaded from cache: " << reused
              << ", failed: " << failed << "\n"
              << "  total descriptors: " << totalDescriptors << "\n"
              << "  average per image: " << (done ? totalDescriptors / done : 0) << "\n";
    return failed == 0 ? 0 : 1;
}

int main(int argc, char** argv) {
    std::cout << "VLAD image retrieval (OpenCV " << CV_VERSION << ")" << std::endl;

    if (argc < 2) {
        printUsage();
        return 0;
    }

    std::string mode = argv[1];
    auto args = parseArgs(argc, argv, 2);

    if (mode == "selftest") return runSelfTest();
    if (mode == "extract") return runExtract(args);

    std::cout << "Unknown mode: " << mode << std::endl;
    printUsage();
    return 1;
}
