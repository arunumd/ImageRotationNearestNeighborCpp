# Image rotation and translation in C++ using Nearest neighbor interpolation
The project is related to Nearest neighbor interpolation technique for image rotation. We read the user input for rotation angle, and then create a transformation matrix. Later, we multiply the destination image coordinates with the inverse of this transformation matrix to get the corresponding source image coordinates. After some rounding off operation, we get the nearest coordinates. The project uses OpenCV only fo rmatrix manipulations and not for the nearest neighbor interpolation itself.

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
./ImageTransform <image> <rotation-angle>
```

# Output

** Original image **
![alt text](https://raw.githubusercontent.com/username/projectname/branch/path/to/output/original.png)

** Rotated image **
![alt text](https://raw.githubusercontent.com/username/projectname/branch/path/to/output/rotated.png)
