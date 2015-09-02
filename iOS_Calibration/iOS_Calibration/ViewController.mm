//
//  ViewController.m
//  iOS_Calibration
//
//  Created by albert on 2015/9/2.
//  Copyright (c) 2015年 albert. All rights reserved.
//

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
    
    self.videoCamera.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;//AVCaptureDevicePositionFront;
    self.videoCamera.defaultAVCaptureSessionPreset = AVCaptureSessionPreset352x288;
    self.videoCamera.defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationLandscapeLeft;//AVCaptureVideoOrientationLandscapeLeft;//AVCaptureVideoOrientationPortrait;
    self.videoCamera.defaultFPS = 1;
    self.videoCamera.grayscaleMode = NO;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



// Reference http://docs.opencv.org/doc/tutorials/ios/image_manipulation/image_manipulation.html
-(UIImage *)UIImageFromCVMat:(cv::Mat)cvMat
{
    NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
    CGColorSpaceRef colorSpace;
    
    if (cvMat.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
                                        cvMat.rows,                                 //height
                                        8,                                          //bits per component
                                        8 * cvMat.elemSize(),                       //bits per pixel
                                        cvMat.step[0],                            //bytesPerRow
                                        colorSpace,                                 //colorspace
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                        provider,                                   //CGDataProviderRef
                                        NULL,                                       //decode
                                        false,                                      //should interpolate
                                        kCGRenderingIntentDefault                   //intent
                                        );
    
    
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return finalImage;
}


#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
extern void my_calibration_init();
extern int my_calibration(Mat& view);
extern void my_undistort_image(Mat &view, Mat &undistortView);

- (void)processImage:(cv::Mat&)image;
{
    static int mode = 0, counter=0;
    cv::Mat image_copy = image.clone();
    cv::Mat image_undistorted;
    
    if( mode != 2 ) {
        // Put image to do calibration

        // pick one frame every second to avoid all sample are at the same position
        mode = my_calibration(image);
    }
    else {;
        self.videoCamera.defaultFPS = 30;
        
        // If calibrated, undistort image
        my_undistort_image(image, image_undistorted);
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            [imageView2 setImage:[self UIImageFromCVMat:image_undistorted]];
            //[imageView2 setNeedsDisplay];
        });

    }
}
#endif



#pragma mark - UI Actions

- (IBAction)actionStart:(id)sender;
{
    [self.videoCamera start];
    my_calibration_init();
}

@end
