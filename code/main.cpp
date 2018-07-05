#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat SobelXY(Mat segImage);
int main() {
    VideoCapture cap("/home/hoaiphuong/Desktop/photos/outcpp.avi"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    long double execTime, prevCount, time;
    execTime = prevCount = time = 0;

    int width=cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),30, Size(width,height));

    for(;;) {
        prevCount = getTickCount() * 1.0000;
        Mat source;
        cap >> source; // get a new frame from camera

        // Mat source = imread("/home/hoaiphuong/Desktop/photos/10.png",CV_LOAD_IMAGE_COLOR);
        imshow("source", source);
        Mat imgHSV;

        cvtColor(source, imgHSV, COLOR_BGR2HLS); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(0, 238, 0), Scalar(178, 255, 255), imgThresholded); //Threshold the image
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        imshow("imgThresholded", imgThresholded);
        Mat cannyImage;
        Canny(imgThresholded, cannyImage, 50, 100);
        imshow("cannyImage", cannyImage);

        vector<Vec4i> hierarchy;
        vector<vector<Point>> contours;

        findContours(cannyImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        vector<Point> contours_poly;
        vector<vector<Point>> contours_list;
        vector<vector<Point>> contours_final;

        for (size_t i = 0; i < contours.size(); i++) {
            double a = arcLength(contours[i], false);
            if (hierarchy[i][3] == -1 && a > 500) {
                // convexHull(contours[i],hull[i]);
                contours_list.push_back(contours[i]);
            }


        }
        Mat contoursImage = source.clone();
        for (size_t idx = 0; idx < contours_list.size(); idx++) {

            drawContours(contoursImage, contours_list, idx, Scalar(0, 255, 0), 5);
        }




        execTime = (getTickCount()*1.0000 - prevCount) / (getTickFrequency() * 1.0000);


        putText(contoursImage,"time detect "+to_string(execTime) + " s" , Point2f(100,100), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2);
        video.write(contoursImage);
        // ROI=SobleImage(Rect(0,SobleImage.rows/2,SobleImage.cols,SobleImage.rows/2));
        imshow("contoursImage", contoursImage);
        cout << "execTime = " << execTime  << endl;
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
