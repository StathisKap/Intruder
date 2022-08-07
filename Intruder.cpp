#define QUANTITY_OF_TIME  3000
#define MOVEMENT_THRESHOLD 45

#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

using namespace cv;
using namespace std;


int main(int argc, const char** argv)
{
  char name_of_Image[256];
  // add your file name
  VideoCapture cap; //argv[1]);

  if (argc < 2)
    cap.open(0); //argv[1]);
  else
    cap.open(atoi(argv[1])); //argv[1]);

  Mat flow, frame;
  // some faster than mat image container
  UMat  flowUmat, prevgray;
  while(true)
  {
    if (cap.grab() == false) {
      // if video capture failed
      cout << "Video Capture Fail" << endl;
      break;
    }
    else {
      Mat img, original, forPicture;
      // capture frame from video file
      cap.retrieve(img, CAP_OPENNI_BGR_IMAGE);
      cap.retrieve(forPicture, CAP_OPENNI_BGR_IMAGE);
      resize(img, img, Size(640, 480));
      // save original for later
      img.copyTo(original);
      // just make current frame gray
      cvtColor(img, img, COLOR_BGR2GRAY);
      // For all optical flow you need a sequence of images.. Or at least 2 of them. Previous
      //and current frame
      // if previous frame is not empty.. There is a picture of previous frame. Do some
      //optical flow alg.

      if (prevgray.empty() == false ) {
        // calculate optical flow
        calcOpticalFlowFarneback(prevgray, img, flowUmat, 0.4, 1, 12, 4, 8, 27.5, 0);
        // copy Umat container to standard Mat
        flowUmat.copyTo(flow);
        // By y += 5, x += 5 you can specify the grid
        for (int y = 0; y < original.rows; y += 10) {
          for (int x = 0; x < original.cols; x += 10)  {
            // get the flow from y, x position * 10 for better visibility
            const Point2f flowatxy = flow.at<Point2f>(y, x) * 5;

            float xLen = flowatxy.x*10, yLen = flowatxy.y*10, hypotenuse = hypot(xLen,yLen);
            if((hypotenuse > MOVEMENT_THRESHOLD ) && (clock()%QUANTITY_OF_TIME==0))
            {
              sprintf(name_of_Image,"IntruderPics/Intruder%ld.jpg",clock()/CLOCKS_PER_SEC);
              printf("x:%.3f \t y: %.3f\n",xLen,yLen);
              imwrite(name_of_Image,forPicture);
            }
            line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255,0,0));
            // draw initial point
            circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
          }
        }
        // draw the results
        namedWindow("Intruder", WINDOW_AUTOSIZE);
        imshow("Intruder", original);
        // fill previous image again
        img.copyTo(prevgray);
      }
      else {
        // fill previous image in case prevgray.empty() == true
        img.copyTo(prevgray);
      }
      waitKey(20);
    }
  }
}
