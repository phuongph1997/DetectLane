#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
bool isWhite(int l, int a, int b);
bool isWhite2(int l,int a, int b);
bool isYellow(int a, int b);
void LocNhieu(int x,Mat segImage);
void mag_thresh(Mat segImage);
void abc(Mat Image);
int main() {
    namedWindow("source",WINDOW_NORMAL);
    namedWindow("imageLab",WINDOW_NORMAL);
    namedWindow("locmau",WINDOW_NORMAL);
    namedWindow("locnhieu",WINDOW_NORMAL);
    namedWindow("SobleXImgae",WINDOW_NORMAL);
    namedWindow("SobleYImgae",WINDOW_NORMAL);
    namedWindow("SobleImage",WINDOW_NORMAL);
    namedWindow("SobleImage",WINDOW_NORMAL);
    namedWindow("test",WINDOW_NORMAL);
    namedWindow("result",WINDOW_NORMAL);
    namedWindow("binary",WINDOW_NORMAL);
    namedWindow("cannyImage",WINDOW_NORMAL);
    Mat imageLab, equalizeHistImage,grayImage,GaussianBlurImage,SobleXImgae,SobleYImgae,SobleImage,test,binary,ROI,cannyImage;
    Mat source = imread("/home/hoaiphuong/Desktop/photos/13.png",CV_LOAD_IMAGE_COLOR);
    imshow("source",source);
  //  LocNhieu(1,source);
    mag_thresh(source);
    /*
   // imshow("cannyImage",cannyImage);
 //   GaussianBlur(source,GaussianBlurImage,Size(5,5),1);
    cvtColor(source,grayImage,COLOR_BGR2GRAY);
//    imshow("grayImage",grayImage);
   equalizeHist(grayImage,equalizeHistImage);
 //   imshow("equalizeHistImage",equalizeHistImage);
    cvtColor(source,imageLab,COLOR_BGR2Lab);
    imshow("imageLab",imageLab);

    Mat segImage(source.rows,source.cols,CV_8UC3);
    for (int i=0;i<imageLab.rows;++i) {
        const  uchar *lab_data = imageLab.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);

        for (int j = 0; j < imageLab.cols; ++j) {
            int l = *lab_data++;
            l-=128;
            int a = *lab_data++;
            a-=128;
            int b = *lab_data++;
            b-=128;
            if (isWhite2(l,a,b)) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }

            else
            if (isYellow(a,b))
            {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }

            else
            {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }
  //  addWeighted( binary, 0.5, segImage, 0.5, 0, test );
    imshow("locmau", segImage);


   // threshold(SobleImage,binary,50,255,THRESH_BINARY);
   // imshow("binary",binary);



    erode(segImage, segImage, getStructuringElement(MORPH_ELLIPSE, Size(35, 35)) );// tang vung den
      dilate( segImage, segImage, getStructuringElement(MORPH_ELLIPSE, Size(35, 35)) ); //tang vung trang

      //morphological closing (removes small holes from the foreground)
      dilate( segImage, segImage, getStructuringElement(MORPH_ELLIPSE, Size(25, 25)) );
      erode(segImage, segImage, getStructuringElement(MORPH_ELLIPSE, Size(25, 25)) );
    imshow("locnhieu",segImage);

//    imshow("locnhieu", segImage);
   // ROI=SobleImage(Rect(0,SobleImage.rows/2,SobleImage.cols,SobleImage.rows/2));
   // imshow("test",ROI);
   // abc(ROI);
     */
      waitKey(0);

  }
  bool isWhite(int l,int a, int b)
  {
      if (((a>=-2) && (a<=5) && (b>=-17) && (b<=-5) ) )
          return true;
      return false;
  }
  bool isWhite2(int l,int a, int b)
  {
      if ( (a>=-2) && (a<=3) && (b>-9) && (b<=-4) && l<83)
          return true;
      return false;
  }
bool isYellow(int a, int b)
{
    //if ( (a>=-10) && (a<=-2) && (b>=20) && (b<=60) )
        if ( (a>=-10) && (a<=-1) && ((b>=20) && (b<=60)||(b>=5) && (b<=30) ) )
        return true;
    return false;
}

void abc(Mat Image)
{
    // The 4-points at the input image
    vector<Point2f> origPoints;
    origPoints.push_back( Point2f(Image.cols/3, 0) );
    origPoints.push_back( Point2f(Image.cols*2/3, 0) );
    origPoints.push_back( Point2f(Image.cols, Image.rows) );
    origPoints.push_back( Point2f(0, Image.rows) );

    // The 4-points correspondences in the destination image
    vector<Point2f> dstPoints;
    dstPoints.push_back( Point2f(Image.cols*0.5/3, 0) );
    dstPoints.push_back( Point2f(Image.cols*2.5/3, 0) );
    dstPoints.push_back( Point2f(Image.cols*0.75, Image.rows) );
    dstPoints.push_back( Point2f(Image.cols*0.25,  Image.rows) );
    Mat Matrix,result;
    Matrix=getPerspectiveTransform(origPoints,dstPoints);
    warpPerspective(Image,result,Matrix,Size(Image.cols,Image.rows));
    imshow("result",result);
}
void LocNhieu(int x,Mat segImage)
{
    Mat SobleXImgae,SobleYImgae,SobleImage,binaryImage2,scaleSobel;
    Sobel(segImage,SobleXImgae,-1,1,0);
    convertScaleAbs(SobleXImgae,SobleXImgae);


    imshow("SobleXImgae",SobleXImgae);


    Sobel(segImage,SobleYImgae,-1,0,1);
    convertScaleAbs(SobleYImgae,SobleYImgae);
    imshow("SobleYImgae",SobleYImgae);

    addWeighted( SobleXImgae, 1, SobleYImgae, 1, 0.5, SobleImage );

    imshow("SobleImage",SobleImage);
}
void mag_thresh(Mat segImage)
{
    Mat SobleXImgae,SobleYImgae,SobleImage,binaryImage2,gramag,ketqua;
    Sobel(segImage,SobleXImgae,-1,1,0);
    convertScaleAbs(SobleXImgae,SobleXImgae);
    Sobel(segImage,SobleYImgae,-1,0,1);
    convertScaleAbs(SobleYImgae,SobleYImgae);

    for (int i=0;i<segImage.rows;++i) {
        const  uchar *lab_data = segImage.ptr<u_int8_t >(i);
        for (int j = 0; j < imageLab.cols; ++j) {
            int l = *lab_data++;
            l-=128;
            int a = *lab_data++;
            a-=128;
            int b = *lab_data++;
            b-=128;
            if (isWhite2(l,a,b)) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }

            else
            if (isYellow(a,b))
            {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            }

            else
            {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }

  //  addWeighted( SobleXImgae, 1, SobleYImgae, 1, 0.5, SobleImage );
  sqrt(  SobleXImgae*SobleXImgae+SobleYImgae*SobleYImgae,SobleImage);
    double min ,max;
     minMaxLoc(SobleImage,&min,&max);
     SobleImage.convertTo(SobleImage,CV_8U,1/max);
    imshow("SobleImage",SobleImage);
}