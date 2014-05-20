#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdio>

using namespace cv;
using namespace std;

int main( int argc,  char** argv ) {
	if ((argc < 2) || (argc > 3)) {
		fprintf(stderr, "ERROR: Incorrect number of args.");
		return -1;
	}

	//cout << "args:" << argc << "\n" << endl;
	//
	/* Read data from input and store it in img */
	//Mat img = imread(argv[1]);
	//
	
	Mat img[2];
	int i;
	
	for (i=1; i<argc; i++) {
		img[i-1] = imread(argv[1]);
	}

	/* Check if we actually loaded an image */
	if (img[0].empty()) {
		/* If we didn't, print an Error and terminate the program */
		cout << "Error : Image cannot be loaded...!!" << endl;
		return -1;
	}

	/* Create a new window to display the image in */
	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	/* Display the image in this window */
	imshow("Display Image", img[0]);

	/* Wait for a key press (infinite time) */
	waitKey(0);
    /* Destruct the window and terminate the program */
    destroyWindow("Display Image");

	return 0;
}