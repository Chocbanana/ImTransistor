ImTransistor
============

**Image Transition Animator (for CS 11)**

by Bhavana Jonnalagadda and Anusha Sinha

## Version

n/a


### Milestones

Program can:
- Read in two same formatted, sized, monocolored images and:
  * ~~output them straight.~~
  * convert to animated gif (of 2 images).
- Create transition:
  * of any type (most likely fade transition).
  * that can have a specified number of intermediate images and speed.
  * which can be output in animated gif format.
  * using any algorithm.
  * using image-deformation algorithm, not modified.
  * using modified/personally created algorithm.
- Have a GUI.
- Read in different types of images:
  * that are different formats.
  * that are multiple colors (solid colored).
  * that are of any colors and gradients (will convert to solid color).
  * any sizes, any colors, different formats.
- Do more? :D



### Goals

**Basic Features to Aim for:**
- Given two images (centered, only 2 colors, jpg)
- it will create simple movement of the lineart from one image to another, optimizing creation/deletion/movement of the lineart color (the color less present).
- Then, given the length of time (in seconds) and fps, it will make a gif that loops for that length of time and fps.

**Second level:**
- Can give any type of images (will convert to jpg/ 2 color internally)
- not centered
- different options for type of transition (less optimized, include bias for side/direction?)
- gui
