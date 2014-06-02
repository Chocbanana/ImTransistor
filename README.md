ImTransistor
============

**Image Transition Animator (for CS 11)**

by Bhavana Jonnalagadda and Anusha Sinha

## Version

<<<<<<< HEAD
1.0.0

## Description

ImTransistor creates a series of frames (and movie if option selected) between two images. Arguments are 2 images of the same size, and can be mostly any format (except .gif). The program will convert them to black and white, and ask for the number of frames to be created; currently, the movie outputs at 30fps. Make sure there is no folder named “Output” in the same directory!

Will be improved in the future, with features such as a GUI, transition options, and more
natural/fluid looking transitions. :)
=======
0.9.0
>>>>>>> FETCH_HEAD


### Milestones

Program can:
- Read in two same sized images and:
  * ~~output them straight.~~
<<<<<<< HEAD
  * ~~convert to monochrome and output.~~
=======
>>>>>>> FETCH_HEAD
- Create transition:
  * ~~of any type (fade transition).~~
  * ~~that can have a specified number of intermediate images and speed.~~
  * ~~which can be output in .mov format.~~
  * ~~using any algorithm.~~
<<<<<<< HEAD
  * ~~using modified/personally created algorithm.~~
- Have a GUI.
- Read in different types of images:
  * that are different formats.
  * that are multiple colors, and keeps colors.
- Do more? :D


=======
  * using image-deformation algorithm, not modified.
  * using modified/personally created algorithm.
- Have a GUI.
- Read in different types of images:
  * ~~that are different formats.~~
  * that are multiple colors (solid colored).
  * that are of any colors and gradients (will convert to solid color).
  * any sizes, any colors, different formats.
- Do more? :D



### Goals

**Basic Features to Aim for:**
- Given two images (centered, only 2 colors, jpg)
- it will create simple movement of the lineart from one image to another, optimizing creation/deletion/movement of the lineart color (the color less present).
- Then, given the length of time (in seconds) and fps, it will make a mov for that length of time and fps.

**Second level:**
- Can give any type of images (will convert to jpg/ 2 color internally)
- not centered
- different options for type of transition (less optimized, include bias for side/direction?)
- gui
>>>>>>> FETCH_HEAD
