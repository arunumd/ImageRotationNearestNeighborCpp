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

// Trivial constant
constexpr double Pi = 3.1415926535897932384626433832795;

/*!
* \brief Function to generate rotation matrix
* \param angle is the angle of rotation from user input
* \return rotation matrix
*/
cv::Mat CreateTransMat(double angle, std::pair<int, int> &pivot) {
	angle = Pi * angle / 180;
	return (cv::Mat_<double>(3, 3) << cos(angle), -sin(angle), pivot.first,
	        sin(angle), cos(angle), pivot.second, 0, 0, 1);
}

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
* \brief Function to transform an image based on a rotation angle and translation
         matrix. When rotation and translation happen at the same time, the
         two matrices can be combined
* \param src being source image
* \param dest being destination image
* \param trans_mat being the transformation (rotation/ translation) matrix
*/
void ImageTransform(const cv::Mat &src, const cv::Mat &trans_mat, cv::Mat &dest) {
	int src_rows = src.rows;
	int src_cols = src.cols;
	int dest_rows = dest.rows;
	int dest_cols = dest.cols;
	const cv::Mat inverse_mat = trans_mat.inv();
	//#pragma omp parallel for simd
	for (int row = 0; row < dest_rows; row++) {
		//#pragma omp parallel for simd
		for (int col = 0; col < dest_cols; col++) {
			cv::Mat src_pos = CoordTransform(inverse_mat,
			                                 (cv::Mat_<double>(3, 1) << col, row, 1));
			const int x_actual = static_cast<int>(src_pos.at<double>(0, 0) + 0.5);
			const int y_actual = static_cast<int>(src_pos.at<double>(0, 1) + 0.5);

			if (x_actual >= 0 && x_actual < src_cols &&
			        y_actual >= 0 && y_actual < src_rows)
				dest.at<cv::Vec3b>(row, col) = src.at<cv::Vec3b>(y_actual, x_actual);
			else
				dest.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
		}
	}
}

/*!
* \brief User manual for command-line args input
*/
void Usage() {
	std::cout << "COMMAND INPUT : - \n\n" <<
	          "          ./ImageTransform <input file name> <output file name> <rotation-angle>" <<
	          std::endl;
}

/*!
* \brief main function to read a user input location for an image, then apply the
*        required transformations (rotation / translation) and store the output
*        file name to the memory
*/
int main(int argc, char *argv[])
{
	for (auto i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;
	auto start = std::chrono::steady_clock::now();
	if (argc < 4)
		Usage();
	else {
		double degree = 0;
		if (argv[3] < 0) {
			degree = static_cast<double>(std::atof(argv[3]));
		}
		else
			degree = std::stod(argv[3]);
		double angle = degree * CV_PI / 180.;
		cv::Mat src_img = cv::imread(argv[1]);
		std::pair<int, int> null_trans = std::make_pair(0, 0);
		std::pair<int, int> translation_initial =
		    std::make_pair(src_img.cols / 2 + 1, src_img.rows / 2 + 1);
		std::pair<int, int> translation_final =
		    std::make_pair(0, -src_img.rows / 2 - 4);
		if (!src_img.data)
		{
			std::cout << "image null" << std::endl;
			cv::waitKey(0);
		}
		cv::imshow("Source", src_img);
		cv::Mat dest_img = cv::Mat(static_cast<int>(2 * src_img.rows),
		                           static_cast<int>(2 * src_img.cols),
		                           src_img.type());
		cv::Mat trans_mat1 = CreateTransMat(degree, translation_initial);
		ImageTransform(src_img, trans_mat1, dest_img);
		cv::imshow("Interim", dest_img);
		cv::Mat interim_img = dest_img;
		dest_img.release();
		dest_img = cv::Mat(src_img.rows, src_img.cols, src_img.type());
		cv::Mat trans_mat2 = CreateTransMat(0, translation_final);
		ImageTransform(interim_img, trans_mat2, dest_img);
		cv::imshow("Final image", dest_img);
		cv::imwrite(argv[2], dest_img);
		cv::waitKey(0);
	}
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << std::chrono::duration <double, std::milli> (diff).count() <<
	          " ms" << std::endl;
}
