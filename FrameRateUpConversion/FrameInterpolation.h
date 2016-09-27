#ifndef __FRAMEINTERPOL_H__
#define __FRAMEINTERPOL_H__
#include <iostream>
#include <omp.h>


typedef unsigned char	uchar;
class FrameInterpolation {

	int* m_FMVX;
	int* m_FMVY;
	int* m_BMVX;
	int* m_BMVY;
	int* m_occ_n_hole_Mask;

public:
	FrameInterpolation(){}
	FrameInterpolation(int width, int height): m_FMVX(NULL), m_FMVY(NULL), m_BMVX(NULL), m_BMVY(NULL), m_occ_n_hole_Mask(NULL) {
		init(width, height);
	}
	~FrameInterpolation() {
		if (m_FMVX) delete[] m_FMVX;
		if (m_FMVY) delete[] m_FMVY;
		if (m_BMVX) delete[] m_BMVX;
		if (m_BMVY) delete[] m_BMVY;
		if (m_occ_n_hole_Mask) delete[] m_occ_n_hole_Mask;
	}
	void init(int width, int height) {
		m_FMVX = new int[width * height];
		m_FMVY = new int[width * height];
		m_BMVX = new int[width * height];
		m_BMVY = new int[width * height];
		m_occ_n_hole_Mask = new int[width * height];
		memset(m_occ_n_hole_Mask, 0, width*height*sizeof(int));
		memset(m_FMVX, 0, width*height*sizeof(int));
		memset(m_FMVX, 0, width*height*sizeof(int));
		memset(m_BMVX, 0, width*height*sizeof(int));
		memset(m_BMVY, 0, width*height*sizeof(int));
	}

	void interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval, int overlapsize
		) {
		this->reshapeMVMap(frameWidth, frameHeight, frame_interval, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		this->occ_n_hole_Mask(frameWidth, frameHeight, frame_interval, overlapsize);
		int idx = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int bmx = m_BMVX[idx + x];
				int bmy = m_BMVY[idx + x];
				int fmx = m_FMVX[idx + x];
				int fmy = m_FMVY[idx + x];
				int b_idx = x + bmx + (y + bmy)*frameWidth;
				int f_idx = x + fmx + (y + bmy)*frameWidth;
				OUT_interpolFrame[b_idx] += (prevFrame[idx + x] >> 1) / m_occ_n_hole_Mask[b_idx];
				OUT_interpolFrame[f_idx] += (nextFrame[idx + x] >> 1) / m_occ_n_hole_Mask[f_idx];
			}
			idx += frameWidth;
		}
		this->hole_processing();
	}
	void hole_processing() {
		
	};
	void occ_n_hole_Mask(int width, int height, int frameinterval, int overlapsize=0) {
		
		int idx = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int fx = x + m_FMVX[idx + x];
				int fy = y + m_FMVY[idx + x];
				int bx = x + m_BMVX[idx + x];
				int by = y + m_BMVY[idx + x];
				if ((fx - overlapsize < 0 && fx + overlapsize >= width) ||
					(fy - overlapsize < 0 && fy + overlapsize >= height)) {
					int fidx = fx + fy * width;
					m_occ_n_hole_Mask[fidx]++;
				}
				if ((bx - overlapsize < 0 && bx + overlapsize >= width) ||
					(by - overlapsize < 0 && by + overlapsize >= height)) {
					int bidx = bx + by * width;
					m_occ_n_hole_Mask[bidx]++;
				}
			}
			idx += width;
		}

	}

	void reshapeMVMap(int width, int height, int frameInterval,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY) {
		int idx = 0;
		#pragma omp parallel for
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				m_FMVX[idx + x] = (forward_mvX[idx + x] - 128) / frameInterval;
				m_FMVY[idx + x] = (forward_mvY[idx + x] - 128) / frameInterval;
				m_BMVX[idx + x] = (forward_mvX[idx + x] - 128) / frameInterval;
				m_BMVY[idx + x] = (forward_mvY[idx + x] - 128) / frameInterval;
			}
			idx += width;
		}
	}
	


};



#endif