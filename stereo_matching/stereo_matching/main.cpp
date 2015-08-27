//
//  main.cpp
//  stereo_matching
//
//  Created by albert on 2015/8/28.
//  Copyright (c) 2015年 albert. All rights reserved.
//

#include "stereo.h"
#include "params_parser.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

#define PI 3.1415
using namespace cv;
int main(int argc, char** argv)
{
    //string pFile = (char*) argv[1];
    string pFile = "/Users/miuki001/Work/opencv_test/stereo_matching/Images/EXAMPLE-f.ini";
    //string pFile = "/Users/miuki001/Work/opencv_test/stereo_matching/Images/EXAMPLE-p.ini";
    string imgFile1 = "/Users/miuki001/Work/opencv_test/stereo_matching/Images/Fish-L.jpg";
    string imgFile2 = "/Users/miuki001/Work/opencv_test/stereo_matching/Images/Fish-R.jpg";
    string imgRecFile1 = "Fish-L-rec-p-s.jpg";
    string imgRecFile2 = "Fish-R-rec-p-s.jpg";
    string imgDis = "Fish-dis.jpg";
    
    
    //string pFile = "..\\..\\..\\Images\\EXAMPLE-p.ini";
    //string imgFile1 = "..\\..\\..\\Images\\Per-L.bmp";
    //string imgFile2 = "..\\..\\..\\Images\\Per-R.bmp";
    //string imgRecFile1 = "..\\..\\..\\Images\\Per-L-rec-p.bmp";
    //string imgRecFile2 = "..\\..\\..\\Images\\Per-R-rec-p.bmp";
    //string imgDis = "..\\..\\..\\Images\\Per-dis.bmp";
    params_parser pp(pFile);
    
    namedWindow( "Image1", WINDOW_AUTOSIZE );
    namedWindow( "Image2", WINDOW_AUTOSIZE );
    namedWindow( "RecImage1", WINDOW_AUTOSIZE );
    namedWindow( "RecImage2", WINDOW_AUTOSIZE );
    namedWindow( "DisImage", WINDOW_AUTOSIZE );
    
    stereo st(CameraType::CT_FISHEYE, RectifyType::RT_PESPECTIVE);
    Mat R1,R2,P1,P2,Q;
    Mat img1 = imread(imgFile1);
    Mat img2 = imread(imgFile2);
    //Size imgsize = img1.size();
    
    imshow( "Image1", img1 );
    imshow( "Image2", img2 );
    
    // use rectify function of stereo class
    st.stereoRectify(pp.cameraMatrix1, pp.cameraMatrix2, pp.R, pp.T, R1, R2, P1, P2);
    
    //use rectify function of opencv, cv::stereoRectify must use data of CV_64F
    /*pp.cameraMatrix1.convertTo(pp.cameraMatrix1,CV_64F);
     pp.cameraMatrix2.convertTo(pp.cameraMatrix2,CV_64F);
     pp.distCoeffs1.convertTo(pp.distCoeffs1,CV_64F);
     pp.distCoeffs2.convertTo(pp.distCoeffs2,CV_64F);
     pp.R.convertTo(pp.R, CV_64F);
     pp.T.convertTo(pp.T, CV_64F);
     cv::stereoRectify(pp.cameraMatrix1,pp.distCoeffs1, pp.cameraMatrix2, pp.distCoeffs2, imgsize, pp.R, pp.T, R1, R2, P1, P2, Q);
     R1.convertTo(R1,CV_32F);
     R2.convertTo(R2,CV_32F);
     pp.cameraMatrix1.convertTo(pp.cameraMatrix1,CV_32F);
     pp.cameraMatrix2.convertTo(pp.cameraMatrix2,CV_32F);
     pp.distCoeffs1.convertTo(pp.distCoeffs1,CV_32F);
     pp.distCoeffs2.convertTo(pp.distCoeffs2,CV_32F);*/
    
    Mat map11, map12, map21, map22;
    Point2f logRange(0*PI, 1*PI);
    Point2f latRange(0*PI, 1*PI);
    Mat cameraMatrixNew1 = pp.cameraMatrix1.clone();
    cameraMatrixNew1.at<float>(0,0) = cameraMatrixNew1.at<float>(0,0) / 4;
    cameraMatrixNew1.at<float>(1,1) = cameraMatrixNew1.at<float>(1,1) / 4;
    st.initUndistortRectifyMap(pp.cameraMatrix1, pp.distCoeffs1, R1, cameraMatrixNew1, img1.size(),CV_32F, map11, map12, logRange, latRange);
    st.initUndistortRectifyMap(pp.cameraMatrix2, pp.distCoeffs2, R2, cameraMatrixNew1, img2.size(), CV_32F, map21, map22, logRange, latRange);
    Mat recImg1, recImg2;
    st.rectifyImage(img1, img2, map11, map12, map21, map22, recImg1, recImg2);

    imshow( "RecImage1", recImg1 );
    imshow( "RecImage2", recImg2 );
    imwrite(imgRecFile1, recImg1);
    imwrite(imgRecFile2, recImg2);
    Mat dis;
    st.stereoMatching(recImg1, recImg2, dis, 0, 100/16*16, 5, 8*5*5, 32*5*5);
    dis.convertTo(dis, CV_8U);
    imshow( "DisImage", dis );
    //imwrite(imgDis, dis);
    waitKey(0);
    
}