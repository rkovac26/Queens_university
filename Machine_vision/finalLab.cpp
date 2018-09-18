
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <windows.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
// OpenCV Imports
#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#define WINDOW_SIZE 11
#define SLIDE_DISTANCE 100

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
using namespace std::chrono;

void printWindow(int width, int height, Mat Window){
	for(int i = 0; i < width; i ++){
		for(int j = 0; j < height; j ++){
			cout << Window.at<int>(i,j) << ", ";
		}
		cout << endl;
	}
}

Mat blockMatch(Mat imA, Mat imB){
	int width = imA.cols;
	int lastPercent = 0;
	int height = imB.rows;
	int disparity = WINDOW_SIZE;
	int slideDistance = 0;
	double targetWindowMinError = DBL_MAX;

	double targetWindowError;
	//Sliding Windows
	Mat windowA(WINDOW_SIZE,WINDOW_SIZE,CV_8UC1);
	Mat windowB(WINDOW_SIZE,WINDOW_SIZE,CV_8UC1);
	Mat windowB_last = Mat::zeros(WINDOW_SIZE,WINDOW_SIZE,CV_8UC1);
	Mat disparityMat(height, width, CV_8UC1);
	//Set Up offsets
	int xOffset, yOffset, minErrorColIndex, slideWindowXOffset, slideMinOffset, slideMaxOffset;

	//Loop through height then width
	//If values out of bound dump in 0 to windowA Mat
	for(int i = 0; i < height; i++){ // Loop through Main Height
		if(lastPercent != (int)((((float)i) / 480.0) * 100)){
			cout << "Processing: " << (int)((((float)i) / 480.0) * 100) <<"%"  << endl;
		}
		lastPercent = (int)((((float)i) / 480.0) * 100);
		for(int j = 0; j < width; j++){ // Loop through Main Width
			//cout << "Width Index: " << j << endl;
			//Initialize A window (source)
			for(int k = 0; k < disparity; k++){ // Loop through window width
				xOffset = k - ((WINDOW_SIZE - 1) / 2); // Set offset in x dir
				for(int l = 0; l < disparity; l++){ // Loop through window height
					yOffset = l - ((WINDOW_SIZE - 1) / 2); // set offset in y dir
					if(j - xOffset < 0 || i - yOffset < 0){ // if out of bounds
						windowA.at<unsigned char>(k,l) = 0;
					}
					else{
						windowA.at<uchar>(l,k) = imA.at<uchar>(i - yOffset,j - xOffset);//BAD Global COORDS
					}
				}
			}
			/*Target += 15 sliding window code*/
			slideMinOffset = j - SLIDE_DISTANCE;
			slideMaxOffset = j + SLIDE_DISTANCE;

			if(slideMinOffset < 0){
				slideMinOffset = 0;
			}
			if(slideMaxOffset > width){
				slideMaxOffset = width;
			}

			//Start Slide
			for(int slideX = slideMinOffset; slideX < slideMaxOffset; slideX++){
				for(int slideXWindow = 0; slideXWindow < disparity; slideXWindow++){ // Loop through window width
					xOffset = slideXWindow - ((WINDOW_SIZE - 1) / 2); // Set offset in x dir
					for(int slideYWindow = 0; slideYWindow < disparity; slideYWindow++){ // Loop through window height
						yOffset = slideYWindow - ((WINDOW_SIZE - 1) / 2); // set offset in y dir
						if(i - yOffset < 0 || slideX - xOffset < 0){ // if out of bounds
							windowB.at<uchar>(slideXWindow,slideYWindow) = 0;
						}
						else{
							windowB.at<uchar>(slideYWindow,slideXWindow) = imB.at<uchar>(i - yOffset, slideX - xOffset);
						}
					}
				}

				//Compare Windows
				Mat diff(WINDOW_SIZE,WINDOW_SIZE,CV_8UC1);
				Mat diff_B(WINDOW_SIZE,WINDOW_SIZE,CV_8UC1);
				Mat diffBlownUp;
				//Get absolute difference between windows
				absdiff(windowA, windowB, diff);
				targetWindowError = sum(diff)[0];
				if(targetWindowError < targetWindowMinError){
					targetWindowMinError = targetWindowError;
					minErrorColIndex = slideX;
					slideDistance = abs(slideX - j);
				}
			}//End Slide
			if(j < 25){
				disparityMat.at<unsigned char>(i,j) = 50;
			} else{
				disparityMat.at<unsigned char>(i,j) = slideDistance;
			}

			targetWindowMinError = DBL_MAX;
			targetWindowError = DBL_MAX;
		}
	}
	cout << "Block Match Complete..."<< endl;
	return disparityMat;
}

void displayDisparity(Mat disparityMatrix, int height, int width){
	Mat disparityMatrix_Normalized(height, width, CV_8UC1);
	Mat disparityMatrix_Equalized(height, width, CV_8UC1);
	double min, max;

	//Write disparity matrix to jpg image
	imwrite("disparityMatrix.jpg", disparityMatrix);
	normalize(disparityMatrix, disparityMatrix_Normalized, 0, 255, NORM_MINMAX, CV_8UC1);
	equalizeHist(disparityMatrix, disparityMatrix_Equalized);
	cv::minMaxLoc(disparityMatrix, &min, &max);
	cout << "Showing Disparity Matrix - Width: "<<disparityMatrix.cols<<", Height: "<<disparityMatrix.rows<< ", MIN: "<< min <<", MAX: "<<max <<endl;
	//namedWindow("DIPARITY_MATRIX_NORMALIZED");
	namedWindow("DIPARITY_MATRIX_EQUALIZED");
	//Display Disparity Matrix
	//imshow("DIPARITY_MATRIX_NORMALIZED",disparityMatrix_Normalized);
	imshow("DIPARITY_MATRIX_EQUALIZED",disparityMatrix_Equalized);
	waitKey();


}

struct stereoParam{
	float baseline;
	float f;
	float doffs;
	float cx_cam0;
	float cx_cam1;
	float cy;
};

void threeDPointSet(Mat disparityMat, stereoParam params){
	ofstream points3D;
	points3D.open("points3D.txt");

	//No idea what to scale and what not
	int scaledBaseline = params.baseline * 1;
	int scaledF = params.f * 1;
	int scaledDoffs = params.doffs * 1;
	int scaledCx = params.cx_cam0 * 1;
	int scaledCy = params.cy * 1;

	//Is fx = fy = f ??

	//Output coordinates
	int x, y, z;
	//Loops through the disparity matrix
	for(int i = 0; i < disparityMat.cols; i++){
		for(int j = 0; j < disparityMat.rows; j++){
			//Apply formulas
			z = scaledBaseline * scaledF / ((int)disparityMat.at<uchar>(i,j) + scaledDoffs);
			x = ((i - scaledCx) * z) / scaledF;
			y = ((j - scaledCy) * z) / scaledF;
			points3D << x << ","<< y << "," << z << "\n";
			//cout << x << ","<< y << "," << z << "\n";
		}
	}
	points3D.close();

}


int main()
{
	cout << "...Final Lab..." << endl;;

	//Read in Two Perfect Images & Convert Images to Grayscale
	Mat pianoA = imread("Piano-perfect-im0-s.png",CV_LOAD_IMAGE_GRAYSCALE);
	Mat pianoB = imread("Piano-perfect-im1-s.png",CV_LOAD_IMAGE_GRAYSCALE);
	Mat disparitySaved = imread("disparityMatrix.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Print Image Sizes: 705 x 480
	cout << "Image width: " << pianoA.cols << ", Image height: " << pianoA.rows << endl;
	//Display Perfect Images with key press between
//	namedWindow("PERFECT_DISPLAYA",1);
//	namedWindow("PERFECT_DISPLAYB",1);
//	imshow("PERFECT_DISPLAYA",pianoA);
//	imshow("PERFECT_DISPLAYB",pianoB);
//	waitKey();

	//Start Block Matching Algorithm
	Mat disparityMatrix(pianoA.rows, pianoA.cols, CV_8UC1);

	//Wrap disparity function in time test
	milliseconds ms_start = duration_cast< milliseconds >(
			system_clock::now().time_since_epoch();
	)

	disparityMatrix = blockMatch(pianoA, pianoB);

	displayDisparity(disparityMatrix, pianoA.rows, pianoA.cols);

	//Initialize piano parameters
	stereoParam piano;
	piano.baseline = 178.089;
	piano.f = 2826.171;
	piano.doffs = 123.77;
	piano.cx_cam0 = 1292.29;
	piano.cx_cam1 = 1415.97;
	piano.cy = 965.806;

	threeDPointSet(disparitySaved, piano);
	return 0;
}


