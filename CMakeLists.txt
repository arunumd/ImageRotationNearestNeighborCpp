#Set your CMake Minimum requirement
cmake_minimum_required(VERSION 3.2.1)

#Define your project
project (ImageTransform)

#Find required packages
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
#set(OpenCV_LIBS /usr/local/lib/cmake/opencv4)
set(NAME_SRC app/main.cpp)

include(CMakeToolsHelpers OPTIONAL)

#Add executables
add_executable(ImageTransform ${NAME_SRC})

#Link libraries
target_link_libraries(ImageTransform ${OpenCV_LIBS})
