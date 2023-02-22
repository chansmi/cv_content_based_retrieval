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
#include <cmath>

// Creates the data base for each of my images depending on the task: baselineMatching for example
void createFeatureDatabase( std::string T, std::string B, int N, std::vector<std::string> imagePaths, std::string csvFile ){
    std::string csvName = "/Users/chandlersmith/Desktop/CS5330/2_Project/2_Project/olympus/" + csvFile;
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
            char *imgName_cstr = new char[imagePaths[k].length() + 1];
            strcpy(imgName_cstr, imagePaths[k].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
        }
    }
    // create multihist matching
    else if(csvFile == "multiHistogramMatching.csv"){
        // Get the full historgram, using code like histogram matching
        //cout << "\n" << imagePaths.size();
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
            // Create the histogram
            //std::vector<float> histogramVector;
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
            //cout << image << "\n" ;
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
            //cv::Mat imageCenter = cv::imread(imagePaths[k]);
            //cout << imagePaths[k];
            cv::Mat imageCenter = image;
            //centerPic(imageCenter, centerPicVector);
            // create hist for centerpic
            std::vector<float> CenterFlatHist;
            for(int k=0; k<243; k++){
                int Hsize = 8;
                int divisor = 256 / Hsize;
                int dim[3] = {Hsize, Hsize, Hsize};
                cv::Mat centerHist = cv::Mat::zeros(3, dim, CV_32F); // floating point
                // Create the histogram
                //cv::Mat image = cv::imread(imagePaths[k]); // read in the image
                float x = (image.cols -9) / 2; // Left column of the 9x9
                float y = (image.rows -9) / 2;  // top row of 9x9
                int x_ = (int)x;
                int y_ = (int)y;
                for(int i=x_; i<(x_+9); i++){ // iterate over rows and columns
                    cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                    for(int j=y_; j<(y_+9); j++){
                        float b = sptr[j][0] / divisor;
                        float g = sptr[j][1] / divisor;
                        float r = sptr[j][2] / divisor;
                        centerHist.at<float>(b, g, r)++;
                    }
                }
                float sum = 0.0;
                sum = cv::sum(centerHist)[0];
                centerHist /= sum;
                CenterFlatHist = flattenHist(centerHist);
            }
            // Append my centerpic data to the end of my histogram data
            flatHist.insert(flatHist.end(), CenterFlatHist.begin(), CenterFlatHist.end());
            
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[k].length() + 1];
            strcpy(imgName_cstr, imagePaths[k].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
        }
    }
    else if(csvFile == "textureAndColor.csv"){
        for(int k=0; k<imagePaths.size(); k++){
            // Regular histogram
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
            // Sobel Mag texture
            std::vector<float> textureMat;
            textureMat = texture( image );
            // append TextureMat
            flatHist.insert(flatHist.end(), textureMat.begin(), textureMat.end());
            
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[k].length() + 1];
            strcpy(imgName_cstr, imagePaths[k].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
            break;
        }
    }
    // create customDesign
    else if(csvFile == "customDesign.csv"){
        // Get the full historgram, using code like histogram matching
        //cout << "\n" << imagePaths.size();
        for(int k=0; k<imagePaths.size(); k++){
            int Hsize = 8;
            int divisor = 256 / Hsize;
            int i, j;
            int dim[3] = {Hsize, Hsize, Hsize};
            cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F); // floating point
            // Create the histogram
            //std::vector<float> histogramVector;
            cv::Mat image = cv::imread(imagePaths[k]); // read in the image
            //cout << image << "\n" ;
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
            //cv::Mat imageCenter = cv::imread(imagePaths[k]);
            //cout << imagePaths[k];
            cv::Mat imageCenter = image;
            //centerPic(imageCenter, centerPicVector);
            // create hist for centerpic
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[k].length() + 1];
            strcpy(imgName_cstr, imagePaths[k].c_str());
            // append each vector
            append_image_data_csv(csvName_cstr, imgName_cstr, flatHist, 0);
        }
    }
}
//////////////////////// SINGLE VECTOR FUNCTIONS ////////////////////////
// Identify the target if comparing one vector
std::vector<float> calcTarget(std::string T, std::string csvFile, std::vector<std::string> imagePaths, std::vector<float> TargetVector){
    if(csvFile == "baselineMatching.csv"){
        //cout << "\n Target Venter Pic" << T;
        cv::Mat image = cv::imread(T);
        centerPic(image, TargetVector);
    }
    else if (csvFile == "histogramMatching.csv"|| csvFile == "customDesign.csv"){
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
        TargetVector = flattenHist(hist);
    }
    return TargetVector;
}

// Loop through whole db and make comparisons for single comparisons
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
    else if (csvFile == "histogramMatching.csv" ){
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
            
            histogramSortVec.push_back(make_pair(intersection,imagePaths[k]));
            }
        sort(histogramSortVec.begin(),histogramSortVec.end());
        cout << "\n Top Images for histogramMatching";
        for (int i=0; i<N; i++){
            //topImages.push_back(baselineSortVec[i].second);
            cout << "\n" << histogramSortVec[i].second;
        }
    }
    return topImages;
}
    
///////////////////   DOUBLE VECTOR FUNCTIONS //////////////////
// Calc target if two vectors needed
std::vector<std::vector<float>> twoCalcTarget(std::string T, std::string csvFile, std::vector<std::string> imagePaths, std::vector<std::vector<float>> TargetVector){
        if(csvFile == "multiHistogramMatching.csv"){
            // Get the full historgram, using code like histogram matching
                int Hsize = 8;
                int divisor = 256 / Hsize;
                int i, j;
                int dim[3] = {Hsize, Hsize, Hsize};
                cv::Mat hist = cv::Mat::zeros(3, dim, CV_32F);
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
                float sum = 0.0;
                sum = cv::sum(hist)[0];
                hist /= sum;
                std::vector<float> flatHist = flattenHist(hist);
                // get the center pic and repeat
                std::vector<float> centerPicVector;
                //cv::Mat imageCenter = cv::imread(T);
                cv::Mat imageCenter = image;
                //centerPic(imageCenter, centerPicVector);
                // create hist for centerpic
                std::vector<float> CenterFlatHist;
                for(int k=0; k<243; k++){
                    int Hsize = 8;
                    int divisor = 256 / Hsize;
                    int dim[3] = {Hsize, Hsize, Hsize};
                    cv::Mat centerHist = cv::Mat::zeros(3, dim, CV_32F); // floating point
                    // Create the histogram
                    //cv::Mat image = cv::imread(imagePaths[k]); // read in the image
                    float x = (image.cols -9) / 2; // Left column of the 9x9
                    float y = (image.rows -9) / 2;  // top row of 9x9
                    int x_ = (int)x;
                    int y_ = (int)y;
                    for(int i=x_; i<(x_+9); i++){ // iterate over rows and columns
                        cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                        for(int j=y_; j<(y_+9); j++){
                            float b = sptr[j][0] / divisor;
                            float g = sptr[j][1] / divisor;
                            float r = sptr[j][2] / divisor;
                            centerHist.at<float>(b, g, r)++;
                        }
                    }
                    //normalize the histogram - should be sum of 1 at the end
                    float sum = 0.0;
                    sum = cv::sum(centerHist)[0];
                    centerHist /= sum;
                    CenterFlatHist = flattenHist(centerHist);
                    TargetVector.push_back(flatHist);
                    TargetVector.push_back(CenterFlatHist);
            }
            std::cout << "\n TARGET" << TargetVector[1][0];
        }
        else if(csvFile == "textureAndColor.csv"){
            
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
            std::vector<float> flatHist = flattenHist(hist);
            // Sobel Mag texture
            std::vector<float> textureMat;
            textureMat = texture( image );
            TargetVector.push_back(flatHist);
            TargetVector.push_back(textureMat);
        }
        return TargetVector;
    }
    
    // Loop through whole db and make comparisons for two comparison
    std::vector<string> twoDistCalc(std::vector<std::vector<float>> TargetVector, std::string B, int N, std::vector<std::string> imagePaths, std::string csvFile){
        std::string csvName = B + "/" + "distance_"+ csvFile;
        cout << "\n" << csvName << "\n";
        std::vector<std::string> topImages;
        // create baseline
         if(csvFile == "multiHistogramMatching.csv"){
            // Calculate histogram of both full and center for current image
             std::vector<pair<float, string>> multiHistSortVec;
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
                 cv::Mat imageCenter = image;
                 //centerPic(imageCenter, centerPicVector);
                 // create hist for centerpic
                 std::vector<float> CenterFlatHist;
                 for(int k=0; k<243; k++){
                     int Hsize = 8;
                     int divisor = 256 / Hsize;
                     int dim[3] = {Hsize, Hsize, Hsize};
                     cv::Mat centerHist = cv::Mat::zeros(3, dim, CV_32F); // floating point
                     // Create the histogram
                     //cv::Mat image = cv::imread(imagePaths[k]); // read in the image
                     float x = (image.cols -9) / 2; // Left column of the 9x9
                     float y = (image.rows -9) / 2;  // top row of 9x9
                     int x_ = (int)x;
                     int y_ = (int)y;
                     for(int i=x_; i<(x_+9); i++){ // iterate over rows and columns
                         cv::Vec3b *sptr = image.ptr<cv::Vec3b>(i); // cols
                         for(int j=y_; j<(y_+9); j++){
                             float b = sptr[j][0] / divisor;
                             float g = sptr[j][1] / divisor;
                             float r = sptr[j][2] / divisor;
                             centerHist.at<float>(b, g, r)++;
                         }
                     }
                     //normalize the histogram - should be sum of 1 at the end
                     float sum = 0.0;
                     sum = cv::sum(centerHist)[0];
                     centerHist /= sum;
                     CenterFlatHist = flattenHist(centerHist);
                 }
                 
                 // Calculate the intersection distance
                 // compare the outer hist of growth = store
                 // TargetVector[0] vs flatHist
                 std::vector<float> distances;
                 // compute sum of histogram buckets for normalization
                 double intersection = 0.0;
                 double outerIntersection = 0.0;
                 // compute sum of histogram buckets.
                 // For outer histograms
                 double sumT = 0.0;
                 double sumI = 0.0;
                 for( int i=0; i<flatHist.size();i++){
                     sumT += TargetVector[0][i];
                     sumI += flatHist[i];
                 }
                 // compute intersection
                 for(int i=0; i<flatHist.size(); i++){
                     double af = TargetVector[0][i] / sumT;
                     double bf = flatHist[i] / sumI;
                     intersection += af < bf ? af : bf;
                 }
                 outerIntersection = (1.0 - intersection);
                 
                 std::vector<pair<float, string>> outerHistogramSortVec;
                 outerHistogramSortVec.push_back(make_pair(outerIntersection,imagePaths[k]));
                 // compare the center of both = store
                 // TargetVector[1] vs centerFlatHist
                 double innerIntersection = 0.0;
                 // compute sum of histogram buckets.
                 // For outer histograms
                 double sumInnerT = 0.0;
                 double sumInnerI = 0.0;
                 for( int i=0; i<CenterFlatHist.size();i++){
                     sumInnerT += TargetVector[1][i];
                     sumInnerI += CenterFlatHist[i];
                 }
                 // compute intersection
                 for(int i=0; i<CenterFlatHist.size(); i++){
                     double af = TargetVector[1][i] / sumInnerT;
                     double bf = CenterFlatHist[i] / sumInnerI;
                     intersection += af < bf ? af : bf;
                 }
                 innerIntersection = (1.0 - intersection);
                 
                 std::vector<pair<float, string>> innerHistogramSortVec;
                 innerHistogramSortVec.push_back(make_pair(innerIntersection,imagePaths[k]));
                 // get average of both - store in another vector
                 float outerSum = 0.0;
                 for ( const auto& pair : outerHistogramSortVec){
                     outerSum += pair.first;
                 }
                 float outerAverage = outerSum / outerHistogramSortVec.size();
                 // Repeat for inner
                 float innerSum = 0.0;
                 for ( const auto& pair : innerHistogramSortVec){
                     innerSum += pair.first;
                 }
                 float innerAverage = innerSum / innerHistogramSortVec.size();
                 // combine
                 float intersectionAverage = (outerAverage + innerAverage) / 2;
                 // sort and return N
                 multiHistSortVec.push_back(make_pair(intersectionAverage,imagePaths[k]));
             }
            sort(multiHistSortVec.begin(),multiHistSortVec.end());
            cout << "\n Top Images for multiHistMatching";
            for (int i=0; i<N; i++){
                //topImages.push_back(baselineSortVec[i].second);
                cout << "\n" << multiHistSortVec[i].second;
                }
        }
        else if(csvFile == "textureAndColor.csv"){
            //Don't append hist values
            // save them separately
            // compare the outer hist of growth = store
            // compare the center of both = store
            // get average of both - store in another vector
            // sort and return N
            std::vector<pair<float, string>> textureHistSortVec;
            for(int k=0; k<imagePaths.size(); k++){
                // Regular histogram
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
                // Sobel Mag texture
                std::vector<float> textureMat;
                textureMat = texture( image );
                
                // Calculate the intersection distance
                // compare the outer hist of growth = store
                // TargetVector[0] vs flatHist
                std::vector<float> distances;
                // compute sum of histogram buckets for normalization
                double intersection = 0.0;
                double colorIntersection = 0.0;
                // compute sum of histogram buckets.
                // For outer histograms
                double sumT = 0.0;
                double sumI = 0.0;
                for( int i=0; i<flatHist.size();i++){
                    sumT += TargetVector[0][i];
                    sumI += flatHist[i];
                }
                // compute intersection
                for(int i=0; i<flatHist.size(); i++){
                    double af = TargetVector[0][i] / sumT;
                    double bf = flatHist[i] / sumI;
                    intersection += af < bf ? af : bf;
                }
                colorIntersection = (1.0 - intersection);
                
                std::vector<pair<float, string>> colorHistogramSortVec;
                colorHistogramSortVec.push_back(make_pair(colorIntersection,imagePaths[k]));
                // compare the center of both = store
                // TargetVector[1] vs centerFlatHist
                double textureIntersection = 0.0;
                // compute sum of histogram buckets.
                // For outer histograms
                double sumTextT = 0.0;
                double sumTextI = 0.0;
                for( int i=0; i<textureMat.size();i++){
                    sumTextT += TargetVector[1][i];
                    sumTextI += textureMat[i];
                }
                // compute intersection
                for(int i=0; i<textureMat.size(); i++){
                    double af = TargetVector[1][i] / sumTextT;
                    double bf = textureMat[i] / sumTextI;
                    intersection += af < bf ? af : bf;
                }
                textureIntersection = (1.0 - intersection);
                
                std::vector<pair<float, string>> textureHistogramSortVec;
                textureHistogramSortVec.push_back(make_pair(textureIntersection,imagePaths[k]));
                // get average of both - store in another vector
                float colorSum = 0.0;
                for ( const auto& pair : colorHistogramSortVec){
                    colorSum += pair.first;
                }
                float colorAverage = colorSum / colorHistogramSortVec.size();
                // Repeat for inner
                float textureSum = 0.0;
                for ( const auto& pair : textureHistogramSortVec){
                    textureSum += pair.first;
                }
                float textureAverage = textureSum / textureHistogramSortVec.size();
                float intersectionAverage = (colorAverage + textureAverage) / 2;
                // sort and return N
                textureHistSortVec.push_back(make_pair(intersectionAverage,imagePaths[k]));
            }
           sort(textureHistSortVec.begin(),textureHistSortVec.end());
           cout << "\n Top Images for textureHistMatching";
           for (int i=0; i<N; i++){
               //topImages.push_back(baselineSortVec[i].second);
               cout << "\n" << textureHistSortVec[i].second;
               }
        }
        return topImages;
    }
// calculation for the customDesign function
std::vector<string> plantDistance(std::vector<float> TargetVector, std::string B, int N, std::vector<std::string> imagePaths, std::string csvFile ){
    
    std::string csvName = B + "/" + "distance_"+ csvFile;
    cout << "\n" << csvName << "\n";
    float dist;
    std::vector<std::string> topImages;
    // create baseline
        std::vector<pair<float, string>> plantSortVec;
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
            
            // Elucidean distance calc
            // target vector
            // flatHist - both flat vectors
            // d =√[(x2 – x1)2 + (y2 – y1)2]
            // loop through each value, find the sqrt of sqr dist
                //for a pixel
                    //(target row - plant row)2
                    // target col - plant col)2
                    // sum
                    // sqrt
            // Elucidean
            float differenceSquared = 0.0;
            
            for( int j = 0; j<TargetVector.size(); j++){
                float difference = TargetVector[j] - flatHist[j];
                differenceSquared += difference * difference;
            }
            dist = sqrt(differenceSquared);
            plantSortVec.push_back(make_pair(dist,imagePaths[k]));
            }
        sort(plantSortVec.begin(),plantSortVec.end());
        cout << "\n Top Images for plantMatching";
        for (int i=0; i<N; i++){
            cout << "\n" << plantSortVec[i].second;
        }
    return topImages;
}

// controls the pipline and what functions are run.
void pipeline( std::string T, std::string B, int N, std::string csvFile){
    //Go through whole DB via the function readfiles
    std::string database = B; //Filepath to olympus
    std::string dirName = "olympus";
    // read the images - return an array (vector) of images
    std::vector<std::string> imagePaths = readImages(database, dirName);
    // can input a manual toggle before calling createFeatureDatabse - T or F
    createFeatureDatabase(T, B, N, imagePaths, csvFile);
    if(csvFile == "baselineMatching.csv" || csvFile == "histogramMatching.csv"){
        std::vector<float> TargetVector;
        TargetVector = calcTarget(T, csvFile, imagePaths, TargetVector);
        distCalc(TargetVector, B, N, imagePaths,csvFile);
    }
    else if(csvFile == "multiHistogramMatching.csv" || csvFile == "textureAndColor.csv"){
        std::vector<std::vector<float>> TargetVector;
        //cout << "RUN MY CODE";
        TargetVector = twoCalcTarget(T, csvFile, imagePaths, TargetVector);
        twoDistCalc(TargetVector, B, N, imagePaths,csvFile);
    }
    else if( csvFile == "customDesign.csv"){
        std::vector<float> TargetVector;
        TargetVector = calcTarget(T, csvFile, imagePaths, TargetVector);
        plantDistance(TargetVector, B, N, imagePaths,csvFile);
    }
}

// change arguements in in pipeline which dictates what kind of matching. This is done in the main file and that gets passed to create the feature vector


