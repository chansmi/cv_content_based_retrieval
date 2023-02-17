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
#include "csv_util.h"
#include <fstream>
#include <sstream>

///////////////

// Write a function to crop an image 9x9 and store into feature vector. Takes in an image and returns a vector with a souble and vector object.
void centerPic( cv::Mat &src, std::vector<float> &vector){
    
    float x = (src.cols -9) / 2; // Left column of the 9x9
    float y = (src.rows -9) / 2;  // top row of 9x9
    int x_ = (int)x;
    int y_ = (int)y;
    //float dist = 0.0; // initialize dist
    //cv::Mat centerSquare(9, 9, CV_8UC3); // allocate 9x9 memory - CV_8UC3 for RGB values.
    // crop the image and store as centerSquare
    // process inspired by assignment 1
    for(int i=x_; i<(x_+9); i++){ // iterate over rows and columns
        for(int j=y_; j<(y_+9); j++){
            for(int c=0; c<3; c++){ //channel
                int val = int(src.at<cv::Vec3b>(i,j)[c]);
                // Thanks Ravina for push_back suggestion!
                vector.push_back(val);
            }
        }
    }
}


std::vector<float> flattenHist( cv::Mat hist){
    std::vector<float> flatHist(512);
    int i, j, k;
    int index = 0;
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            for(k=0; k<8; k++){
                flatHist[index] = hist.at<float>(i, j, k);
                index++;
            }
        }
    }
    return flatHist;
}

