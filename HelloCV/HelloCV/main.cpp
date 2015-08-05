//
//  main.cpp
//  HelloCV
//
//  Created by albert on 2015/8/5.
//  Copyright (c) 2015年 albert. All rights reserved.
//

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    cv::Mat src;
    
    src=cv::imread("/Users/miuki001/work/opencv_practice/picture/Lena.jpg", -1);
    
    cv::imshow("show image",src);
    
    cv::waitKey(0);
    
    std::cout << "Hello, World!\n";
    return 0;
}
