#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;
using namespace boost::filesystem;

// The functor for sorting
struct sorter {
    Point center;
    sorter(Point cntr) : center(cntr) {};

    bool operator() (Point pt1, Point pt2) { 
      return (norm(pt1 - center) > norm(pt2 - center));
    }
};



void get_images(int argc,  char** argv, Mat *src1, Mat *src2);
vector<Mat> gradient_transition(Mat im1, Mat im2, int num);
vector<Mat> move_transition(Mat im1, Mat im2, int num);



// The function takes in 2 image names as command-line arguments, then asks for
// the number of frames. It ouputs a series of images as the transition.
int main( int argc, char** argv ) {

  Mat src1, src2; // Source images
  get_images(argc, argv, &src1, &src2); // Does the error checking.

  // Get number of frames for transisition.
  int num;
  cout << "\nEnter number of frames: ";
  cin >> num;
  if (!num) {
    fprintf(stderr, "ERROR: Not a corrent integer input.\n");
    return -1;
  }

  // Mat test;
  // Canny(src1, test, 200, 201);
  // imshow("Edges", test);
  // waitKey(0);
  // destroyWindow("Edges");


  // Create the transistion frames, including the originals in the frames.
  vector<Mat> frames(move_transition(src1, src2, num));


  // Create a folder for the transistion frame images.
  if (!create_directory(current_path() / "Output")) {
    fprintf(stderr, "ERROR: Output folder could not be created.\n");
    return -1;
  }

  // Write the images.
  for (int i = 0; i < frames.size(); i++) {
    char filename[99];
    sprintf(filename, "Output/frame%d.jpg", i);
    if (!imwrite(filename, frames.at(i))) {
      fprintf(stderr, "ERROR: Output image could not be created.\n");
      return -1;
    }
  }

  // Ask if a video should be created.
  char ifyes;
  cout << "Create video along with frames (it might take a while) (currently "
       << "only works on a Mac)?\n"
       << "Y / N: ";
  cin.clear();
  cin >> ifyes;

  // Write the video from the frames if yes.
  if ((ifyes == 'Y') || (ifyes == 'y')) {
    cout << "Creating video...\n";

    VideoWriter outputVideo("Output/Output Video.mov", 
                            CV_FOURCC('m','p','4','v'), 30, src1.size(), true);
    if (!outputVideo.isOpened()) {
      fprintf(stderr, "ERROR: Video couldn't be created.\n");
      return -1;
    }
    for (int i = 0; i < frames.size(); i++) {
      outputVideo.write(frames.at(i));
    }
    outputVideo.release();
  } 
  else
    cout << "Then, ";


  cout << "Done! Files are in the Output folder.\n\n";
  return 0;
}







void get_images(int argc,  char** argv, Mat *src1, Mat* src2) {

  if (argc != 3) {
    fprintf(stderr, "ERROR: ImTransistor requires 2 image filenames.\n");
    exit(0);
  }

  *src1 = imread(argv[1], 0);
  *src2 = imread(argv[2], 0);

  // Check if we actually loaded an image.
  if (src1->empty() || src2->empty()) {
    fprintf(stderr, "ERROR: Images could not be loaded.\n");
    exit(0);
  }

  if (src1->size() != src2->size()) {
    fprintf(stderr, "ERROR: Images are not the same size!.\n");
    exit(0);
  }

  // Use threshold to make (inverted) binary images, so that the operations
  // can be performed on the lesser amount of pixels (since black is 0, 
  // without inverting, the transistion would happen on the white pixels!).
  threshold(*src1, *src1, 50, 255, THRESH_BINARY_INV);
  threshold(*src2, *src2, 50, 255, THRESH_BINARY_INV);

}



// Makes the frames for a gradient transistion.
vector<Mat> gradient_transition(Mat im1, Mat im2, int num) {
  vector<Mat> frames(num);
  int i;
  double alpha, beta, step;
  step = 1.0 / (double) num;

  for (i = 0; i < frames.size(); i++) {
    beta += step;
    alpha = 1.0 - beta;
    addWeighted(im1, alpha, im2, beta, 0.0, frames.at(i));
  }

  // Needed cuz vectors are stupid and need iterators :P
  vector<Mat>::iterator it = frames.begin();
  it = frames.insert(it, im1);

  return frames;
}



// Rudimentary animated transistion.
vector<Mat> move_transition(Mat im1, Mat im2, int num){
  // Frames contains all the frames; the Point vectors contain the indices to
  // the initial and final images, along with arbitrary intermediary.
  vector<Mat> frames(num);
  vector<Point> finalv, initv, pixels;
  Mat same; // The pixels that will not change.
  double dist;

  bitwise_and(im1, im2, same); // Finds the pixels that are the same.
  // Get non-zero indices.
  findNonZero(im1 - same, initv);
  findNonZero(im2 - same, finalv);

  // These lines sort the vectors in the order of pixels to operate on/delete.
  // Get center of image for sorting the coordinate vectors.
  const Point center(im2.size().width/2, im2.size().height/2);
  sorter condition(center);
  sort(initv.begin(), initv.end(), condition);
  sort(finalv.begin(), finalv.end(), condition);


  // If initial image has more pixels. VERY CONVOLUTED! 
  // TODO: Simplify and make more clear of a process :P
  if (initv.size() > finalv.size()) {
    pixels.resize(finalv.size(), Point(0,0)); // make proper size

    for (int i = 0; i < finalv.size(); i++) {
      int indice;
      dist = 1000;

      // Find the point which has the least distance.
      for (int j = 0; j < initv.size(); j++) { 
        if (norm(finalv.at(i) - initv.at(j)) < dist) {
          indice = j;
          dist = norm(finalv.at(i) - initv.at(j));
        }
      }

      // Add point to pixels vector and delete from original vector.
      pixels.at(i) = initv.at(indice);
      initv.erase(initv.begin() + indice);
    }

    // Get the distances to travel, stored as points. Can be negative!
    subtract(finalv, pixels, finalv);
    // The vector to use in the loop; is updated every time with new
    // coordinates at which to draw the pixels for that frame.
    vector<Point> move;
    // The Mat for the extra pixels not used in the initial image. First draws
    // all of them, then deletes pixels in the loop.
    Mat extra = Mat::zeros(im1.size(), CV_8UC1);
    for (int j = 0; j < initv.size(); j++)
      extra.at<uchar>(initv.at(j)) = 255;
    int count = 0;

    // Create the frames.
    for (int i = 0; i < frames.size(); i++) { 
      // Scale the distance by the step amount.
      scaleAdd(finalv, (i + 1.0)/(double)num, pixels, move);
      // Make frame initially blank.
      frames.at(i) = Mat::zeros(im1.size(), CV_8UC1);
      // Draw the pixels.
      // TODO: See if anything more efficient than for loop for drawing 
      // the frames?
      for (int j = 0; j < move.size(); j++)
        frames.at(i).at<uchar>(move.at(j)) = 255;
      // Reduce amount of extra pixels being drawn each time.
      for (; count < (int)((i + 1.0)*initv.size()/(double)num); count++)
        extra.at<uchar>(initv.at(count)) = 0;
      // Add the different Mats together. So simple! :D
      frames.at(i) += same + extra;
    }

  // If final image has more pixels. EQUALLY VERY CONVOLUTED! 
  } else {

    pixels.resize(initv.size(), Point(0,0));

    for (int i = 0; i < initv.size(); i++) {
      int indice;
      dist = 1000;

      // Find point at least distance.
      for (int j = 0; j < finalv.size(); j++) { 
        // norm() calculates distance, works with both Point and Mat.
        if (norm(finalv.at(j) - initv.at(i)) < dist) {
          indice = j;
          dist = norm(finalv.at(j) - initv.at(i));
        }
      }

      pixels.at(i) = finalv.at(indice);
      finalv.erase(finalv.begin() + indice);
    }

    subtract(pixels, initv, pixels);
    vector<Point> move;

    // The extra Mat here starts out empty initially.
    Mat extra = Mat::zeros(im1.size(), CV_8UC1);
    int count = 0;

    for (int i = 0; i < frames.size(); i++) { 
      scaleAdd(pixels, (i + 1.0)/(double)num, initv, move);

      frames.at(i) = Mat::zeros(im1.size(), CV_8UC1);
      for (int j = 0; j < move.size(); j++)
        frames.at(i).at<uchar>(move.at(j)) = 255;

      // Increase amount drawn each time.
      for (; count < (int)((i + 1.0)*finalv.size()/(double)num); count++)
        extra.at<uchar>(finalv.at(count)) = 255;

      frames.at(i) += same + extra;
    }

  }

  // Needed cuz vectors are stupid and need iterators :P
  vector<Mat>::iterator it = frames.begin();
  it = frames.insert(it, im1); // Insert original image in at first.

  for (int i = 0; i < frames.size(); i++) {
    // Invert the grayscale back to normal.
    bitwise_not(frames.at(i), frames.at(i));
    // Convert to color.
    cvtColor(frames.at(i), frames.at(i), CV_GRAY2RGB);
  }

  return frames;
}


