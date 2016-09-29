#ifndef __BDHI_H__
#define __BDHI_H__


typedef unsigned char BYTE;

void BDHI(int* LUT, BYTE* pdst, int dstPitch, int width, int height, int W_threshold, int H_threshold);


float getPh(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& D, int& p);
float getPv(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& D, int& p);
float getPr(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& D, int& p);
float getPd(int* LUT, BYTE* pixels, int x, int y, int width, int height, int pixelPitch, float& D, int& p);

#endif