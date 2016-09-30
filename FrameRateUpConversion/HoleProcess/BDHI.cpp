#include "BDHI.h"
/*
Referenced By
IEEE Transactions on Broadcasting, VOL. 56, NO. 2, JUNE 2010
Motion-Compensated Frame Rate Up-Conversion—Part II: New Algorithms for Frame Interpolation

*/
#include <iostream>
#define PADDING(a) a = a > 256 ? 255 : a < 0 ? 0 : a

#define PEL_OCCLUSION	4
#define PEL_FILLED	1
#define PEL_FILLED_FROM_FORWARD	2
#define PEL_FILLED_FROM_BACKWARD	3
#define PEL_HOLE	0
#define PEL_UNFILLED	-1
#define PEL_FILLED_BY_BDHI	5

#define OCCLUSION	1
#define NOT_OCCLUSION	0

typedef unsigned char BYTE;

#define SGMIN(x,y)	(x>y ? y:x)
#define SGMAX(x,y)	(x<y ? y:x)
#define SGABS(x)	(x<0 ? -x:x)

float getAlpha(float A, float a, float b, float c) {
	return 1.f - ((b*c + a*c + a*b) / (a*b*c));
}

void BDHI(int* LUT, BYTE* pdst, int dstPitch, int width, int height, int W_threshold, int H_threshold)
{
	for (int h = 0; h<height; h++) {
		for (int w = 0; w<width; w++) {
			if (LUT[w + h*width] == PEL_HOLE) {
				int pdh, pdv, pdd, pdr; pdh = pdv = pdd = pdr = 0;
				float Dh, Dv, Dd, Dr; Dh = Dv = Dd = Dr = 1;
				float Ph = getPh(LUT, pdst, w, h, width, height, dstPitch, Dh, pdh);
				float Pv = getPv(LUT, pdst, w, h, width, height, dstPitch, Dv, pdv);
				float Pd = getPd(LUT, pdst, w, h, width, height, dstPitch, Dd, pdd);
				float Pr = getPr(LUT, pdst, w, h, width, height, dstPitch, Dr, pdr);
				float Pix;
				int totalPixelDiff = 0;

				/*
				if (Dh > H_threshold) {	//for blending
				w += Dh; continue;
				}
				if (Dv > H_threshold) {	//for blending
				h += Dv; continue;
				}
				*/

				if (SGMIN(SGMIN(SGMIN(Dh, Dv), Dd), Dr) == Dh) { totalPixelDiff = pdh; Pix = Ph; }
				else if (SGMIN(SGMIN(SGMIN(Dh, Dv), Dd), Dr) == Dv) { totalPixelDiff = pdv; Pix = Pv; }
				else if (SGMIN(SGMIN(SGMIN(Dh, Dv), Dd), Dr) == Dr) { totalPixelDiff = pdr; Pix = Pr; }
				else if (SGMIN(SGMIN(SGMIN(Dh, Dv), Dd), Dr) == Dd) { totalPixelDiff = pdd; Pix = Pd; }



				Ph = (Ph*(Dv + Dd + Dr) + Pv*(Dh + Dd + Dr) + Pd*(Dh + Dv + Dr) + Pr*(Dh + Dv + Dd)) / (3 * (Dh + Dv + Dd + Dr));
				PADDING(Ph);
				pdst[w] = (BYTE)(Ph);
				LUT[w + h*width] = PEL_FILLED_BY_BDHI;
			}
		}
		pdst += dstPitch;

	}
}
//Ph, Pv, Pd, Pr, in page 146
float getPh(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& Dh, int& AbsDiff) {
	int x1, x2; x1 = x; x2 = x; int d1 = 0, d2 = 0;
	int p1 = 1, p2 = 1;
	BYTE* pixelTemp = pixels;

	while (LUT[(x1)+y*width] == PEL_HOLE) {
		d1++; x1--;
		if (x1 < 0) {
			p1 = 0;
			break;
		}
	}
	while (LUT[(x2)+y*width] == PEL_HOLE) {
		d2++; x2++;
		if (x2 >= width) {
			p2 = 0;
			break;
		}
	}
	Dh = (float)(d1 + d2);

	if (p1 == 0)	{ return pixels[x2]; }
	else if (p2 == 0){ return pixels[x1]; }
	else { p1 = pixels[x1]; p2 = pixels[x2]; }
	pixels = pixelTemp;
	AbsDiff = std::abs(p1 - p2);
	return ((p1 * ((float)d2 / (d1 + d2))) + (p2 * ((float)d1 / (d1 + d2))));;
}

float getPv(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& Dv, int& AbsDiff) {
	int y1, y2; y1 = y; y2 = y; int d1 = 0, d2 = 0;
	BYTE p1 = 1, p2 = 1;
	BYTE* pixelTemp = pixels;
	while (LUT[(x)+y1*width] == PEL_HOLE) {
		y1--; d1++;
		if (y1 <= 0) {
			p1 = 0;
			break;
		}

	}
	while (LUT[(x)+y2*width] == PEL_HOLE) {
		y2++; d2++;
		if (y2 >= height) {
			p2 = 0;
			break;
		}
	}
	if (p1 != 0) {
		for (int i = 0; i<d1; i++)
			pixels -= pixelPitch;
		p1 = pixels[x];

	}
	pixels = pixelTemp;
	if (p2 != 0) {
		for (int i = 0; i<d2; i++)
			pixels += pixelPitch;
		p2 = pixels[x];
	}
	Dv = (float)(d1 + d2);
	pixels = pixelTemp;
	if (p1 == 0) return p2;
	if (p2 == 0) return p1;
	AbsDiff = std::abs(p1 - p2);
	return (float)(p1 * ((float)d2 / (d1 + d2)) + p2 * ((float)d1 / (d1 + d2)));
}

float getPd(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& Dd, int& AbsDiff) {
	int y1, y2; y1 = y2 = y; int d1 = 0, d2 = 0; int x1 = x, x2 = x;
	BYTE p1 = 1, p2 = 1;
	BYTE* pixelTemp = pixels;
	while (LUT[(x1)+y1*width] == PEL_HOLE) {
		y1--; x1++; d1++;
		if (x1 >= width || y1<0) {
			p1 = 0;
			break;
		}

	}
	while (LUT[(x2)+y2*width] == PEL_HOLE) {
		x2--; y2++; d2++;
		if (x2 < 0 || y2 >= height) {
			p2 = 0;
			break;
		}
	}
	if (p1 != 0) {
		for (int i = 0; i<d1; i++)
			pixels -= pixelPitch;
		p1 = pixels[x1];

	}
	pixels = pixelTemp;
	if (p2 != 0) {
		for (int i = 0; i<d2; i++)
			pixels += pixelPitch;
		p2 = pixelTemp[x2];
	}
	pixels = pixelTemp;
	Dd = (float)(d1 + d2);
	if (p1 == 0) return p2;
	if (p2 == 0) return p1;
	AbsDiff = std::abs(p1 - p2);
	return (float)(p1 * ((float)d2 / (d1 + d2)) + p2 * ((float)d1 / (d1 + d2)));
}


float getPr(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& Dr, int& AbsDiff) {
	int y1, y2; y1 = y2 = y; int d1 = 0, d2 = 0; int x1 = x, x2 = x;
	BYTE p1 = 1, p2 = 1;
	BYTE* pixelTemp = pixels;
	while (LUT[(x1)+y1*width] == PEL_HOLE) {
		y1--; x1--; d1++;
		if (x1 < 0 || y1 < 0) {
			p1 = 0;
			break;
		}

	}
	while (LUT[(x2)+y2*width] == PEL_HOLE) {
		x2++; y2++; d2++;
		if (x2 >= width || y2 >= height) {
			p2 = 0;
			break;
		}
	}

	if (p1 != 0) {
		for (int i = 0; i<d1; i++)
			pixelTemp -= pixelPitch;
		p1 = pixels[x1];
		pixelTemp = pixels;
	}
	if (p2 != 0) {
		for (int i = 0; i<d2; i++)
			pixelTemp += pixelPitch;
		p2 = pixelTemp[x2];
	}
	Dr = (float)(d1 + d2);
	if (p1 == 0) return p2;
	if (p2 == 0) return p1;
	AbsDiff = std::abs(p1 - p2);
	return (float)(p1 * ((float)d2 / (d1 + d2)) + p2 * ((float)d1 / (d1 + d2)));
}