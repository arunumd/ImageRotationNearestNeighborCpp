[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Image Rotation and Translation in C++ using Nearest Neighbor Interpolation
The project is related to Nearest neighbor interpolation technique for image rotation. We read the user input for rotation angle, and then create a transformation matrix. Later, we multiply the destination image coordinates with the inverse of this transformation matrix to get the corresponding source image coordinates. After some rounding off operation, we get the nearest coordinates. The project uses OpenCV only for rmatrix manipulations and not for the nearest neighbor interpolation itself.

# Assumptions
 - You have OpenCV version 3 or greater on your machine
 - You have a C++ 11/14 compiler
 - You have the CMake build system

# Author
Arun Kumar Devarajulu - University of Maryland, College Park, MD, USA

# License
This project is released under the MIT License

# Build & Run Instructions
```
git clone https://github.com/arunumd/ImageRotationNearestNeighborCpp.git
cd ImageRotationNearestNeighborCpp
mkdir build
cd build
cmake .. && make
./ImageTransform <input file name> <output file name> <rotation-angle>
```

# Output

| Original image | Rotated image |
| --- | --- |
| ![alt text](https://raw.githubusercontent.com/arunumd/ImageRotationNearestNeighborCpp/master/output/original.png) | ![alt text](https://raw.githubusercontent.com/arunumd/ImageRotationNearestNeighborCpp/master/output/rotated.png) |
