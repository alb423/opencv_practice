//
//  ViewController.m
//  Hello_iOS_CV
//
//  Created by albert on 2015/8/18.
//  Copyright (c) 2015年 albert. All rights reserved.
//


// *************
// Need to test on the target to access the camera
// Can not test on simulator
#import "ViewController.hh"

@interface ViewController ()

@end

@implementation ViewController 
@synthesize videoCamera;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.videoCamera = [[CvVideoCamera alloc] initWithParentView:imageView];
    self.videoCamera.delegate = self;
    
    self.videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
    self.videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait;
    self.videoCamera.defaultFPS = 30;
    //self.videoCamera.grayscale = NO;
    self.videoCamera.grayscaleMode = NO;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void)processImage:(cv::Mat&)image;
{
    // Do some OpenCV stuff with the image
    cv::Mat image_copy;
    cvtColor(image, image_copy, CV_BGRA2BGR);
    
    // invert image
    cv::bitwise_not(image_copy, image_copy);
    cvtColor(image_copy, image, CV_BGR2BGRA);
}
#endif



#pragma mark - UI Actions

- (IBAction)actionStart:(id)sender;
{
    [self.videoCamera start];
}

@end
