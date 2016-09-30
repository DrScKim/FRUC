#ifndef __FRM_INTERPOL_FB_CLASSIFY_H__
#define __FRM_INTERPOL_FB_CLASSIFY_H__

#include "FrameInterpolation.h"
#include "./FBClassify/FBClassifier.h"
#include "./HoleProcess/BDHI.h"

class FrameInterpolation_FBClassify : public FrameInterpolation
{



	void fillbackground(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame, FBClassifier& fbClassifier, int frameWidth, int frameHeight) {
		int* fb_table = fbClassifier.getTable();
		int idx = 0;
		int i = 0, j =0, k=0, l=0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int cl = fb_table[idx + x];
				switch (cl) {
				case SG_BACKGROUND:
					OUT_interpolFrame[idx + x] = prevFrame[idx + x];//(prevFrame[idx + x] >> 1) + (nextFrame[idx + x] >> 1);
					m_occ_n_hole_Mask_F[idx + x] = 0;
					m_occ_n_hole_Mask_B[idx + x] = 0;
					m_hole_table[idx + x] = 2;
					i++;
					break;
				
					/*
					case SG_BACKGROUND_FROM_BACKWARD:
					OUT_interpolFrame[idx + x] = prevFrame[idx + x];
					m_occ_n_hole_Mask_B[idx + x] = 0;
					m_hole_table[idx + x] = 2;
					k++;
					break;
					case SG_BACKGROUND_FROM_FORWARD:
					OUT_interpolFrame[idx + x] = nextFrame[idx + x];
					m_occ_n_hole_Mask_F[idx + x] = 0;
					l++;
					break;
				case SG_FOR_AVERAGING:
					OUT_interpolFrame[idx + x] = (prevFrame[idx + x] >> 1) + (nextFrame[idx + x] >> 1);
					m_occ_n_hole_Mask[idx + x] = 0;
					j++;
					break;
					*/
				default:
					;
				}
			}
			idx += frameWidth;
		}
		cout << "background : " << i << " Averaging : "<< j << endl;
		cout << "from backward : " << k << " from forward : " << l << endl;
		cout << "unprocessed : " << frameWidth*frameHeight - (i + j + k + l) << endl;
	}
public:
	FrameInterpolation_FBClassify() {
	}
	~FrameInterpolation_FBClassify() {
	}
	void basic_interpolation() {
	
	
	}


	void basic_interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval,
		int overlapsize, FBClassifier& fbClassifier, bool doesFBClassifierUse = false, bool doesHoleProc = false
		) 
	{
		memset(m_hole_table, 0, frameWidth * frameHeight * sizeof(int));
		this->reshapeMVMap(frameWidth, frameHeight, 1, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		this->occ_n_hole_Mask(frameWidth, frameHeight, frame_interval, nBlkWidth, nBlkHeight, overlapsize);
		
		if (doesFBClassifierUse == true)
			this->fillbackground(OUT_interpolFrame, prevFrame, nextFrame, fbClassifier, frameWidth, frameHeight);
			
		int idx = 0;
		int* hole_table = new int[frameWidth* frameHeight];
		// when false of fbclaissfier

		//
		unsigned int* pixel = new unsigned int[frameWidth*frameHeight];
		memset(pixel, 0, frameWidth*frameHeight*sizeof(int));
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int bmx = x + m_BMVX[idx + x];
				int bmy = y + m_BMVY[idx + x];
				// fill pixels from motion vector
				if (!((bmx < 0 || bmx >= frameWidth) || (bmy < 0 || bmy >= frameHeight))) {
					int b_idx = bmx + (bmy)*frameWidth;
					if (m_occ_n_hole_Mask_B[b_idx] != 0 ) {
						pixel[b_idx] = (int)prevFrame[idx + x];
						m_hole_table[b_idx] = 1;
						//m_occ_n_hole_Mask_B[b_idx] = 0;
					}
				}
				if (overlapsize == 0) continue;
			}
			idx += frameWidth;
		}
		idx = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int fmx = x + m_FMVX[idx + x];
				int fmy = y + m_FMVY[idx + x];
				// fill pixels from motion vector
				if (!((fmx < 0 || fmx >= frameWidth) || (fmy < 0 || fmy >= frameHeight))) {
					int f_idx = fmx + (fmy)*frameWidth;
					if (m_occ_n_hole_Mask_F[f_idx] != 0) {
						pixel[f_idx] += (int)nextFrame[idx + x];
						m_hole_table[f_idx] = 1;
						//m_occ_n_hole_Mask_F[f_idx] = 0;
					}
				}
			}
			idx += frameWidth;
		}

		idx = 0;
		for (int y = 0; y < frameHeight; y++) {	//
			for (int x = 0; x < frameWidth; x++) {
				if (m_hole_table[idx + x] == 1)
				OUT_interpolFrame[idx + x] = (pixel[idx + x] ) > 255 ?	255 : (pixel[idx + x]);
				
			}
			idx += frameWidth;
		}

		//if (doesHoleProc == true)
		this->hole_processing(m_hole_table, OUT_interpolFrame, frameWidth, frameHeight, frameWidth / 40, frameHeight / 40);

		//delete [] pixel;
	}

	void hole_processing(int* LUT, BYTE* pdst, int width, int height, int W_threshold, int H_threshold) {
		BDHI(LUT, pdst, width, width, height, W_threshold, H_threshold);
	};

};

#endif