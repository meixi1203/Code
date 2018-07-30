#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;

int main()
{
    VideoCapture cap;
    cap.open("test.mp4");
    if (!cap.isOpened())
        return -1;

    namedWindow("frame", WINDOW_AUTOSIZE);
    namedWindow("cvtColor", WINDOW_AUTOSIZE);
    namedWindow("Canny", WINDOW_AUTOSIZE);

    Mat frame;
    Mat dstImage;
    Mat image;
    double threshold1 = 40.0;
    double threshold2 = 200.0;

    for (;;)
    {
        cap >> frame;
        if (frame.empty())
        {
            break;
        }

        Canny(frame, image, threshold1, threshold2);
        cvtColor(frame,dstImage, COLOR_BGR2GRAY);

        imshow("frame", frame);
        imshow("cvtColor", dstImage);
        imshow("Canny", image);

        if (waitKey(33) >= 0)
        {
            break;
        }
    }

    destroyWindow("frame");
    destroyWindow("cvtColor");
    destroyWindow("Canny");
    return 0;
}