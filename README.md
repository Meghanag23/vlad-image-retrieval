# Revisiting VLAD: Compact Image Representation for Image Retrieval in OpenCV

Course project by Meghana Reddy Gavinolla.

This project implements VLAD (Jégou et al., CVPR 2010) in C++ with OpenCV,
and extends it with RootSIFT and power normalization, evaluated on the
INRIA Holidays dataset.

## Build (macOS)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./vlad_search
```
