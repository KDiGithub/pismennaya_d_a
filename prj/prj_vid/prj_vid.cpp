#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2\calib3d\calib3d.hpp>

#include <opencv2\core\core.hpp>
#include <opencv2\video.hpp>
#include "opencv2/video/background_segm.hpp"

#include <iostream>
#include <stdio.h>
#include <highgui.h>
#include <vector>
#include <cv.h>
#include <Windows.h>


using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {
	Mat frame_original, frame;

	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("original", 0, 0);

	/*cvNamedWindow("segmentation", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("segmentation", 500, 0);*/

	cvNamedWindow("stuff", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("stuff", 0, 300);

	string source = "../../../images/2.avi";
	//string source = "skate.avi";
	VideoCapture capture(source);
	//VideoCapture capture(0);

	string::size_type pAt = source.find_last_of('.');
	//const string NAME = source.substr(0, pAt) + "_stuff" + ".avi";
	const string NAME = "stuff.avi";
	int ex = static_cast<int>(capture.get(CV_CAP_PROP_FOURCC));
	char EXT[] = { (char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0 };
	Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));

	VideoWriter outputVideo;
	outputVideo.open(NAME, ex, capture.get(CV_CAP_PROP_FPS), S, true);

	if (!outputVideo.isOpened())
	{
		cout << "Could not open the output video for write" << endl;
		return -1;
	}
	//go
	while (1) {
		try {
			capture >> frame_original;
			capture >> frame;
			imshow("original", frame_original);
			pyrMeanShiftFiltering(frame, frame, 8, 30, 3);


			cv::Mat imageHsv;
			std::vector< std::vector<cv::Point> > contours;

			// Преобразуем в hsv, чтобы точнее вылавливать цве
			cv::cvtColor(frame, imageHsv, cv::COLOR_BGR2HSV);
			cv::Mat tmp_img(frame.size(), CV_8U);

			// Выделение подходящих по цвету областей 
			cv::inRange(imageHsv,
				cv::Scalar(0, 100, 100),
				cv::Scalar(255, 255, 255),
				tmp_img);

			//  Выделение непрерывных областей 
			cv::findContours(tmp_img, contours,
				CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

			//bitwise_not(tmp_img, tmp_img);
			Mat lol;
			frame.copyTo(lol);
			cvtColor(tmp_img, lol, CV_GRAY2BGR);
			/*frame.convertTo(frame, CV_8UC3);
			tmp_img.convertTo(tmp_img, CV_8UC3);*/

			lol = frame + lol;
        	
			//imshow("segmentation", frame);
			imshow("stuff", lol);

			outputVideo << lol;

			char c = cvWaitKey(33);
			if (c == 27) {
				break;
			}
		}
		catch (Exception ex) { break; };
	}
	capture.release();
	return 0;
}

