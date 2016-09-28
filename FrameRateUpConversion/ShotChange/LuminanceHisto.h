#ifndef LUMINANCE_HISTOGRAM_H__
#define LUMINANCE_HISTOGRAM_H__


#include <math.h>
#include "../MatUtil.h"
typedef unsigned char	BYTE;

class LuminanceHisto
{

private:
	int m_dim;
	double getVarLHisto(int LShotChange[], int n)
	{
		float sum = 0;
		for (int i = 0; i < n; i++) {
			sum += LShotChange[i];
		}
		float avg = sum / n;
		double var = 0;
		for (int i = 0; i < n; i++) {
			var = (avg - LShotChange[i])*(avg - LShotChange[i]);
		}
		var = sqrt(var);
		return var;
	}
	bool isLHistoShotchange(double diffHisto, double thr) {
		if (diffHisto < thr)
			return false;
		else
			return true;
	}

	void makeL256histo(int bin[], const BYTE* Y, int width, int height) {
		for (int i = 0; i < 256; i++)
			bin[i] = 0;
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				int a = (int)(Y[w + h*width]);
				a = a >= 0 ? a : 0;
				a = a < 256 ? a : 255;
				bin[a]++;
			}
		}

	}
	double XiSquareHistoDiff(int bin1[], int bin2[], int dim) {
		double x = 0;
		for (int i = 0; i < dim; i++) {
			if (bin1[i] == 0 && bin2[i] == 0)
				x += 0;
			else
				x += (double)(((bin1[i] - bin2[i])*(bin1[i] - bin2[i]))) / ((bin1[i] + bin2[i]));
		}
		return x;
	}
public:
	LuminanceHisto() : m_dim(0) {}
	~LuminanceHisto() {

	}
	void setDim(int dim)
	{
		m_dim = dim;
	}
	int getDim() { return m_dim; }
	double getDiffHisto(int type, BYTE* curYFrame, BYTE* nextYFrame, int width, int height)
	{
		const BYTE* pSrc[1], *pRef[1];
		int YPitch;
		int *bin1 = new int[m_dim];
		int *bin2 = new int[m_dim];

		makeL256histo(bin1, (const BYTE*) curYFrame,  width, height);
		makeL256histo(bin2, (const BYTE*), width, height);
		double diff = XiSquareHistoDiff(bin1, bin2, 256);
		return diff;
	}
};


#endif