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


// Takes the histogram and turns it into a flat list of values
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

// sobel texture
// Sobol filter
// 3x3 x values, first dirivitive in x direction
// pass it over every pizel to determine edge gradients
int sobelX3x3( cv::Mat &src, cv::Mat &dst ){
    // src is unsigned (0, 255)
    // need destination to be sign
    cv::Mat temp(src.rows, src.cols, CV_16SC3);
    dst = src.clone();
    dst.convertTo(dst, CV_16SC3);

    // iterating over rows
    for(int i=0;i<src.rows;i++) {
        for(int j=1;j<src.cols-1;j++) {
            cv::Vec3s &tptr = temp.at<cv::Vec3s>(i,j);
            for(int c = 0; c <= 2; c++)
            {
                tptr[c] = (1*src.at<cv::Vec3b>(i, j-1)[c] + 0*src.at<cv::Vec3b>(i, j)[c] + -1*src.at<cv::Vec3b>(i, j+1)[c]);
            }
        }
    }
    // iterating over columns
    for(int i=1;i<src.rows-1;i++) {
        for(int j=0;j<src.cols;j++){
            cv::Vec3s &dptr = dst.at<cv::Vec3s>(i,j);
            for(int c = 0; c <= 2; c++)
            {
                dptr[c] = (1*temp.at<cv::Vec3s>(i-1, j)[c] + 2*temp.at<cv::Vec3s>(i, j)[c] + 1*temp.at<cv::Vec3s>(i+1, j)[c]);
                dptr[c] = dptr[c]/4;// Normalize by 4
            }
        }
    }
    return 0;
}
// 3x3 y values, the inverse of the above function
int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
    cv:: Mat3s temp(src.rows, src.cols, CV_16SC3);
    dst = src.clone();
    dst.convertTo(dst, CV_16SC3);
    // Columns
    for(int i=0;i<src.rows;i++) {
        for(int j=1;j<src.cols-1;j++){
            cv::Vec3s &tptr = temp.at<cv::Vec3s>(i,j);
            for(int c = 0; c <= 2; c++)
            {
                tptr[c] = (1*src.at<cv::Vec3b>(i, j-1)[c] + 2*src.at<cv::Vec3b>(i, j)[c] + 1*src.at<cv::Vec3b>(i, j+1)[c]);
                //tptr[c] = tptr(i,j)[c]/4;
            }
        }
    }
        // iterate over columns
    for(int i=1;i<src.rows-1;i++) {
        for(int j=0;j<src.cols;j++) {
            cv::Vec3s &dptr = dst.at<cv::Vec3s>(i,j);
            for(int c = 0; c <= 2; c++)
            {
                dptr[c] = (-1*temp.at<cv::Vec3s>(i-1, j)[c] + 0*temp.at<cv::Vec3s>(i, j)[c] + 1*temp.at<cv::Vec3s>(i+1, j)[c]);
                dptr[c] = dptr[c]/4;
            }
        }
    }
    return 0;
}

// magnitude helper
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){
    dst = sx.clone();
    // magnitute is sqrt of sx2 + sy2
    for(int i=0;i<sx.rows;i++) { //sx is interchangable with sy
        for(int j=0;j<sx.cols;j++) {
            // x value from sx and y value from sy
            cv::Vec3s &x = sx.at<cv::Vec3s>(i, j);
            cv::Vec3s &y = sy.at<cv::Vec3s>(i, j);
            // run the Euclidean calculation for BGR so it is color
            short b = sqrt(pow(x[0], 2) + pow(y[0], 2));
            short g = sqrt(pow(x[1], 2) + pow(y[1], 2));
            short r = sqrt(pow(x[2], 2) + pow(y[2], 2));
            dst.at<cv::Vec3s>(i, j)[0] = b;
            dst.at<cv::Vec3s>(i, j)[1] = g;
            dst.at<cv::Vec3s>(i, j)[2] = r;
        }
    }
    return 0;
}

// Texture helper function
std::vector<float> texture( cv::Mat src ){
    cv::Mat x, y, output, y_output, mag;
    std::vector<float> vector;
    cv::Mat dst = src.clone();
    // gradient Magnitude calculation
    sobelX3x3(src, output);
    cv::convertScaleAbs(output, x);
    sobelY3x3(src, y_output);
    cv::convertScaleAbs(y_output, y);

    magnitude(x, y, mag); // TODO: Fix as this is returning 0s
    cv::convertScaleAbs(mag, dst);
    //std::cout << "\n Magnitude" << dst;
    int i, j, c;
    for(i=0; i<dst.rows; i++){
        for(j=0;j<dst.cols;j++){
            for(c=0; c<3; c++){ //channel
                int val = int(dst.at<cv::Vec3b>(i,j)[c]);
                vector.push_back(val);
            }
        }
    }
    return vector;
}
// unused
cv::Mat normHistogram( std::string imagePaths, cv::Mat &hist ){
    
        int Hsize = 8;
        int divisor = 256 / Hsize;
        int i, j;
        int dim[3] = {Hsize, Hsize, Hsize};
        hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
        // Create the histogram
        cv::Mat image = cv::imread(imagePaths); // read in the image
        for(i=0; i<image.rows; i++){ // rows
            cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
            for(j=0;j<image.cols;j++){
                float b = sptr[j][0] / divisor;
                float g = sptr[j][1] / divisor;
                float r = sptr[j][2] / divisor;
                hist.at<float>(b, g, r)++;
            }
        }
        //normalize the histogram - should be sum of 1 at the end
        float sum = 0.0;
        sum = cv::sum(hist)[0];
        hist /= sum;
        return hist;
}
