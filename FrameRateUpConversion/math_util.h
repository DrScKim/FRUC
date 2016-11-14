#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include "me_util.h"

#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;
inline int calcSAD(const uint8* src, const uint8* dst, int blkW, int blkH, int frameW) {
	/*
		incorrect because it calculate only integer point when normalize time(divide blkW*blkH), 
		but fast
	*/
	int sad = 0;
	//printf("src addr : %p, dst addr : %p\n", src, dst);
	for (int y = 0; y < blkH; y++) {
		for (int x = 0; x < blkW; x++) {
			sad += std::abs(*(src + x + (y*frameW)) - *(dst + x + (y*frameW)));
		}
	}
	return sad/(blkW*blkH);
}
inline float calcSADF(const uint8* src, const uint8* dst, int blkW, int blkH, int frameW) {
	float sad= 0;
	//printf("src addr : %p, dst addr : %p\n", src, dst);
	for (int y = 0; y < blkH; y++) {
		for (int x = 0; x < blkW; x++) {
			sad += (float)std::abs(*(src + x + (y*frameW)) - *(dst + x + (y*frameW)));
		}
	}

	return sad/(float)(blkW*blkH);
}
inline double getPSNRFrame(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double  mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}

inline double getPSNRFrame(const Mat& I1, const Mat& I2, int borderX, int borderY)
{
	Mat s1;
	cv::Rect myROI(borderX, borderY, I1.rows - borderX, I1.cols - borderY);
	Mat I11 = I1(myROI);
	Mat I22 = I2(myROI);
	absdiff(I11, I22, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // sum elements per channel

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double  mse = sse / (double)(I11.channels() * I11.total());
		double psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}


inline double getAvgFramesPSNR(char* ori_folder, char* cmp_folder, char* exp = "jpg") {
	char oriFrame[256];
	char cmpFrame[256];
	int idx = 1;
	int cnt = 0;
	double sumPSRN = 0.0;
	while (1) {
		Mat ori, cmp;
		sprintf(oriFrame, "%s%04d.%s", ori_folder, idx, exp);
		sprintf(cmpFrame, "%s%04d.%s", cmp_folder, idx, exp);
		ori = imread(oriFrame);
		cmp = imread(cmpFrame);
		if (!ori.data || !cmp.data) {
			break;
		}

		sumPSRN += getPSNRFrame(ori, cmp);
		cnt++;
		idx += 2;
	}
	return sumPSRN / cnt;
}
/*
inline double getAvgPSNR(string cmpPath1, string cmpPath2) {
	VideoIO tmp, tmp2;
	tmp.readFromFile(cmpPath1);
	tmp2.readFromFile(cmpPath2);
	int i = 0;
	double sum = 0.0;//
	char framePath[255];
	while (1) {
		i++;
		tmp.read();
		tmp2.read();
		tmp.read();
		tmp2.read();
		Mat mat1 = tmp.getFrame();
		Mat mat2 = tmp2.getFrame();
		sprintf(framePath, "E:/ROK/frame_%d.jpg", i);
		if (mat1.empty() || mat2.empty()) {
			break;
		}
		imwrite(string(framePath), mat2);
		cout << "!::" << mat1.empty() << endl;
		double psnr = getPSNRFrame(mat1, mat2);;
		sum += psnr;

	}
	return sum / i;
}
*/
#endif //__MATH_UTIL_H__