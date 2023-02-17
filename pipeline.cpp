//
//  pipeline.cpp
//  2_Project
//
//  Created by Chandler Smith on 2/14/23.
//

#include "pipeline.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include <utility>
#include "readfiles.hpp"
#include "matching.hpp"
#include "csv_util.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// Creates the data base for each of my images depending on the task: baselineMatching for example
void createFeatureDatabase( std::string T, std::string B, int N, std::vector<std::string> imagePaths, std::string csvFile ){
    //If the DB doesn’t already exist, clear the csv, input data into csv
    //Otherwise, just reading from existing csv
    std::string csvName = B + "/" + csvFile;
    cout << "\n" << csvName << "\n";
    
    // create baseline
    if(csvFile == "baselineMatching.csv"){
        for(int i=0; i<imagePaths.size(); i++){
            std::vector<float> centerPicVector; // ints of center pic for each image
            cv::Mat image = cv::imread(imagePaths[i]);
            centerPic(image, centerPicVector);
            // This produces a vector of 9x9 pixels
            // Gopal block to make append image data work
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[i].length() + 1];
            strcpy(imgName_cstr, imagePaths[i].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, centerPicVector, 0);
            // Now that I have a vector of images and the corresponding pixels, now I need to call Baseline based on the target
            //baselineMatching(T, N, csvFile);
            // print out the image filepaths gathered from baseline matching
        }
    }
    // Create histogram matching db
    else if(csvFile == "histogramMatching.csv"){
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
        // Create the histogram
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
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
            std::vector<float> flatHist = flattenHist(hist);
            // write to csv
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[i].length() + 1];
            strcpy(imgName_cstr, imagePaths[i].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
            
        }
    }
    // create multihist matching
    else if(csvFile == "multiHistogramMatching"){
        // Get the full historgram, using code like histogram matching
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
            // Create the histogram
            //std::vector<float> histogramVector;
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
            for(i=0; i<image.rows; i++){ // rows
                cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                for(j=0;j<image.cols;j++){
                    float b = sptr[j][0] / divisor;
                    float g = sptr[j][1] / divisor;
                    float r = sptr[j][2] / divisor;
                    hist.at<float>(b, g, r)++;
                }
            }
            float sum = 0.0;
            sum = cv::sum(hist)[0];
            hist /= sum;
            std::vector<float> flatHist = flattenHist(hist);
            // get the center pic and repeat
            std::vector<float> centerPicVector;
            cv::Mat imageCenter = cv::imread(imagePaths[i]);
            centerPic(imageCenter, centerPicVector);
            // Append my centerpic data to the end of my histogram data
            flatHist.insert(flatHist.end(), centerPicVector.begin(), centerPicVector.end());
            
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[i].length() + 1];
            strcpy(imgName_cstr, imagePaths[i].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
        }
    }
    else if(csvFile == "textureAndColor.csv"){
        
    }
}


// Identify the target
std::vector<float> calcTarget(std::string T, std::string csvFile, std::vector<std::string> imagePaths, std::vector<float> TargetVector){
    if(csvFile == "baselineMatching.csv"){
        //cout << "\n Target Venter Pic" << T;
        cv::Mat image = cv::imread(T);
        centerPic(image, TargetVector);
    }
    else if (csvFile == "histogramMatching.csv"){
        int Hsize = 8;
        int divisor = 256 / Hsize;
        int i, j;
        int dim[3] = {Hsize, Hsize, Hsize};
        cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
    // Create the histogram
        cv::Mat image = cv::imread(T); // read in the image
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
        std::vector<float> TargetVector = flattenHist(hist);
    }
    else if(csvFile == "multiHistogramMatching"){
        // Get the full historgram, using code like histogram matching
        for(int k=0; k<T.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
            // Create the histogram
            //std::vector<float> histogramVector;
            cv::Mat image = cv::imread(T); // read in the image
            for(i=0; i<image.rows; i++){ // rows
                cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                for(j=0;j<image.cols;j++){
                    float b = sptr[j][0] / divisor;
                    float g = sptr[j][1] / divisor;
                    float r = sptr[j][2] / divisor;
                    hist.at<float>(b, g, r)++;
                }
            }
            float sum = 0.0;
            sum = cv::sum(hist)[0];
            hist /= sum;
            std::vector<float> flatHist = flattenHist(hist);
            // get the center pic and repeat
            std::vector<float> centerPicVector; // ints of center pic for each image
            cv::Mat imageCenter = cv::imread(T);
            centerPic(imageCenter, centerPicVector);
            // Append my centerpic data to the end of my histogram data
            flatHist.insert(flatHist.end(), centerPicVector.begin(), centerPicVector.end());
            std::vector<float> TargetVector = flatHist;
        }
    }
    else if(csvFile == "textureAndColor.csv"){
        
    }
    return TargetVector;
}
    
// Loop through whole db and make comparisons between t and
std::vector<string> distCalc(std::vector<float> TargetVector, std::string B, int N, std::vector<std::string> imagePaths, std::string csvFile){
    std::string csvName = B + "/" + "distance_"+ csvFile;
    cout << "\n" << csvName << "\n";
    float dist;
    std::vector<std::string> topImages;
    // create baseline
    if(csvFile == "baselineMatching.csv"){
        std::vector<pair<float, string>> baselineSortVec;
        for(int i=0; i<imagePaths.size(); i++){
            std::vector<float> centerPicVector; // ints of center pic for each image
            cv::Mat image = cv::imread(imagePaths[i]);
            centerPic(image, centerPicVector);
            // This produces a vector of 9x9 pixels
            // Sum of squared error
            float targetSSE =0.0;
            float currentImageSSE = 0.0;
            //cout << "\n in baseline sse";
            //cout << "\n" << TargetVector << "\n";
            // SSE calculation
            for( int j = 0; j<243; j++){
                float error = TargetVector[j] - centerPicVector[j]; // error
                targetSSE += error * error; // sum square the error for target
                currentImageSSE += centerPicVector[j] * centerPicVector[j]; // sum square of error
            }
            dist = targetSSE - currentImageSSE;
            // thank you to Ravina for code inspiration
            baselineSortVec.push_back(make_pair(dist,imagePaths[i]));
            }
        sort(baselineSortVec.begin(),baselineSortVec.end());
        cout << "\n Top Images for baselineMatching";
        for (int i=0; i<N; i++){
            //topImages.push_back(baselineSortVec[i].second);
            cout << "\n" << baselineSortVec[i].second;
        }
    }
    else if (csvFile == "histogramMatching.csv"){
        std::vector<pair<float, string>> histogramSortVec;
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
            // Create the histogram
            //std::vector<float> histogramVector;
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
            for(i=0; i<image.rows; i++){ // rows
                cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                for(j=0;j<image.cols;j++){
                    float b = sptr[j][0] / divisor;
                    float g = sptr[j][1] / divisor;
                    float r = sptr[j][2] / divisor;
                    hist.at<float>(b, g, r)++;
                }
            }
            float sum = 0.0;
            sum = cv::sum(hist)[0]; // sum of histogram buckets - bucket is location in the array
            hist /= sum;
            std::vector<float> flatHist = flattenHist(hist);
            double intersection = 0.0;
            // compute sum of histogram buckets.
            double sumT = 0.0;
            double sumI = 0.0;
            for( int i=0; i<flatHist.size();i++){
                sumT += TargetVector[i]; //TODO: Here is the problem
                sumI += flatHist[i];
            }
            // compute intersection
            for(int i=0; i<flatHist.size(); i++){
                double af = TargetVector[i] / sumT;
                double bf = flatHist[i] / sumI;
                intersection += af < bf ? af : bf;
            }
            intersection = (1.0 - intersection);
            
            histogramSortVec.push_back(make_pair(intersection,imagePaths[i]));
            }
        sort(histogramSortVec.begin(),histogramSortVec.end());
        cout << "\n Top Images for histogramMatching";
        for (int i=0; i<N; i++){
            //topImages.push_back(baselineSortVec[i].second);
            cout << "\n" << histogramSortVec[i].second;
        }
    }
    else if(csvFile == "multiHistogramMatching"){
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F);
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
            for(i=0; i<image.rows; i++){ // rows
                cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                for(j=0;j<image.cols;j++){
                    float b = sptr[j][0] / divisor;
                    float g = sptr[j][1] / divisor;
                    float r = sptr[j][2] / divisor;
                    hist.at<float>(b, g, r)++;
                }
            }
            float sum = 0.0;
            sum = cv::sum(hist)[0];
            hist /= sum;
            std::vector<float> flatHist = flattenHist(hist);
            // get the center pic and repeat
            std::vector<float> centerPicVector; // ints of center pic for each image
            cv::Mat imageCenter = cv::imread(imagePaths[i]);
            centerPic(imageCenter, centerPicVector);
            // Append my centerpic data to the end of my histogram data
            flatHist.insert(flatHist.end(), centerPicVector.begin(), centerPicVector.end());
            
            // Calculate the intersection distance
            std::vector<float> distances;
            // compute sum of histogram buckets for normalization
            double intersection = 0.0;
            double sumT = 0.0;
            double sumI = 0.0;
            for( int i=0; i<flatHist.size();i++){
                sumT += TargetVector[i];
                sumI += flatHist[i];
            }
            // compute intersection
            for(int i=0; i<flatHist.size(); i++){
                double af = TargetVector[i] / sumT;
                double bf = flatHist[i] / sumI;
                intersection += af < bf ? af : bf;
            }
            intersection = (1.0 - intersection);
            distances.push_back(intersection);
            
            // Weight for each value
            // TODO: add weights to calculation
            float weightedDistance = 0.0;
            
            std::vector<pair<float, string>> baselineSortVec;
            baselineSortVec.push_back(make_pair(intersection,imagePaths[i]));
            sort(baselineSortVec.begin(),baselineSortVec.end());
            cout << "\n Top Images for histogramMatching";
            for (int i=0; i<N; i++){
                //topImages.push_back(baselineSortVec[i].second);
                cout << "\n" << baselineSortVec[i].second;
            }
        }
    }
    else if(csvFile == "textureAndColor.csv"){
        
    }
    return topImages;
}

void pipeline( std::string T, std::string B, int N, std::string csvFile){
    //Go through whole DB via the function readfiles
    std::string database = B; //Filepath to olympus
    std::string dirName = "olympus";
    // read the images - return an array (vector) of images
    std::vector<std::string> imagePaths = readImages(database, dirName);
    // can input a manual toggle before calling createFeatureDatabse - T or F
    createFeatureDatabase(T, B, N, imagePaths, csvFile);
    // now I have a csv file as a feature db, for baseline, it is a image row and 243 values for RGB for each pixel.
    std::vector<float> TargetVector;
    TargetVector = calcTarget(T, csvFile, imagePaths, TargetVector);
    distCalc(TargetVector, B, N, imagePaths,csvFile);
}

// change arguements in in pipeline which dictates what kind of matching
// that gets passed to create feature
// 5 functions for 5 tasks - reuse it for the target image and can actually do the comparisons

