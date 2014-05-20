#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;
using namespace boost::filesystem;

vector<Mat> create_transition(Mat im1, Mat im2, int num);

// The function takes in 2 image names as command-line arguments, then asks for
// the number of frames. It ouputs a series of images as the transition.
int main( int argc,  char** argv ) {
  if (argc != 3) {
    fprintf(stderr, "ERROR: ImTransistor requires 2 image filenames.\n");
    return -1;
  }

  Mat src1, src2;
  src1 = imread(argv[1]);
  src2 = imread(argv[2]);

  // Check if we actually loaded an image.
  if (src1.empty() || src2.empty()) {
    fprintf(stderr, "ERROR: Images could not be loaded.\n");
    return -1;
  }

  if (src1.size() != src2.size()) {
    fprintf(stderr, "ERROR: Images are not the same size!.\n");
    return -1;
  }

  // Get number of frames for transisition.
  int num;
  cout << "\nEnter number of frames: ";
  cin >> num;
  if (!num) {
    fprintf(stderr, "ERROR: Not a corrent integer input.\n");
    return -1;
  }

  Mat test;
  Canny(src1, test, 200, 201);
  imshow("Edges....", test);
  waitKey(0);

  // Create the transistion frames, including the originals in the frames.
  vector<Mat> frames(create_transition(src1, src2, num));
  // Needed cuz vectors are stupid and need iterators :P
  vector<Mat>::iterator it = frames.begin();
  it = frames.insert(it, src1);

  // Create a folder for the transistion frame images.
  if (!create_directory(current_path() / "Output")) {
    fprintf(stderr, "ERROR: Output folder could not be created.\n");
    return -1;
  }

  // Write the images.
  int i;
  for (i = 0; i < frames.size(); i++) {
    char filename[99];
    sprintf(filename, "Output/frame%d.jpg", i);
    if (!imwrite(filename, frames.at(i))) {
      fprintf(stderr, "ERROR: Output image could not be created.\n");
      return -1;
    }
  }

  // Ask is a video should be created.
  char ifyes;
  cout << "Create video along with frames (it might take a while) (currently "
       << "only works on a Mac)?\n"
       << "Y / N: ";
  cin.clear();
  cin >> ifyes;

  // Create a video from the frames if yes.
  if ((ifyes == 'Y') || (ifyes == 'y')) {
    cout << "Creating video...\n";

    VideoWriter outputVideo("Output/Output Video.mov", 
                            CV_FOURCC('m','p','4','v'), 20, src1.size(), true);
    if (!outputVideo.isOpened()) {
      fprintf(stderr, "ERROR: Video couldn't be created.\n");
      return -1;
    }
    for (i = 0; i < frames.size(); i++) {
      outputVideo.write(frames.at(i));
    }
    outputVideo.release();
  } 
  else
    cout << "Then, ";


  cout << "Done! Files are in the Output folder.\n\n";
  return 0;
}


// Made the frames for a blend transistion.
vector<Mat> create_transition(Mat im1, Mat im2, int num) {
  vector<Mat> frames(num);
  int i;
  double alpha, beta, step;
  step = 1.0 / (double) num;

  for (i = 0; i < frames.size(); i++) {
    beta += step;
    alpha = 1.0 - beta;
    addWeighted(im1, alpha, im2, beta, 0.0, frames.at(i));
  }

  return frames;
}



