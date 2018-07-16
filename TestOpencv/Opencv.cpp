#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;

int main()
{
    VideoCapture cap;
    cap.open("test.mp4");
    if (!cap.isOpened())
        return -1;

    namedWindow("OpenCV3", WINDOW_AUTOSIZE);
    Mat frame;
    for (;;) 
    {
        cap >> frame;
        if (frame.empty()) 
            break;

        imshow("OpenCV3", frame);
        if (waitKey(33) >= 0) 
            break;
    }

    destroyWindow("OpenCV3");
    return 0;
}