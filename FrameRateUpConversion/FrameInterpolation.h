#ifndef __FRAMEINTERPOL_H__
#define __FRAMEINTERPOL_H__
#include <iostream>
#include <omp.h>


typedef unsigned char	uchar;
class FrameInterpolation {

protected:
	int* m_FMVX;
	int* m_FMVY;
	int* m_BMVX;
	int* m_BMVY;
	int* m_occ_n_hole_Mask;

	void occ_n_hole_Mask(int width, int height, int frameinterval, int blkW, int blkH, int overlapsize = 0) {
		memset(m_occ_n_hole_Mask, 0, width*height*sizeof(int));
		int idx = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int fx = x + m_FMVX[idx + x];
				int fy = y + m_FMVY[idx + x];
				int bx = x + m_BMVX[idx + x];
				int by = y + m_BMVY[idx + x];
				// normal case
				if ((fx >= 0 && fx < width) && (fy >= 0 && fy < height)) {
					int fidx = fx + fy * width;
					m_occ_n_hole_Mask[fidx]++;
				}
				if ((bx >= 0 && bx < width) && (by >= 0 && by < height)) {
					int bidx = bx + by * width;
					m_occ_n_hole_Mask[bidx]++;
				}

				//inner block (not edge of block) case or no overlap
				if (overlapsize == 0)
					continue;
				if (!(fx - overlapsize >= 0 && fx + overlapsize < width) ||	
					!(fy - overlapsize >= 0 && fy + overlapsize < height)) 
					continue;
				if (!(bx - overlapsize >= 0 && bx + overlapsize < width) ||
					!(by - overlapsize >= 0 && by + overlapsize < height))
					continue;


				// horizontal case of box's edge
				if (x % blkW == 0) {
					for (int i = -overlapsize; i < 0; i++) {
						m_occ_n_hole_Mask[fx + i + fy*width]++;
						m_occ_n_hole_Mask[bx + i + by*width]++;
					}
				}
				if (x % blkW == (blkW - 1)) {
					for (int i = 1; i <= overlapsize; i++) {
						m_occ_n_hole_Mask[fx + i + fy*width]++;
						m_occ_n_hole_Mask[bx + i + by*width]++;
					}
				}
				// vertical case of box's edge 
				if (y % blkH == 0) {
					for (int j = -overlapsize; j < 0; j++) {
						m_occ_n_hole_Mask[fx + (fy + j)*width]++;
						m_occ_n_hole_Mask[bx + (by + j)*width]++;
					}
				}
				if (y % blkH == (blkH - 1)) {
					for (int j = 1; j <= overlapsize; j++) {
						m_occ_n_hole_Mask[fx + (fy + j)*width]++;
						m_occ_n_hole_Mask[bx + (by + j)*width]++;
					}
				}
			}
			idx += width;
		}

	}

	void reshapeMVMap(int width, int height, int frameInterval,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY) {
		int idx = 0;
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
		if (!m_FMVX)
			m_FMVX = new int[width * height];
		if (!m_FMVY)
			m_FMVY = new int[width * height];
		if (!m_BMVX)
			m_BMVX = new int[width * height];
		if (!m_BMVY)
			m_BMVY = new int[width * height];
		if (!m_occ_n_hole_Mask)
			m_occ_n_hole_Mask = new int[width * height];
		memset(m_occ_n_hole_Mask, 0, width*height*sizeof(int));
		memset(m_FMVX, 0, width*height*sizeof(int));
		memset(m_FMVX, 0, width*height*sizeof(int));
		memset(m_BMVX, 0, width*height*sizeof(int));
		memset(m_BMVY, 0, width*height*sizeof(int));
	}

	void overlap_set_pel(int out_idx, int ref_idx, uchar* ref_frame, int overlapsize) {
		
	}


	void basic_interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval, int overlapsize, bool doesHoleProc = false
		) {
		/*
			do not porcess hole regions
		*/
		this->reshapeMVMap(frameWidth, frameHeight, frame_interval, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		this->occ_n_hole_Mask(frameWidth, frameHeight, frame_interval, nBlkWidth, nBlkHeight, overlapsize);
		int idx = 0;
		int* pixel = new int[frameWidth*frameHeight];
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int bmx = x+m_BMVX[idx + x];
				int bmy = y+m_BMVY[idx + x];
				int fmx = x+m_FMVX[idx + x];
				int fmy = y+m_FMVY[idx + x];
				if (!((bmx < 0 || bmx >= frameWidth) || (fmy < 0 || fmy >= frameHeight))) {
					int b_idx = bmx + (fmy)*frameWidth;
					if (m_occ_n_hole_Mask[idx+x] != 0)
						pixel[idx + x] += prevFrame[b_idx];
					else {	//	hole
					}
				}
					

				if (!((fmx < 0 || fmx >= frameWidth) || (fmy < 0 || fmy >= frameHeight))) {
					int f_idx = fmx + (fmy)*frameWidth;
					if (m_occ_n_hole_Mask[idx + x] != 0) {
						pixel[idx + x] += nextFrame[f_idx];
					}
					else {	//	hole
					}
				}
					

				
			}
			idx += frameWidth;
		}
		if (doesHoleProc == true)
			this->hole_processing();
		idx = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				OUT_interpolFrame[idx + x] = pixel[idx + x] / m_occ_n_hole_Mask[idx + x];
			}
			idx += frameWidth;
		}
		delete [] pixel;
	}
	void hole_processing() {
		
	};

	


};



#endif