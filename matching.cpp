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

/* // CODE for histogramMatching
float histogramMatching( float T, float hist){
    // 512 entries - 8x8x8
    //flatten to 512 by 1 array
    std::vector<float> flatHist = flattenHist(hist);
    std::vector<float> flatTarget = flattenHist(T);
    // intersection between two flattened histograms
    
    // sums are normalization
    double sumT = 0.0;
    double sumH = 0.0;
    for(int i=0; i<flatHist.size(); i++){
        sumH += hist[i];
        sumT += flatTarget[i];
    }
    
    for(int i=0; i<ha.size(; ))
    
    
    // std vector - I could take two standard vectors as my intersection
    // compute sum of histogram buckets
    
    
    return 0;
}

*/



/*
 /* char *csvFile_cstr = new char[csvFile.length() + 1];
 strcpy(csvFile_cstr, csvFile.c_str());
 std::vector<char *> filenames;
 std::vector<std::vector<float>> data;
 */
 // For each image in the database
 /* int read_image_data_csv( char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file = 0 ); */
 
 // read the image
 //int getImageData = read_image_data_csv(csvFile_cstr, imgName_cstr, data, true);
 
 //std::cout << "\n" << getImageData << "\n";
 
 /* float dist = 0.0;
 // go grab the target image from db
 //read_image_data_csv()
 //cv::Mat target = read_image_data_csv(csvFile);
 //cout << "\n" + target + "\n";
 std::ifstream file(csvFile);
 std::string row;
 std::vector<char*> filenames;
 std::vector<std::vector<float>> data;
 
/* while (std::getline(file, row)){
     std::stringstream ss(row);
     std::string cell;
     char*
 }*/
 


/*
 FILE *fp;
 float fval;
 char img_file[256];

 fp = fopen(csvFile, "r");
 if( !fp ) {
   printf("Unable to open feature file\n");
   return(-1);
 }

 printf("Reading %s\n", filename);
 for(;;) {
   std::vector<float> dvec;
   
   
   // read the filename
   if( getstring( fp, img_file ) ) {
     break;
   }
   // printf("Evaluting %s\n", filename);

   // read the whole feature file into memory
   for(;;) {
     // get next feature
     float eol = getfloat( fp, &fval );
     dvec.push_back( fval );
     if( eol ) break;
   }
   // printf("read %lu features\n", dvec.size() );

   data.push_back(dvec);

   char *fname = new char[strlen(img_file)+1];
   strcpy(fname, img_file);
   filenames.push_back( fname );
 }
 fclose(fp);
 printf("Finished reading CSV file\n");

 if(echo_file) {
   for(int i=0;i<data.size();i++) {
     for(int j=0;j<data[i].size();j++) {
   printf("%.4f  ", data[i][j] );
     }
     printf("\n");
   }
   printf("\n");
 }

 return(0);
}
 */
