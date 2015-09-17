//
//  ViewController.h
//  iOS_FindMovingObject
//
//  Created by albert on 2015/9/9.
//  Copyright (c) 2015年 albert. All rights reserved.
//
#import <opencv2/opencv.hpp>
#import <opencv2/videoio/cap_ios.h>
#import <UIKit/UIKit.h>

using namespace cv;

@interface ViewController : UIViewController<CvVideoCameraDelegate>
{
    IBOutlet UIImageView *imageView;
    IBOutlet UIButton *button;
    CvVideoCamera* videoCamera;
}
- (IBAction)actionStart:(id)sender;

@property (nonatomic, retain) CvVideoCamera* videoCamera;

@end

