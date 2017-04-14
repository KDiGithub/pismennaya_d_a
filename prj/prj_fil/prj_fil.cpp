#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <highgui.h>
#include <vector>
#include <cv.h>


using namespace std;
using namespace cv;

Mat img, img5;
int Att[6];

void filter()
{
	Mat img1, img2, img3, img4;
	int fli = Att[0];
	int color = Att[1]; 
	int mblur = Att[2];
	int scalinga = Att[3];
	int scalingb = Att[4];
	int other = Att[5];
	int a = Att[6];

	
	img.copyTo(img1);
	if (fli!=0) flip(img, img1, fli-2);

	img1.copyTo(img2);
	if (color == 1)
	{
		cvtColor(img1, img2, COLOR_BGR2GRAY);
	}
	if ((mblur+1) % 2 == 0) mblur += 1;

	medianBlur(img2, img3, mblur+1);

	img3.convertTo(img4, -1, (scalinga+1), scalingb-100);

	/*Mat kernel_sobelx = Mat(3, 3, CV_32F);
	kernel_sobelx.at<float>(0, 0) = -1; kernel_sobelx.at<float>(0, 1) = 0; kernel_sobelx.at<float>(0, 2) = 1;
	kernel_sobelx.at<float>(1, 0) = -2; kernel_sobelx.at<float>(1, 1) = 0; kernel_sobelx.at<float>(1, 2) = 2;
	kernel_sobelx.at<float>(2, 0) = -1; kernel_sobelx.at<float>(2, 1) = 0; kernel_sobelx.at<float>(2, 2) = 1;
	Mat kernel_sobely = Mat(3, 3, CV_32F);
	kernel_sobely.at<float>(0, 0) = 1; kernel_sobely.at<float>(0, 1) = 2; kernel_sobely.at<float>(0, 2) = 1;
	kernel_sobely.at<float>(1, 0) = 0; kernel_sobely.at<float>(1, 1) = 0; kernel_sobely.at<float>(1, 2) = 0;
	kernel_sobely.at<float>(2, 0) = -1; kernel_sobely.at<float>(2, 1) = -2; kernel_sobely.at<float>(2, 2) = -1;
	*/Mat kernel_laplacian = Mat(3, 3, CV_32F);
	kernel_laplacian.at<float>(0, 0) = 0; kernel_laplacian.at<float>(0, 1) = 1; kernel_laplacian.at<float>(0, 2) = 0;
	kernel_laplacian.at<float>(1, 0) = 1; kernel_laplacian.at<float>(1, 1) = -4-a; kernel_laplacian.at<float>(1, 2) = 1;
	kernel_laplacian.at<float>(2, 0) = 0; kernel_laplacian.at<float>(2, 1) = 1; kernel_laplacian.at<float>(2, 2) = 0;

	img4.copyTo(img5);
	
	if (other != 0)
	{
		
		filter2D(img4, img5, CV_32F, kernel_laplacian);
		//bitwise_not(img4, img4);
		//if (color == 1) cvtColor(img5, img5, CV_GRAY2BGR);
		if (other==2) normalize(img5, img5, 0, 255, CV_MINMAX, CV_8U);
		if (other == 3)
		{
			if (color == 1)
			{
				cvtColor(img4, img4, CV_GRAY2BGR);
				cvtColor(img5, img5, CV_GRAY2BGR);
				
			}
			img4.convertTo(img4, CV_8UC3);
			img5.convertTo(img5, CV_8UC3);

			img5 = img4+img5;
		}
		
	}
	
	//else if (other == 2) filter2D(img3g, img4, CV_32F, kernel_sobelx);
	//else if (other == 3) filter2D(img3g, img4, CV_32F, kernel_sobely);
}


void on_trackbar(int, void*)
{
	filter();
	imshow("Updates", img5);
}

int main(int argc, char *argv[]) {

	cv::String keys =
		"{path |../../../images/pl.jpg | path to file}"
		"{help || show help message}";

	CommandLineParser parser(argc, argv, keys);

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	String input_image_path = parser.get<String>("path"); 
	//save_image_path = parser.get<String>("save");
	//mblur = parser.get<int>("mblur");
	cout << input_image_path << endl;

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	img = imread(input_image_path);
	
	//int Att[7];
	Att[0] = 0; Att[1] = 0; Att[2] = 0; Att[3] = 0; 
	Att[4] = 100; Att[5] = 0; Att[6] = 0; Att[7] = 0;

	namedWindow("Filters", 1);
	/// Create Trackbars
	char TrackbarName[50];
	sprintf(TrackbarName, "flip", 3);
	createTrackbar(TrackbarName, "Filters", &Att[0], 3, on_trackbar);
	sprintf(TrackbarName, "color", 1);
	createTrackbar(TrackbarName, "Filters", &Att[1], 1, on_trackbar);
	sprintf(TrackbarName, "mblur", 10);
	createTrackbar(TrackbarName, "Filters", &Att[2], 10, on_trackbar);
	sprintf(TrackbarName, "scaling a", 3);
	createTrackbar(TrackbarName, "Filters", &Att[3], 3, on_trackbar);
	sprintf(TrackbarName, "scaling b", 201);
	createTrackbar(TrackbarName, "Filters", &Att[4], 201, on_trackbar);
	sprintf(TrackbarName, "other", 3);
	createTrackbar(TrackbarName, "Filters", &Att[5], 3, on_trackbar);
	sprintf(TrackbarName, "center", 10);
	createTrackbar(TrackbarName, "Filters", &Att[6], 10, on_trackbar);

	/// Show some stuff
	on_trackbar(Att[0], 0);


   // imshow("Original", img);

	waitKey(0);
	return 0;
}