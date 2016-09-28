#ifndef __FRM_INTERPOL_FB_CLASSIFY_H__
#define __FRM_INTERPOL_FB_CLASSIFY_H__

#include "FrameInterpolation.h"
#include "./FBClassify/FBClassifier.h"

class FrameInterpolation_FBClassify : public FrameInterpolation
{
	void fillbackground(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame, FBClassifier& fbClassifier, int frameWidth, int frameHeight) {
		int* fb_table = fbClassifier.getTable();
		int idx = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				switch (fb_table[idx + x]) {
				case SG_BACKGROUND:
					OUT_interpolFrame[idx + x] = (prevFrame[idx + x] >> 1) + (nextFrame[idx + x] >> 1);
					m_occ_n_hole_Mask[idx + x] = 0;
				case SG_BACKGROUND_FROM_FORWARD:
					OUT_interpolFrame[idx + x] = nextFrame[idx + x];
					m_occ_n_hole_Mask[idx + x] = 0;
				case SG_BACKGROUND_FROM_BACKWARD:
					OUT_interpolFrame[idx + x] = prevFrame[idx + x];
					m_occ_n_hole_Mask[idx + x] = 0;
				case SG_FOR_AVERAGING:
					OUT_interpolFrame[idx + x] = (prevFrame[idx + x] >> 1) + (nextFrame[idx + x] >> 1);
					m_occ_n_hole_Mask[idx + x] = 0;
				}
			}
			idx += 0;
		}
	}
public:
	FrameInterpolation_FBClassify() {
	}
	~FrameInterpolation_FBClassify() {
	}
	void basic_interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval,
		int overlapsize, FBClassifier& fbClassifier, bool doesFBClassfierUse = false, bool doesHoleProc = false
		) {
		this->reshapeMVMap(frameWidth, frameHeight, frame_interval, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		this->occ_n_hole_Mask(frameWidth, frameHeight, frame_interval, nBlkWidth, nBlkHeight, overlapsize);
		this->fillbackground(OUT_interpolFrame, prevFrame, nextFrame, fbClassifier, frameWidth, frameHeight);
		int idx = 0;
		int* pixel = new int[frameWidth*frameHeight];
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int bmx = x + m_BMVX[idx + x];
				int bmy = y + m_BMVY[idx + x];
				int fmx = x + m_FMVX[idx + x];
				int fmy = y + m_FMVY[idx + x];
				// fill pixels from motion vector
				if (!((bmx < 0 || bmx >= frameWidth) || (bmy < 0 || bmy >= frameHeight))) {
					int b_idx = bmx + (bmy)*frameWidth;
					if (m_occ_n_hole_Mask[idx + x] != 0 ) {
						pixel[idx + x] += prevFrame[b_idx];
					}
				}
				if (!((fmx < 0 || fmx >= frameWidth) || (fmy < 0 || fmy >= frameHeight))) {
					int f_idx = fmx + (fmy)*frameWidth;
					if (m_occ_n_hole_Mask[idx + x] != 0) {
						pixel[idx + x] += nextFrame[f_idx];
					}
				}
				// overlap region process
				if (overlapsize == 0) continue;
				/*
				if (!(fmx - overlapsize >= 0 && fmx + overlapsize < frameWidth) ||
					!(fmy - overlapsize >= 0 && fmy + overlapsize < frameHeight))	continue;
				if (!(bmx - overlapsize >= 0 && bmx + overlapsize < frameWidth) ||
					!(bmy - overlapsize >= 0 && bmy + overlapsize < frameHeight))	continue;
				*/
				
				if (x % nBlkWidth == 0) { // horizontal case of box's edge
					for (int i = -overlapsize; i < 0; i++) {
						if ( m_occ_n_hole_Mask[idx + x + i] != 0 ) {
							if (!((fmx+i < 0 || fmx+i >= frameWidth) || (fmy < 0 || fmy >= frameHeight)))
								pixel[idx + x + i] += prevFrame[fmx + i + fmy*frameWidth];
							if (!((bmx+i < 0 || bmx+i >= frameWidth) || (bmy < 0 || bmy >= frameHeight)))
								pixel[idx + x + i] += prevFrame[bmx + i + bmy*frameWidth];
						}
					}
				} else if (x % nBlkWidth == (nBlkWidth - 1)) {
					for (int i = 1; i <= overlapsize; i++) {
						if (m_occ_n_hole_Mask[idx + x + i] != 0) {
							pixel[idx + x + i] += prevFrame[fmx + i + fmy*frameWidth];
						}
					}
				}
				if (y % nBlkHeight == 0) {	// vertical case of box's edge 
					for (int j = -overlapsize; j < 0; j++) {
						if (!((fmx < 0 || fmx >= frameWidth) || (fmy + j < 0 || fmy + j >= frameHeight)))
							pixel[idx + x + j*frameWidth] += prevFrame[fmx + (fmy + j)*frameWidth];
						if (!((bmx < 0 || bmx >= frameWidth) || (bmy + j < 0 || bmy + j >= frameHeight)))
							pixel[idx + x + j*frameWidth] += prevFrame[bmx + (bmy + j)*frameWidth];
					}
				} else if (y % nBlkHeight == (nBlkHeight - 1)) {
					for (int j = 1; j <= overlapsize; j++) {
						if (!((fmx < 0 || fmx >= frameWidth) || (fmy + j < 0 || fmy + j >= frameHeight)))
							pixel[idx + x + j*frameWidth] += prevFrame[fmx + (fmy+j)*frameWidth];
						if (!((bmx < 0 || bmx >= frameWidth) || (bmy + j < 0 || bmy + j >= frameHeight)))
							pixel[idx + x + j*frameWidth] += prevFrame[bmx + (bmy+j)*frameWidth];
					}
				}
			}
			idx += frameWidth;
		}
		if (doesHoleProc == true)
			this->hole_processing();
		idx = 0;
		for (int y = 0; y < frameHeight; y++) {	//
			for (int x = 0; x < frameWidth; x++) {
				OUT_interpolFrame[idx + x] = pixel[idx + x] / m_occ_n_hole_Mask[idx + x];
			}
			idx += frameWidth;
		}
		delete[] pixel;
	}

};

#endif