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

#define TEST_CEILING_VIEW 1
#define TEST_WALL_VIEW 2
#define TEST_METHOD TEST_WALL_VIEW

// Only CelinigView support RENDER_DOUBLE_PARONOMA
#define RENDER_PARONOMA 1
#define RENDER_DOUBLE_PARONOMA 2
#define RENDER_METHOD RENDER_DOUBLE_PARONOMA


//#define TEST_FILE_Ceiling "/Users/miuki001/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg"
//#define TEST_FILE_Wall "/Users/miuki001/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg"
#define TEST_FILE_Ceiling "/Users/liaokuohsun/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg"
#define TEST_FILE_Wall "/Users/liaokuohsun/Work/opencv_practice/fisheye_paronoma_1/fisheye_paronoma_1/lillestromfisheye_small.jpg"

// globals
cv::Mat src, temp, dst, doubleParonoma;
cv::Mat map_x, map_y;
#define REMAP_WINDOW "Remap Test"

void make_circle_to_rectangle_map(int Wd,int Hd,int R1,int R2,int Cx,int Cy, int OffsetX);
void buildMapForWallView(int Wd, int Hd, int Cx, int Cy, float hfovd, float vfovd);

void TEST_CeilingView(void);
void TEST_WallView(void);

int main(int argc, char** argv) {
    
#if TEST_METHOD == TEST_CEILING_VIEW
    TEST_CeilingView();
#endif
    
#if TEST_METHOD == TEST_WALL_VIEW
    TEST_WallView();
#endif

    
}


void TEST_CeilingView() {
    
    // load image
    src = cv::imread(TEST_FILE_Ceiling, 1);
    if(src.empty()==true) {
        printf("The file %s can not be loaded\n", TEST_FILE_Ceiling);
        return;
    }
    
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


void TEST_WallView() {
    
    int Ws, Hs, Wd, Hd;
    
    // load image
    src = cv::imread(TEST_FILE_Wall, 1);
    if(src.empty()==true) {
        printf("The file %s can not be loaded\n", TEST_FILE_Wall);
        return;
    }
    
    cv::namedWindow(REMAP_WINDOW, CV_WINDOW_AUTOSIZE);

    Ws = src.cols;
    Hs = src.rows;
    Wd = src.cols*(4.0/3.0);
    Hd = src.rows;
    map_x.create(Hd, Wd, CV_32FC1);
    map_y.create(Hd, Wd, CV_32FC1);
    
    // create destination and the maps
    dst.create(Hd/2, Wd, src.type());
    
    buildMapForWallView(Ws,Hs,Wd,Hd, 120.0, 120.0);
    remap(src, temp, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
    
#if 0
    cv::Size dSize = cv::Size(temp.cols/4, temp.rows/4);
    cv::resize(temp, dst, dSize);
    cv::resizeWindow(REMAP_WINDOW, temp.cols/4, temp.rows/4);
    
    cv::Mat sub;
    cv::Mat subImage(dst, cv::Rect(0, dst.rows/4, dst.cols, dst.rows/2));
    imshow(REMAP_WINDOW, subImage);
    
#else
//    cv::Mat subImage(temp, cv::Rect(0, temp.rows/4, temp.cols, temp.rows/2));
//    imshow(REMAP_WINDOW, subImage);
    imshow(REMAP_WINDOW, temp);
#endif
    
    waitKey(10*1000);
}


// Reference
// http://paulbourke.net/dome/fish2/
// https://github.com/kscottz/dewarp/blob/master/fisheye/defish.py
void buildMapForWallView(int Ws, int Hs, int Wd, int Hd, float hfovd, float vfovd)
{
    int i, j, count = 0;
    float vstart=0, hstart=0, vfov=0, hfov=0;
    float xmax, xmin, xscale, xoff, zmax, zmin, zscale, zoff;
    float xp, zp, xS, yS, phi, theta;
    
    //Build the fisheye mapping
    vfov = (vfovd/180.0)*CV_PI;
    hfov = (hfovd/180.0)*CV_PI;
    vstart = ((180.0-vfovd)/180.00)*CV_PI/2.0;
    hstart = ((180.0-hfovd)/180.00)*CV_PI/2.0;
    
    //need to scale to changed range from our
    //smaller cirlce traced by the fov
    xmax = sin(CV_PI/2.0)*cos(vstart);
    xmin = sin(CV_PI/2.0)*cos(vstart+vfov);
    xscale = xmax-xmin;
    xoff = xscale/2.0;
    zmax = cos(hstart);
    zmin = cos(hfov+hstart);
    zscale = zmax-zmin;
    zoff = zscale/2.0;
    
    //Fill in the map, this is slow but
    //we could probably speed it up
    //since we only calc it once, whatever
    for( j = 0; j < Hd; j++) {
        for (i = 0; i < Wd; i++) {
            count = count + 1;
            phi   = vstart + (vfov * ((float(i)/float(Wd))));
            theta = hstart + (hfov * ((float(j)/float(Hd))));
            xp = ((sin(theta)*cos(phi))+xoff)/zscale;
            zp = ((cos(theta))+zoff)/zscale;
            xS = Ws-(xp*Ws);
            yS = Hs-(zp*Hs);
            
            map_x.at<float>(j, i) = xS;
            map_y.at<float>(j, i) = yS;
        }
    }
}
