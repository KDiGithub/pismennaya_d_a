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

String save_image_path;
int percent = 5;
const int percent_slider_max = 90;
int track_percent;
Mat img, img2;

vector<uint32_t> calc_hist(Mat& img2)
{
	vector<uint32_t> hist(256);
	for (int y = 0; y < img2.rows; y++) {
		uint8_t* img2_line = img2.ptr<uint8_t>(y);
		for (int x = 0; x < img2.cols; x++) {
			hist[img2_line[x]]++;
		}
	}
	return hist;
}

int index(int del, int k, vector<uint32_t> hist)
{
	double sum = 0;
	if (k == 0)
	{
		while (sum <= del)
		{
			sum = sum + hist[k];
			k++;
		}
		k--;
	}

	if (k == 255)
	{
		while (sum <= del)
		{
			sum = sum + hist[k];
			k--;
		}
		k++;
	}
	return k;
}

int max_min(int u, int sum_m[])
{
	int max_min = 0;
	if (u == 0)
	{
		max_min = sum_m[0];
		for (int i = 1; i < 3; i++)
		{
			if (sum_m[i] > max_min) max_min = sum_m[i];
		}
	}

	if (u == 1)
	{

		max_min = sum_m[0];
		for (int i = 1; i < 3; i++)
		{
			if (sum_m[i] < max_min) max_min = sum_m[i];
		}
	}

	return max_min;
}


void contrast()
{
	vector<Mat> bgr_planes;
	img.copyTo(img2);
	split(img2, bgr_planes);

	auto b_hist = calc_hist(bgr_planes[0]);
	auto g_hist = calc_hist(bgr_planes[1]);
	auto r_hist = calc_hist(bgr_planes[2]);

	int del = img2.rows*img2.cols * percent / 100;

	int sum_min[3] = { index(del, 0, b_hist), index(del, 0, g_hist),index(del, 0, r_hist) };
	int sum_max[3] = { index(del, 255, b_hist), index(del, 255, g_hist), index(del, 255, r_hist) };

	int min = max_min(1, sum_min);
	int max = max_min(0, sum_max);

	for (int i = 0; i < img2.rows; i++)
	{
		for (int j = 0; j < img2.cols; j++)
		{
			double fake = 0;

			for (int chan = 0; chan < 3; chan++)
			{
				if (img2.at<Vec3b>(i, j)[chan] < min)
				{
					img2.at<Vec3b>(i, j)[chan] = 0;
				}
				else if (img2.at<Vec3b>(i, j)[chan] > max)
				{
					img2.at<Vec3b>(i, j)[chan] = 255;
				}
				else
				{
					fake = (img2.at<Vec3b>(i, j)[chan] - min) * 255 / (double)(max - min); //I'
					img2.at<Vec3b>(i, j)[chan] = int(fake);
				}
			}

		}
	}

}


void on_trackbar(int, void*)
{
	contrast();
	cout << percent << endl;
	imshow("Contrasted image", img2);
	imwrite(save_image_path, img2);
}

int main(int argc, char *argv[]) {

	cv::String keys =
		"{path |../../../images/p4hist.jpg | path to file}"
		"{save |../../../images/p4hist_new.jpg | path for saving file}"
		"{percent |5| number }"
		"{help || show help message}";

	CommandLineParser parser(argc, argv, keys);

	if (parser.has("help")) {
		parser.printMessage();
		return 0;
	}

	String input_image_path = parser.get<String>("path"); 
	save_image_path = parser.get<String>("save");
	percent = parser.get<float>("percent");
	cout << input_image_path << endl;
	cout << percent << endl;

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	img = imread(input_image_path);
	
	namedWindow("Contrasted image", 1);
	/// Create Trackbars
	char TrackbarName[50];
	sprintf(TrackbarName, "Percent", percent_slider_max);
	createTrackbar(TrackbarName, "Contrasted image", &percent, percent_slider_max, on_trackbar);

	/// Show some stuff
	on_trackbar(percent, 0);

	imshow("Contrasted image", img2);

	waitKey(0);
	return 0;
}
