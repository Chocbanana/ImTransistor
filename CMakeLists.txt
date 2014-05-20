cmake_minimum_required(VERSION 2.8)

# Project name.
project( ImTransistor )

# Get libraries/packages
find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS filesystem system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# The executable
add_executable( ImTransistor IT_main.cpp )

# Linked libraries
link_directories( ${Boost_LIBRARY_DIRS} )
target_link_libraries( ImTransistor ${Boost_LIBRARIES} )
target_link_libraries( ImTransistor ${OpenCV_LIBS} )