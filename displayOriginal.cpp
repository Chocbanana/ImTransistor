#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc,  char** argv ) {
	if (argc != 2) {
		printf("Incorrect number of args.");
	}

	/* Read data from input and store it in img */
	Mat img = imread(argv[1]);

	/* Check if we actually loaded an image */
	if (img.empty()) {
		/* If we didn't, print an Error and terminate the program */
		cout << "Error : Image cannot be loaded..!!" << endl;
		return -1;
	}

	/* Create a new window to display the image in */
	namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
	/* Display the image in this window */
	imshow("Display Image", img);

	/* Wait for a key press (infinite time) */
	waitKey(0);

    /* Destruct the window and terminate the program */
    destroyWindow("DispWin");
	return 0;
}