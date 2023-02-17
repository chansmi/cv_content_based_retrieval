//  Content Based Retrieval
//  main.cpp
//  2_Project
//
//  Created by Chandler Smith on 2/14/23.
//

// Strategy
    // Main - File run reads and executes the program - that is the main file
    // Pipeline - file two produces the overall data pipeline, reading and writing to varios databases,
    // Matching - file three carries all the code for the specific matching formulas

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include <utility>
#include "pipeline.hpp"
#include "readfiles.hpp"

int main(int argc, const char * argv[]) {
    // TEST :printf("hello Chandler");
    std::string T = argv[1]; // Target
    std::string B = argv[2]; // Database filepath
    int N = std::stoi(argv[3]); //number
    
    // TEST
    for( int i=0; i<argc; i++){
        std::cout << argv[i] << "\n";
    }
    
    //cout << "test main";
    // Now that I have successfully read in my arguments and can run the code, now it is time to reference the pipeline.
    // Below are the various types of matching, beginning with baseline matching.
    std::string matching = "baselineMatching.csv";
    //std::string matching = "histogramMatching.csv";
    //std::string matching = "multiHistogramMatching.csv";
    //std::string matching = "textureAndColor.csv";
    pipeline( T, B, N, matching);
    
    
    
    
    
}
