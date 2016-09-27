#ifndef __COLORCONVERTER_H__
#define __COLORCONVERTER_H__

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

class ColorConverter {

private:

public:
	static int BGR2YUV(Mat& org_img, Mat& dst_img) {
		cvtColor(org_img, dst_img, CV_BGR2YUV);
		return 0;
	}
	static int YUV2Y(Mat& yuv_img, Mat& Y_img) {
		Mat channel[3];
		split(yuv_img, channel);
		channel[0].copyTo(Y_img);
		return 0;
	}
};

class AvgFrame {

public:
	static int getAvgFrame(Mat& frame1, Mat& frame2, Mat& avgFrame) {
		//if (avgFrame.channels != )
	}
};


#endif