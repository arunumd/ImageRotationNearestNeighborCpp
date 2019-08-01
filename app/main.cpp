/*!
 * \file        main.cpp
 * \author      Arun Kumar Devarajulu
 * \date        08/01/2019
 * \brief       The file main.cpp contains the code for rotation of an input
                image with nearest neighbour interpolation without using OpenCV 
                inbuilt functions
 * \license     This project is released under the MIT License.
 */
#include <iostream>
#include <cmath>
#include <utility>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

const double Pi = 3.1415926535897932384626433832795;

/*!
* \brief Function to generate rotation matrix
* \param angle is the angle of rotation from user input
* \return rotation matrix
*/
cv::Mat CreateRotMat(double angle) {
	angle = Pi * angle / 180;
	return (cv::Mat_<double>(3, 3) << cos(angle), -sin(angle), 0,
	        sin(angle), cos(angle), 0, 0, 0, 1);
}

//cv::Mat CreateTransMat() {}

/*!
* \brief Function to apply coordinate transform from destination to source
* \param inv_mat being the inverse transformation matrix for the transform needed
* \return pos being the homogeneous coordinates for transformation
*/
cv::Mat CoordTransform(const cv::Mat &inv_mat, const cv::Mat &pos) {
	assert(inv_mat.cols == pos.rows);
	cv::Mat trans_mat = inv_mat * pos;
	return (cv::Mat_<double>(1, 2) <<
	        trans_mat.at<double>(0, 0) / trans_mat.at<double>(0, 2),
	        trans_mat.at<double>(0, 1) / trans_mat.at<double>(0, 2));
}

/*!
* \brief Function to rotate / translate an input image by an angle
* \param src being source image
* \param dest being destination image
* \rot_mat being the rotation matrix
*/
void ImageTransform(const cv::Mat &src, const cv::Mat &rot_mat, cv::Mat &dest) {
	int dest_rows = dest.rows;
	int dest_cols = dest.cols;
	cv::Mat inverse_mat = rot_mat.inv();
	for (int row = 0; row < dest_rows; row++) {
		for (int col = 0; col < dest_cols; col++) {
			cv::Mat src_pos = CoordTransform(inverse_mat,
			                                 (cv::Mat_<double>(3, 1) << col, row, 1));
			const int x_actual = static_cast<int>(src_pos.at<double>(0, 0) + 0.5);
			const int y_actual = static_cast<int>(src_pos.at<double>(0, 1) + 0.5);

			if (x_actual >= 0 && x_actual < dest_cols &&
			        y_actual >= 0 && y_actual < dest_rows)
				dest.at<cv::Vec3b>(row, col) = src.at<cv::Vec3b>(y_actual, x_actual);
			else
				dest.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
		}
	}
}

/*!
* \brief main function to read a user input location for an image and then apply the 
         required transformations (rotation / translation)
*/
int main()
{
	double degree = 70;
	double angle = degree * CV_PI / 180.;

	cv::Mat src_img = cv::imread("../app/lena.png");

	std::pair<int, int> translation_initial =
	    std::make_pair(src_img.cols / 2 + 1, src_img.rows / 2 + 1);

	std::pair<int, int> translation_final =
	    std::make_pair(-src_img.cols / 2 + 1, -src_img.rows / 2 + 1);

	if (!src_img.data)
	{
		std::cout << "image null" << std::endl;
		cv::waitKey(0);
	}

	cv::Mat dest_img = cv::Mat(src_img.rows, src_img.cols, src_img.type());
	cv::Mat rot_mat = CreateRotMat(degree);
	ImageTransform(src_img, rot_mat, dest_img);

	cv::imshow("Rotated image", dest_img);

	cv::waitKey(0);
}
