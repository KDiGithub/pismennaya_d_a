#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cv.h>


using namespace std;
using namespace cv;

Mat src, srcnew, src_gray;
vector<KeyPoint> keypoints;
int perevod;

void find()
{
	src.copyTo(srcnew);
	/// Convert it to gray
	cvtColor(srcnew, src_gray, COLOR_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(src_gray, src_gray, Size(13, 13), 2, 2);
	medianBlur(src_gray, src_gray, 1);
	threshold(src_gray, src_gray, 30, 255, CV_THRESH_BINARY);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 5;
	params.maxThreshold = 200;

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs.
	detector->detect(src_gray, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	drawKeypoints(srcnew, keypoints, srcnew, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


}

void transl()
{
	

}


void on_trackbar(int, void*)
{
	find();
	// Show blobs
	imshow("image", srcnew);

	if (perevod == 1) transl();
}

int main(int argc, char *argv[]) {

	cv::String keys =
		"{pere |0| perevod n/y }"
		"{help || show help message}";

	CommandLineParser parser(argc, argv, keys);

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	string input_image_path="../../../images/braille/";
	string imim = "";
	cin >> imim;
	input_image_path += imim;

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	src = imread(input_image_path);

	namedWindow("image", 1);
	/// Create Trackbars
	char TrackbarName[50];
	sprintf(TrackbarName, "Translate", 1);
	createTrackbar(TrackbarName, "image", &perevod, 1, on_trackbar);

	/// Show some stuff
	on_trackbar(perevod, 0);

	waitKey(0);
	return 0;
}