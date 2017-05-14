#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cv.h>
#include <cmath>


using namespace std;
using namespace cv;

class Insidee
{
public:
	int lett[3][2];
	void lettnew(int let00, int let01, int let10, int let11, int let20, int let21)   //ary[3][2])
	{
		lett[0][0] = let00; lett[0][1] = let01;
		lett[1][0] = let10; lett[1][1] = let11;
		lett[2][0] = let20; lett[2][1] = let21;
	}
	string bukva;
	string chislo;
};

Mat src, srcnew, src_gray, srcnew2;
vector<KeyPoint> keypoints;
vector<Insidee> maybeletters;
int perevod;

vector<Insidee> letters; 
bool response_comparator(KeyPoint& p1, KeyPoint& p2) {
	if ( abs(p1.pt.y - p2.pt.y) <1)
		return p1.pt.x > p2.pt.x;
	else return p1.pt.y > p2.pt.y;
}

bool sravnenie(Insidee p1, Insidee p2)
{
	int rav = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (p1.lett[i][j] == p2.lett[i][j]) rav++;
		}
	}
	if (rav == 6) return true;
	else return false;
}

void find()
{

	/// Convert it to gray
	cvtColor(src, srcnew, COLOR_BGR2GRAY);
	cv::Canny(srcnew, src_gray, 20, 40, 3);
	threshold(srcnew, srcnew2, 20, 200, CV_THRESH_BINARY);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;
	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;
	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	// Detect blobs.
	detector->detect(src_gray, keypoints);
	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	drawKeypoints(srcnew, keypoints, srcnew, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}

void transl()
{
	int boxes = 0;
	while (keypoints.size() != 0)
	{
		maybeletters.resize(boxes + 1);
		vector<KeyPoint> key;

		//èùåì êðàé
		double minx = 100000, miny = 100000, betx = 100000, bety = 100000;
		//int k = 0, l = 0, ki = 0, li = 0;
		for (int i = 0; i < keypoints.size(); i++)
		{
			if (keypoints[i].pt.y < miny+1)
			{
				if (keypoints[i].pt.y < miny - 10)
				{
					minx = 100000;
				}
				miny = keypoints[i].pt.y;
				if (keypoints[i].pt.x < minx)
				{
					minx = keypoints[i].pt.x;
				}
			}

			for (int j = keypoints.size() - 1; j > i; j--)
			{
				double bx = abs(floor(keypoints[i].pt.x) - floor(keypoints[j].pt.x));
				double by = abs(floor(keypoints[i].pt.y) - floor(keypoints[j].pt.y));
				if (bx < betx && bx >= keypoints[i].size) {
					betx = bx;
					/*k = j;
					ki = i;*/
				}
				if (by < bety && by > keypoints[i].size) {
					bety = by;
					/*l = j;
					li = i;*/
				}
			}
		}

		//èùåì ïðÿìîóãîëüíèê
		int ne = 0;
		double krx = 0, kry = 0;
		for (int i = 0; i < keypoints.size(); i++)
		{
			double bx = abs(floor(keypoints[i].pt.x) - minx);
			double by = abs(floor(keypoints[i].pt.y) - miny);
			if (bx < betx * 2 + 5 && by < bety * 2 + 5)
			{
				key.resize(ne + 1);
				key[ne] = keypoints[i];
				ne++;
				//krx = keypoints[i].pt.x;
				//kry = keypoints[i].pt.y;
			}
		}
		
		//çàïèñü â êëàññ 1 èëè 0
		int indx = 1, indy = 2;
		sort(key.begin(), key.end(), response_comparator);
		for (int m = 0; m < key.size(); m++)
		{
			int xx = key[m].pt.x / 1, yy = key[m].pt.y / 1;
			double wat = srcnew2.at<unsigned char>(Point(xx, yy));
			if (wat < 50)
			{
				maybeletters[boxes].lett[indy][indx] = 1;
			}
			else
			{
				maybeletters[boxes].lett[indy][indx] = 0;
			}

			if (indx == 0)
			{
				indy--;
				indx = 1;
			}
			else indx--;
		}

		for (int qq = 0; qq < key.size(); qq++)
		{
			for (int q = 0; q < keypoints.size(); q++)
			{
				if (key[qq].pt.x == keypoints[q].pt.x && key[qq].pt.y == keypoints[q].pt.y)
				{
					keypoints.erase(keypoints.begin() + q);
					//vector<KeyPoint>(keypoints).swap(keypoints);
					break;
				}
			}
		}

		boxes++;

		drawMarker(srcnew, Point(minx, miny), Scalar(0, 255, 0), 0, 5, 1, 8);
		drawKeypoints(srcnew, key, srcnew, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	}

	string result = "";
	int num = 0;
	for (int i = 0; i < maybeletters.size(); i++)
	{
		for (int j = 0; j < letters.size(); j++)
		{
			if (sravnenie(letters[j], maybeletters[i]))
			{
				if (j == 26) num = 1;
				if (j == 27) {
					num = 0;
					result += " ";
					break;
				}
				if (num==1) result += letters[j].chislo;
				else result += letters[j].bukva;
				break;
			}
		}
	}
	cout << "" << endl;
	cout << "ANSWER:"<<endl;
	cout << result;

}


void on_trackbar(int, void*)
{
	find();
	// Show blobs
	//imshow("image2", srcnew2);
	//imshow("image1", src_gray);
	if (perevod == 1) transl();
	imshow("image", srcnew);
}

int main(int argc, char *argv[]) {
	letters.resize(28);
	letters[0].lettnew(1, 0, 0, 0, 0, 0); letters[0].bukva = "a"; letters[0].chislo = "1";
	letters[1].lettnew(1, 0, 1, 0, 0, 0); letters[1].bukva = "b"; letters[1].chislo = "2";
	letters[2].lettnew(1, 1, 0, 0, 0, 0); letters[2].bukva = "c"; letters[2].chislo = "3";
	letters[3].lettnew(1, 1, 0, 1, 0, 0); letters[3].bukva = "d"; letters[3].chislo = "4";
	letters[4].lettnew(1, 0, 0, 1, 0, 0); letters[4].bukva = "e"; letters[4].chislo = "5";
	letters[5].lettnew(1, 1, 1, 0, 0, 0); letters[5].bukva = "f"; letters[5].chislo = "6";
	letters[6].lettnew(1, 1, 1, 1, 0, 0); letters[6].bukva = "g"; letters[6].chislo = "7";
	letters[7].lettnew(1, 0, 1, 1, 0, 0); letters[7].bukva = "h"; letters[7].chislo = "8";
	letters[8].lettnew(0, 1, 1, 0, 0, 0); letters[8].bukva = "i"; letters[8].chislo = "9";
	letters[9].lettnew(0, 1, 1, 1, 0, 0); letters[9].bukva = "j"; letters[9].chislo = "0";
	letters[10].lettnew(1, 0, 0, 0, 1, 0); letters[10].bukva = "k";
	letters[11].lettnew(1, 0, 1, 0, 1, 0); letters[11].bukva = "l";
	letters[12].lettnew(1, 1, 0, 0, 1, 0); letters[12].bukva = "m";
	letters[13].lettnew(1, 1, 0, 1, 1, 0); letters[13].bukva = "n";
	letters[14].lettnew(1, 0, 0, 1, 1, 0); letters[14].bukva = "o";
	letters[15].lettnew(1, 1, 1, 0, 1, 0); letters[15].bukva = "p";
	letters[16].lettnew(1, 1, 1, 1, 1, 0); letters[16].bukva = "q";
	letters[17].lettnew(1, 0, 1, 1, 1, 0); letters[17].bukva = "r";
	letters[18].lettnew(0, 1, 1, 0, 1, 0); letters[18].bukva = "s";
	letters[19].lettnew(0, 1, 1, 1, 1, 0); letters[19].bukva = "t";
	letters[20].lettnew(1, 0, 0, 0, 1, 1); letters[20].bukva = "u";
	letters[21].lettnew(1, 0, 1, 0, 1, 1); letters[21].bukva = "v";
	letters[22].lettnew(0, 1, 1, 1, 0, 1); letters[22].bukva = "w";
	letters[23].lettnew(1, 1, 0, 0, 1, 1); letters[23].bukva = "x";
	letters[24].lettnew(1, 1, 0, 1, 1, 1); letters[24].bukva = "y";
	letters[25].lettnew(1, 0, 0, 1, 1, 1); letters[25].bukva = "z";
	letters[26].lettnew(0, 1, 0, 1, 1, 1); 
	letters[27].lettnew(0, 0, 0, 0, 0, 0);

	cout << "Filename: ";
again:
	string input_image_path="../../../images/braille/";
	string imim = "";
	cin >> imim;
	input_image_path += imim;

	src = imread(input_image_path);
	if (!src.data)
	{
		cout << "ERROR, can't find this file. (You must write name.format, for example hilu.jpg)"<<endl;
		cout << "Try again:";
		goto again;
	}

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

