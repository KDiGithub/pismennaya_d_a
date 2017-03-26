#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;


int main(int argc,char* argv[])
{
	Mat Image = Mat(300, 400, CV_8U);
	rectangle(Image, Point(0, 0), Point(400, 100), Scalar(255.0), CV_FILLED);
	rectangle(Image, Point(0, 100), Point(400, 200), Scalar(128.0), CV_FILLED);
	rectangle(Image, Point(0, 200), Point(400, 300), Scalar(0.0), CV_FILLED);

	circle(Image, Point(50, 50), 25, Scalar(128.0), CV_FILLED);
	rectangle(Image, Point(125, 25), Point(175, 75), Scalar(128.0), CV_FILLED);
	circle(Image, Point(250, 50), 25, Scalar(0.0), CV_FILLED);
	rectangle(Image, Point(325, 25), Point(375, 75), Scalar(0.0), CV_FILLED);

	circle(Image, Point(50, 150), 25, Scalar(255.0), CV_FILLED);
	rectangle(Image, Point(125, 125), Point(175, 175), Scalar(255.0), CV_FILLED);
	circle(Image, Point(250, 150), 25, Scalar(0.0), CV_FILLED);
	rectangle(Image, Point(325, 125), Point(375, 175), Scalar(0.0), CV_FILLED);

	circle(Image, Point(50, 250), 25, Scalar(255.0), CV_FILLED);
	rectangle(Image, Point(125, 225), Point(175, 275), Scalar(255.0), CV_FILLED);
	circle(Image, Point(250, 250), 25, Scalar(128.0), CV_FILLED);
	rectangle(Image, Point(325, 225), Point(375, 275), Scalar(128.0), CV_FILLED);

	imshow("image", Image);


	Mat kernel1 = Mat(2, 2, CV_32F);
	kernel1.at<float>(0, 0) = 1;
	kernel1.at<float>(0, 1) = 0;
	kernel1.at<float>(1, 0) = 0;
	kernel1.at<float>(1, 1) = -1;
	Mat kernel2 = Mat(2, 2, CV_32F);
	kernel2.at<float>(0, 0) = 0;
	kernel2.at<float>(0, 1) = 1;
	kernel2.at<float>(1, 0) = -1;
	kernel2.at<float>(1, 1) = 0;

	Mat im1, im2;

	filter2D(Image, im1, CV_32F, kernel1);

	filter2D(Image, im2, CV_32F, kernel2);

	Mat im1_norm, im2_norm;

	normalize(im1, im1_norm, 0, 255, CV_MINMAX, CV_8U);
	normalize(im2, im2_norm, 0, 255, CV_MINMAX, CV_8U);

	imshow("image1", im1_norm);
	imshow("image2", im2_norm);

    waitKey(0);
    return 0;
}


