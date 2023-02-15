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

void createFeatureDatabase( std::string B, std::vector<std::string> imagePaths, std::string csvFile ){
    //If the DB doesn’t already exist, clear the csv, input data into csv
    //Otherwise, just reading from existing csv

    std::string csvName = B + csvFile;
    cout << "\n" << csvName << "\n";

    // create baseline
    if(csvFile == "baselineMatching"){
        for(int i=0; i<imagePaths.size(); i++){
            std::vector<float> centerPicVector; // ints of center pic for each image
            cout << "\n" << imagePaths[i] << "\n";
            cv::Mat image = cv::imread(imagePaths[i]);
            //centerPic(image, centerPicVector);
            //cout << "\n" << "read image successfully Size:" << centerPicVector.size() <<"\n";
            
            // Gopal block to make append image data work
            char *csvName_cstr = new char[csvName.length() + 1];
            strcpy(csvName_cstr, csvName.c_str());
            char *imgName_cstr = new char[imagePaths[i].length() + 1];
            strcpy(imgName_cstr, imagePaths[i].c_str());
            
            append_image_data_csv(csvName_cstr, imgName_cstr, centerPicVector, 0);
        }
    }
    
    
    
    // open each image
        // Call centerPic - passing in source image
        // save each image centerpic to a row in the csv
}

void pipeline( std::string T, std::string B, int N, std::string csvFile){
    
    //Take input as target image TODO: type of matching can be added later
    //Go through whole DB via the function readfiles
    std::string database = B; //Filepath to olympus
    //std::string dirName = database.substr(-7, -1);//String maniuplation to return the last 7 chars of a strong or everything after the final /
    std::string dirName = "olympus";
    //std::cout << ("This is the Pipeline");
    // read the images - return an array (vector) of images
    std::vector<std::string> imagePaths = readImages(database, dirName);
    // can input a manual toggle before calling createFeatureDatabse - T or F
    createFeatureDatabase(B, imagePaths, csvFile);
    
    //Take target image, calc same feature
    
    
}



// change arguements in in pipeline which dictates what kind of matching
// that gets passed to create feature
// 5 functions for 5 tasks - reuse it for the target image and can actually do the comparisons
//

