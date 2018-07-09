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

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));

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

        for (int i = 0; i < contours.size(); i++) {
            double a = arcLength(contours[i], false);
            if (hierarchy[i][3] == -1 && a > 500) {
                // convexHull(contours[i],hull[i]);
                contours_list.push_back(contours[i]);
            }


        }
        Mat ketqua(source.size(),CV_8UC1,Scalar(0));
        //  ketqua.setTo(Scalar(0));
        imshow("ketqua1", ketqua);
        //  drawContours(ketqua, contours_list,-1, 255, CV_FILLED);
        // Mat contoursImage = source.clone();

        for(size_t i = 0; i < contours_list.size(); i++){
            // approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.04, true);
            drawContours(ketqua, contours_list, i, Scalar(255));   // fill BLUE
        }


        //   drawContours(ketqua, contours_list,-1, 255, CV_FILLED);
        imshow("ketqua", ketqua);

        bool kiemtra =true;

        // Step of each window

        vector<Point> locations;   // output, locations of non-zero pixels
        vector<Point> centerLaneLeft;
        vector<Point> centerLaneRight;
       // vector<Point> test;
        int dem=0;
        for (int row = ketqua.rows; row >=0; row--)
        {

            for (int col = 0; col <= ketqua.cols/2; col ++)
            {

                //  ketqua.at<uchar>(row,col)=255;
                if(ketqua.at<uchar>(Point(col,row))==255)
                {
                  //  test.push_back(Point(col,row));

                    if(kiemtra==true)
                    {
                        locations.push_back(Point(col,row));
                        dem =dem+1;
                        kiemtra= false;
                        if(dem ==2)
                        {
                            break;
                        }
                    }


                }
                else
                {
                    kiemtra= true;
                }
            }
            if(dem ==2)
            {
                int x=(locations[0].x+locations[1].x) / 2;
                int y=locations[0].y;
                centerLaneLeft.push_back(Point(x,y));
                row = row-20;
            }
            dem =0;
            locations.clear();
            kiemtra= true;
        }

        for (int row = ketqua.rows; row >=0; row--)
        {

            for (int col = ketqua.cols/2; col <= ketqua.cols; col ++)
            {

                //  ketqua.at<uchar>(row,col)=255;
                if(ketqua.at<uchar>(Point(col,row))==255)
                {
                    //test.push_back(Point(col,row));

                    if(kiemtra==true)
                    {
                        locations.push_back(Point(col,row));
                        dem =dem+1;
                        kiemtra= false;
                        if(dem ==2)
                        {
                            break;
                        }
                    }


                }
                else
                {
                    kiemtra= true;
                }
            }
            if(dem ==2)
            {
                int x=(locations[0].x+locations[1].x) / 2;
                int y=locations[0].y;
                centerLaneRight.push_back(Point(x,y));
                row = row-20;
            }
            dem =0;
            locations.clear();
            kiemtra= true;
        }


        //   imshow("Step 2 draw Rectangle", ketqua);

        for (int i=0 ; i < centerLaneLeft.size()-1 ; i++)
        {
            line(source,centerLaneLeft[i],centerLaneLeft[i+1],Scalar(0,255,0),10);
            //   circle(source,centerLane[i],10,Scalar(0,255,0));

        }
        for (int i=0 ; i < centerLaneRight.size()-1 ; i++)
        {
            //line(source,centerLane[i],centerLane[i+1],Scalar(0,255,0));
            line(source,centerLaneRight[i],centerLaneRight[i+1],Scalar(0,255,0),10);

        }

        // ROI=SobleImage(Rect(0,SobleImage.rows/2,SobleImage.cols,SobleImage.rows/2));
        // imshow("ketqua", ketqua);
       // imshow("ve vong", source);



        execTime = (getTickCount()*1.0000 - prevCount) / (getTickFrequency() * 1.0000);


        putText(source,"time detect "+to_string(execTime) + " s" , Point2f(100,100), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2);
        video.write(source);
        // ROI=SobleImage(Rect(0,SobleImage.rows/2,SobleImage.cols,SobleImage.rows/2));
        imshow("contoursImage", source);
        cout << "execTime = " << execTime  << endl;
        if(waitKey(30) >= 0) break;
    }

    return 0;
}

