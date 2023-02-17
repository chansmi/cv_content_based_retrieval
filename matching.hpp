//
//  matching.hpp
//  2_Project
//
//  Created by Chandler Smith on 2/15/23.
//

#ifndef matching_hpp
#define matching_hpp
#include <opencv2/core.hpp>

#include <stdio.h>

void centerPic( cv::Mat &src, std::vector<float> &vector);


std::vector<float> flattenHist( cv::Mat hist);

#endif /* matching_hpp */
