#ifndef __MVDIFF_H__
#define __MVDIFF_H__

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

inline float getDiffMVMap(const char* groundtruthMVMap, const char* cmpmvMap) 
{
	Mat gtmat = imread(groundtruthMVMap, 0);
	Mat cmpmat = imread(cmpmvMap, 0);
	int sum = 0;
	for (int j = 0; j < gtmat.rows; j++) {
		for (int i = 0; i < gtmat.cols; i++) {
			int idx = i + j*gtmat.cols;
			sum += std::abs((int)(gtmat.data[idx]) - (int)(cmpmat.data[idx]));
		}
	}
	return (float)sum / gtmat.total();
}

#endif //__MVDIFF_H__