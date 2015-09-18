//
//  main.cpp
//  fisheye_paronoma_1
//
//  Created by albert on 2015/9/18.
//  Copyright (c) 2015年 albert. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "shift.hpp"
using namespace cv;
using namespace std;

#define RENDER_PARONOMA 1
#define RENDER_DOUBLE_PARONOMA 2
#define RENDER_METHOD RENDER_DOUBLE_PARONOMA
// globals
cv::Mat src, temp, dst, doubleParonoma;
cv::Mat map_x, map_y;
#define REMAP_WINDOW "Remap Test"

void make_circle_to_rectangle_map(int Wd,int Hd,int R1,int R2,int Cx,int Cy, int OffsetX);

int main(int argc, char** argv) {
    
    // load image
    //src = imread(argv[1], 1);
    //src = cv::imread("/Users/miuki001/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg", 1);
    src = cv::imread("/Users/liaokuohsun/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg", 1);
    
    int Ws, Hs, Wd, Hd, R1, R2, Cx, Cy;
    Cx=src.cols/2;
    Cy=src.rows/2;
    Ws = src.cols;
    Hs = src.rows;
    R1 = 0;//20;
    R2 = src.cols/2;
    Wd = 2.0*((R2)/2)*CV_PI;
    Hd = R2 - R1;
    
    // create window
    //cv::namedWindow(REMAP_WINDOW, CV_WINDOW_NORMAL);
    cv::namedWindow(REMAP_WINDOW, CV_WINDOW_AUTOSIZE);
    
    // create destination and the maps
    dst.create(Hd, Wd, src.type());
    doubleParonoma.create(Hd*2, Wd/2, src.type());
    
    cv::imshow(REMAP_WINDOW, dst);
    
    map_x.create(Hd, Wd, CV_32FC1);
    map_y.create(Hd, Wd, CV_32FC1);
    
    make_circle_to_rectangle_map(Wd,Hd,R1,R2,Cx,Cy, 0);
    
    remap(src, temp, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
    
    //cv::flip(temp, temp, 0);
    //cv::Size dSize = cv::Size(temp.cols/4, temp.rows/4);
    //cv::resize(temp, dst, dSize);
    //cv::resizeWindow(REMAP_WINDOW, temp.cols/4, temp.rows/4);
    
    cv::flip(temp, dst, 0);
    imshow(REMAP_WINDOW, dst);
    
    int key = 0, OffsetX = 0;
    while ((key = waitKey()) != 27) {
        if(key == '1')
        {
            OffsetX =8;
#if RENDER_METHOD == RENDER_PARONOMA
            shift(dst, dst, Point(OffsetX,0), BORDER_WRAP);
            imshow(REMAP_WINDOW, dst);
#else
            shift(dst, dst, Point(OffsetX,0), BORDER_WRAP);
            cv::Mat upper = dst(cv::Rect(0,0,Wd/2,Hd));
            cv::Mat down = dst(cv::Rect(Wd/2,0,Wd/2,Hd));
            upper.copyTo(doubleParonoma(cv::Rect(0,0,Wd/2,Hd)));
            down.copyTo(doubleParonoma(cv::Rect(0,Hd,Wd/2,Hd)));
            imshow(REMAP_WINDOW, doubleParonoma);
#endif
            
        }
        else if(key == '2')
        {
            OffsetX = -8;
#if RENDER_METHOD == RENDER_PARONOMA
            shift(dst, dst, Point(OffsetX,0), BORDER_WRAP);
            imshow(REMAP_WINDOW, dst);
#else
            shift(dst, dst, Point(OffsetX,0), BORDER_WRAP);
            cv::Mat upper = dst(cv::Rect(0,0,Wd/2,Hd));
            cv::Mat down = dst(cv::Rect(Wd/2,0,Wd/2,Hd));
            upper.copyTo(doubleParonoma(cv::Rect(0,0,Wd/2,Hd)));
            down.copyTo(doubleParonoma(cv::Rect(0,Hd,Wd/2,Hd)));
            imshow(REMAP_WINDOW, doubleParonoma);
#endif
        }
        else if(key == 'q')
            break;
        // just wait
    }
    cvDestroyWindow(REMAP_WINDOW);
    return 0;
}

void make_circle_to_rectangle_map(int Wd,int Hd,int R1,int R2,int Cx,int Cy, int OffsetX){
    
    for (int j = 0; j < dst.rows; j++) {
        for (int i = 0; i < dst.cols; i++) {
            int xS, yS;
            float r, theta;
            
            r = (float)j/(float)Hd*(float)(R2-R1)+(float)R1;
            theta = ((float)(i))/(float)Wd*2.0*CV_PI;
            
            xS = Cx + r*sin(theta) ;
            yS = Cy + r*cos(theta);
            
            map_x.at<float>(j, i) = xS;
            map_y.at<float>(j, i) = yS;
        }
    }
}