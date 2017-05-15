cmake_minimum_required(VERSION 3.4)
project(prj_vid)

find_package( OpenCV REQUIRED )
add_executable( prj_vid prj_vid.cpp )
target_link_libraries( prj_vid ${OpenCV_LIBS} )