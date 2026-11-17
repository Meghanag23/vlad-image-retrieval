# Revisiting VLAD: Compact Image Representations for Image Retrieval

CSS course project (UW Bothell) by **Meghana Reddy Gavinolla**.

This project implements VLAD (Vector of Locally Aggregated Descriptors) from
Jégou et al., *"Aggregating local descriptors into a compact image representation,"* CVPR 2010,
in C++ with OpenCV. It then extends the paper with **RootSIFT** and **power normalization**
and measures the effect of each improvement on the INRIA Holidays dataset.

## Pipeline

```
Images -> SIFT / RootSIFT -> k-means vocabulary -> VLAD encoding -> normalization -> nearest-neighbor search -> mAP
```

## Repository layout

| Folder | Contents |
| --- | --- |
| `src/` | C++ source files |
| `include/` | Class headers |
| `data/` | Dataset location (images are not committed; see `data/README.md`) |
| `results/` | Result tables and figures |
| `docs/` | Proposal, design, report, and the resource/AI log |

## Build

Requires CMake 3.10+, a C++17 compiler, and OpenCV 4.4+ (with opencv_contrib).

```bash
mkdir build && cd build
cmake ..
cmake --build .
./vlad_search selftest
```

## Usage

```bash
# Check that SIFT, RootSIFT and the feature cache work (no dataset needed)
./vlad_search selftest

# Extract RootSIFT features for all Holidays images and cache them
./vlad_search extract --images ../data/holidays --cache ../cache --rootsift --max-size 1024

# Optional: fix images stored sideways (each line: "<file name> <90|180|270>")
./vlad_search extract --images ../data/holidays --cache ../cache --rootsift --rotations ../data/rotations.txt
```

Cached files are named `<id>_<sift|rootsift>_<maxSize>.bin`, so different settings never mix.

## Status

- [x] Proposal (Nov 9)
- [x] Feature extraction + RootSIFT + feature cache (week of Nov 10)
- [ ] Vocabulary + VLAD encoder + mAP evaluator (design review, Nov 23)
- [ ] Bag-of-Words baseline + power normalization + ablation
- [ ] Demo video (Dec 14)
- [ ] Final report and code (Dec 16)

## Resource and AI log

All resources used, including AI prompts and responses, are listed in
[`docs/resource-log.md`](docs/resource-log.md).
