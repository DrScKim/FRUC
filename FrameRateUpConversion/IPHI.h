#ifndef __IPHI_H__
#define __IPHI_H__

#include <iostream>

#define IPHI_VERTICAL	0
#define IPHI_HORIZONTAL	1
#define IPHI_DOWN_LEFT	2
#define IPHI_DOWN_RIGHT	3

#define PREDBLK_WIDTH	4
#define PREDBLK_HEIGHT	4
#define PREDBLK_LEN	16

#define U_LEN	8
#define L_LEN	4

typedef unsigned char	uchar;
typedef unsigned char	in_uchar;
typedef unsigned char	out_uchar;

void IPHI(int* holetable, int frameWidth, int frameHeight, out_uchar* interpolatedFrame);
void fillHole(int x, int y, out_uchar* frame, int* hole_table, int frameWidth, int direction);
uchar fillVerticalHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth);
uchar fillHorizontalHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth);
uchar fillDownLeftHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth);
uchar fillDownRightHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth);
int checkDirection(in_uchar* u, in_uchar* l, uchar M, int x, int y, in_uchar* frames, int frameWidth);
int calcSAD(int x, int y, in_uchar* frames, in_uchar blockPixels[], int frameWidth);
void getPredictedHorizontalBlock(in_uchar* u, out_uchar blockPixels[]);
void getPredictedVerticalBlock(in_uchar* l, out_uchar blockPixels[]); 
void getPredictedDownLeftBlock(in_uchar* u, out_uchar blockPixels[]);
void getPredictedDownRightBlock(in_uchar* u, in_uchar* l, out_uchar* blockPixels, uchar M);


inline void fillHole(int x, int y, out_uchar* frame, int* hole_table, int frameWidth, int direction) {
	int a = x - 1 + (y - 1)*frameWidth;	int b = x - 0 + (y - 1)*frameWidth;	int c = x - 0 + (y + 1)*frameWidth;
	int d = x - 1 + (y - 0)*frameWidth;	int h = x + 0 + (y + 0)*frameWidth; int e = x + 1 + (y - 0)*frameWidth;
	int f = x - 1 + (y + 1)*frameWidth;	int g = x + 0 + (y + 1)*frameWidth;	int i = x + 1 + (y + 1)*frameWidth;
	uchar pixel[9]; 
	pixel[0] = frame[a]; pixel[1] = frame[b]; pixel[2] = frame[c];
	pixel[3] = frame[d];					  pixel[4] = frame[e];
	pixel[5] = frame[f]; pixel[6] = frame[g]; pixel[7] = frame[i];
	pixel[8] = frame[h];
	int weight[8];
	weight[0] = hole_table[a]; weight[1] = hole_table[b]; weight[2] = hole_table[c];
	weight[3] = hole_table[d];							  weight[4] = hole_table[e];
	weight[5] = hole_table[f]; weight[6] = hole_table[g]; weight[7] = hole_table[i];
	if (hole_table[h] == PEL_UNFILLED) {
		switch (direction) {
		case IPHI_VERTICAL:
			frame[h] = fillVerticalHole(x, y, pixel, hole_table, weight, frameWidth);
			break;
		case IPHI_HORIZONTAL:
			frame[h] = fillHorizontalHole(x, y, pixel, hole_table, weight, frameWidth);
			break;
		case IPHI_DOWN_LEFT:
			frame[h] = fillDownLeftHole(x, y, pixel, hole_table, weight, frameWidth);
			break;
		case IPHI_DOWN_RIGHT:
			frame[h] = fillDownRightHole(x, y, pixel, hole_table, weight, frameWidth);
			break;
		default:
			break;
		}

	}
	
}
inline uchar fillVerticalHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth) {
	int w[8];
	for (int i = 0; i < 8; i++)
		w[i] = weight[i] == PEL_UNFILLED ? 0 : 1;
	w[1] = w[1] * 4; w[6] = w[6] * 4;
	int total = 0;
	int pixel = 0;
	for (int i = 0; i < 8; i++) {
		total += w[i];
		pixel += w[i] * (int)pixels[i];
		
	}
	if (total == 0) {
		return (uchar)(pixels[9]);
	}
	hole_table[x + y*frameWidth] = PEL_FILLED;
	return (uchar)(pixel /= total);

}
inline uchar fillHorizontalHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth) {
	int w[8];
	for (int i = 0; i < 8; i++)
		w[i] = weight[i] == PEL_UNFILLED ? 0 : 1;
	w[3] = w[3] * 4; w[4] = w[4] * 4;
	int total = 0;
	int pixel = 0;
	for (int i = 0; i < 8; i++) {
		total += w[i];
		pixel += w[i] * (int)pixels[i];
		
	}
	if (total == 0) {
		return (uchar)(pixels[9]);
	}
	hole_table[x + y*frameWidth] = PEL_FILLED;
	return (uchar)(pixel /= total);
}
inline uchar fillDownLeftHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth) {
	int w[8];
	for (int i = 0; i < 8; i++)
		w[i] = weight[i] == PEL_UNFILLED ? 0 : 1;
	w[2] = w[2] * 4; w[5] = w[5] * 4;
	int total = 0;
	int pixel = 0;
	for (int i = 0; i < 8; i++) {
		total += w[i];
		pixel += w[i] * (int)pixels[i];
	}
	if (total == 0) {
		return (uchar)(pixels[9]);
	}
	hole_table[x + y*frameWidth] = PEL_FILLED;
	return (uchar)(pixel /= total);
}
inline uchar fillDownRightHole(int x, int y, uchar* pixels, int* hole_table, int* weight, int frameWidth) {
	int w[8];
	for (int i = 0; i < 8; i++)
		w[i] = weight[i] == PEL_UNFILLED ? 0 : 1;
	w[0] = w[0] * 4; w[7] = w[7] * 4;
	int total = 0;
	int pixel = 0;
	for (int i = 0; i < 8; i++) {
		total += w[i];
		pixel += w[i] * (int)pixels[i];
		
	}
	if (total == 0) {
		return (uchar)(pixels[9]);
	}
	hole_table[x + y*frameWidth] = PEL_FILLED;
	return (uchar)(pixel /= total);
}

inline int checkDirection(in_uchar* u, in_uchar* l, uchar M, int x, int y, in_uchar* frames, int frameWidth) {
	
	in_uchar blockPixels[PREDBLK_LEN] = { 0, };
	int dir = IPHI_HORIZONTAL;
	int sad = 255 * PREDBLK_LEN;
	int sad2;
	// calc horizon
	getPredictedHorizontalBlock(u, blockPixels);
	sad = calcSAD(x, y, frames, blockPixels, frameWidth);
	// calc vertical
	getPredictedVerticalBlock(l, blockPixels);
	sad2 = calcSAD(x, y, frames, blockPixels, frameWidth);
	dir = sad > sad2 ? IPHI_VERTICAL : dir;
	sad = sad > sad2 ? sad2 : sad;
	// calc down left
	getPredictedDownLeftBlock(u, blockPixels);
	sad2 = calcSAD(x, y, frames, blockPixels, frameWidth);
	dir = sad > sad2 ? IPHI_DOWN_LEFT : dir;
	sad = sad > sad2 ? sad2 : sad;
	// calc down right
	getPredictedDownRightBlock(u, l, blockPixels, M);
	sad2 = calcSAD(x, y, frames, blockPixels, frameWidth);
	dir = sad > sad2 ? IPHI_DOWN_RIGHT : dir;
	sad = sad > sad2 ? sad2 : sad;
	return dir;
}

inline int calcSAD(int x, int y, in_uchar* frames, in_uchar blockPixels[], int frameWidth) {
	int sum = 0;
	for (int i = 0; i < PREDBLK_WIDTH; i++) {
		for (int j = 0; j < PREDBLK_HEIGHT; j++) {
			sum += std::abs(frames[x + i + (y + j)*frameWidth] - blockPixels[i + j*PREDBLK_WIDTH]);
		}
	}
	return sum >> 4;
}

inline void getPredictedHorizontalBlock(in_uchar* u, out_uchar blockPixels[]) {
	for (int i = 0; i < PREDBLK_WIDTH; i++) {
		for (int j = 0; j < PREDBLK_HEIGHT; j++) {
			blockPixels[i + j*PREDBLK_WIDTH] = u[i];
		}
	}
}

inline void getPredictedVerticalBlock(in_uchar* l, out_uchar blockPixels[]) {
	for (int i = 0; i < PREDBLK_WIDTH; i++) {
		for (int j = 0; j < PREDBLK_HEIGHT; j++) {
			blockPixels[i + j*PREDBLK_WIDTH] = l[j];
		}
	}
}

inline void getPredictedDownLeftBlock(in_uchar* u, out_uchar blockPixels[]) {
	blockPixels[0] = (u[0] + u[1] + u[1] + u[2] + 2) >> 2;
	blockPixels[1] = blockPixels[4] = (u[1] + u[2] + u[2] + u[3] + 2) >> 2;
	blockPixels[2] = blockPixels[5] = blockPixels[8] = (u[2] + u[3] + u[3] + u[4] + 2) >> 2;
	blockPixels[3] = blockPixels[6] = blockPixels[9] = blockPixels[12] = (u[3] + u[4] + u[4] + u[5] + 2) >> 2;
	blockPixels[7] = blockPixels[10] = blockPixels[13] = (u[4] + u[5] + u[5] + u[6] + 2) >> 2;
	blockPixels[11] = blockPixels[14] = (u[5] + u[6] + u[6] + u[7] + 2) >> 2;
	blockPixels[15] = (u[6] + u[7] + u[7] + u[7] + 2) >> 2;
}

inline void getPredictedDownRightBlock(in_uchar* u, in_uchar* l, out_uchar* blockPixels, uchar M) {
	blockPixels[3] = (u[1] + u[2] + u[2] + u[3] + 2) >> 2;
	blockPixels[2] = blockPixels[7] = (u[0] + u[1] + u[1] + u[2] + 2) >> 2;
	blockPixels[1] = blockPixels[6] = blockPixels[11] = ( M + u[0] + u[0]+u[1]+ + 2) >> 2;
	blockPixels[0] = blockPixels[5] = blockPixels[10] = blockPixels[15] = (u[0] + M + M + l[0] + 2) >> 2;
	blockPixels[4] = blockPixels[9] = blockPixels[14] = (M + l[0] +l[0] + l[1] + 2) >> 2;
	blockPixels[8] = blockPixels[13] = (l[0] + l[1] + l[1] + l[2] + 2) >> 2;
	blockPixels[12] = (l[1] + l[2] + l[2] + l[3] + 2) >> 2;
}

inline void IPHI(int* holetable, int frameWidth, int frameHeight, out_uchar* interpolatedFrame) {
	int* htable = holetable;
	uchar* u = new uchar[U_LEN];
	uchar* l = new uchar[L_LEN];
	uchar M;
	for (int y = PREDBLK_HEIGHT; y < frameHeight - PREDBLK_HEIGHT; y += PREDBLK_HEIGHT) {
		for (int x = PREDBLK_WIDTH; x < frameWidth - PREDBLK_WIDTH; x += PREDBLK_WIDTH) {
			for (int i = 0; i < U_LEN; i++)
				u[i] = interpolatedFrame[(x + i) + (y - 1)*frameWidth];
			for (int j = 0; j < L_LEN; j++)
				l[j] = interpolatedFrame[(x - 1) + (y + j)*frameWidth];
			M = interpolatedFrame[(x - 1) + (y - 1)*frameWidth];
			
			int dir = checkDirection(u, l, M, x, y, interpolatedFrame, frameWidth);
			for (int i = 0; i < PREDBLK_WIDTH; i++) {
				for (int j = 0; j < PREDBLK_HEIGHT; j++) {
					fillHole(x + i, j + y, interpolatedFrame, holetable, frameWidth, dir);
				}
			}


		}
	}
}

#endif