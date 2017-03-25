#include <opencv2/opencv.hpp>

#include <iostream>
using namespace std;
using namespace cv;

int main() {

	try {
		string input_path = "../../../images/polaroid.jpg";
		Mat img = imread(input_path);
		Mat img2 = imread(input_path);
		Mat img3 = imread(input_path);

		cvNamedWindow("original", WINDOW_NORMAL);
		cvNamedWindow("darker", WINDOW_NORMAL);
		cvNamedWindow("brighter", WINDOW_NORMAL);

		resizeWindow("original", 250, 300);
		resizeWindow("darker", 250, 300);
		resizeWindow("brighter", 250, 300);

		moveWindow("original", 0, 0);
		moveWindow("darker", 0, 350);
		moveWindow("brighter", 300, 350);

		img.convertTo(img2, -1, 0.5, 0); //decrease the contrast (double)
		img.convertTo(img3, -1, 2, 1); //increase the contrast (double)

		imshow("original", img);
		imshow("darker", img2);
		imshow("brighter", img3);
		waitKey(0);
	}
	catch (exception &e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}
	return 0;
}