//
//  main.cpp
//  VideoCaptureByMacCam
//
//  Created by albert on 2015/8/6.
//  Copyright (c) 2015年 albert. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(){
    VideoCapture cap(0);
    
    if(!cap.isOpened()) return -1;
    
    Mat frame;
    
    namedWindow("camera",1);
    
    for(;;)
    {
        cap >> frame;
        imshow("frame", frame);
        if(waitKey(30) >= 0) break;
    }
    system("PAUSE");
    return 0;
}
