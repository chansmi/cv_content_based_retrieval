//
//  matching.cpp
//  Project2_Content_based_retrieval
//  Created by Chandler Smith on 2/9/23.
//
#include <iostream>
#include "matching.hpp"
#include <cstdio>
#include <cstring>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


// Write a function to crop an image 9x9 and store into feature vector. Takes in an image and returns a vector with a souble and vector object.
void centerPic( cv::Mat &src, std::vector<float> &vector){
    
    float x = (src.cols -9) / 2; // Left column of the 9x9
    float y = (src.rows -9) / 2;  // top row of 9x9
    int x_ = (int)x;
    int y_ = (int)y;
    //float dist = 0.0; // initialize dist
    //cv::Mat centerSquare(9, 9, CV_8UC3); // allocate 9x9 memory - CV_8UC3 for RGB values.
    // crop the image and store as centerSquare
    // process inspited by assignment 1
    for(int i=x_; i<(x_+9); i++){ // iterate over rows and columns
        for(int j=y_; j<(y_+9); j++){
            for(int c=0; c<3; c++){ //channel
                int val = int(src.at<cv::Vec3b>(i,j)[c]);
                //int val_ = int(val);
                //std::cout << val << "\n";
                // check why is this not populating correctly - try to typecast (0 and 255 values)
                vector.push_back(val);
            }
            // set vector equal to the new square
            //centerSquare.at<cv::Vec3b>(i,j) = src.at<cv::Vec3b>(y + i, x+j);
        }
    }
    std::cout << vector.size() << "\n";
    // function called pushback function
    // baselineSortVec.push_back(make_pair(squaredDist,buffer));
    // Thanks Ravina for push_back suggestion!
    //vector.push_back(std::make_pair(dist, centerSquare));
}

/*
 //SSE distance from mean = distance
 cv::Scalar mean, sd; // declare mean and sd
 cv::meanStdDev(centerSquare, mean, sd); //
 for(int i=0; i<9; i++){ // iterate over rows and columns
     for(int j=0; j<9; j++){
         cv::Vec3b pixel = centerSquare.at<cv::Vec3b>(i, j); // declare pixel
         dist = dist + pow(pixel[0] - mean[0], 2);//B
         dist = dist + pow(pixel[1] - mean[1], 2);//G
         dist = dist + pow(pixel[2] - mean[2], 2);//R
     }
 }
 */
