#ifndef __FRAMEINTERPOL_FB_DP_H__
#define __FRAMEINTERPOL_FB_DP_H__

#include "FrameInterpol_fbClassify.h"
#include "DiffPixelMap.h"

#include "IPHI.h"

class FrameInterpolFBDP : public FrameInterpolation_FBClassify
{
	uchar* m_diff_table;
	void fillbackground(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame, FBClassifier& fbClassifier, int frameWidth, int frameHeight) {
		int* fb_table = fbClassifier.getTable();
		int idx = 0;
		int i = 0, j = 0, k = 0, l = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int cl = fb_table[idx + x];
				switch (cl) {
				case SG_BACKGROUND:
					OUT_interpolFrame[idx + x] = (prevFrame[idx + x] + nextFrame[idx + x]) >> 1;//prevFrame[idx + x];
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
					m_hole_table[idx + x] = 2;
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
	}
	void fillbackground2(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* srcFrame, uchar* refFrame, uchar* postFrame,
		FBClassifier& fbClassifier, int frameWidth, int frameHeight, uchar* diff_table) {
		int* fb_table = fbClassifier.getTable();
		int idx = 0;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int cl = fb_table[idx + x];

				switch (cl) {

				case SG_BACKGROUND:

					OUT_interpolFrame[idx + x] = (srcFrame[idx + x] + refFrame[idx + x]) >> 1;//prevFrame[idx + x];

					m_hole_table[idx + x] = 3;
					m_diff_table[idx + x] = std::abs(srcFrame[idx + x] - refFrame[idx + x]);
					break;
					
				case SG_BACKGROUND_FROM_BACKWARD:
					OUT_interpolFrame[idx + x] = (prevFrame[idx + x] + 3*srcFrame[idx + x]) >> 2;
					m_hole_table[idx + x] = 3;
					m_hole_table[idx + x] = 3;
					break;

				case SG_BACKGROUND_FROM_FORWARD:
					OUT_interpolFrame[idx + x] = (postFrame[idx + x] + 3*refFrame[idx + x]) >> 2;
					m_hole_table[idx + x] = 3;
					break;
					
				default:
					;
				}
			}
			idx += frameWidth;
		}
	}
public:
	FrameInterpolFBDP() {
	}
	~FrameInterpolFBDP() {
	}
	void neighbour_mv_interpolation() {
	
	
	}


	void basic_interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* srcFrame, uchar* refFrame, uchar* postFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		uchar* sadmapF, uchar* sadmapB, 
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval,
		FBClassifier& fbClassifier, uint8 byte_sad_weight=127, bool doesFBClassifierUse = false, bool doesHoleProc = false
		) {
		DiffPixelMap dpMapF(frameWidth, frameHeight, srcFrame, refFrame, forward_mvX, forward_mvY);
		DiffPixelMap dpMapB(frameWidth, frameHeight, refFrame, srcFrame, backward_mvX, backward_mvY);
		m_diff_table = new uchar[frameWidth*frameHeight];
		//memset(m_hole_table, PEL_HOLE, frameWidth*frameHeight*sizeof(int));

		for (int i = 0; i < frameWidth*frameHeight; i++) {
			m_hole_table[i] = PEL_UNFILLED;
		}

		for (int i = 0; i < frameWidth; i++) {
			for (int j = 0; j < frameHeight; j++) {
				OUT_interpolFrame[i + j*frameWidth] = (srcFrame[i + j*frameWidth] + refFrame[i + j*frameWidth]) >> 1;
			}
		}

		memset(m_diff_table, 255, frameWidth*frameHeight*sizeof(uchar));
		this->reshapeMVMap(frameWidth, frameHeight, 1, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		int idx = 0;


		
		this->fillbackground2(OUT_interpolFrame, prevFrame, srcFrame, refFrame, postFrame, fbClassifier, frameWidth, frameHeight, m_diff_table);
		

		//fill pixel in the interpolated frame using forward motion vector prev->next, source is prev)
		fillPixelFromVector2(dpMapF.getDiffPixelMap(), sadmapF, OUT_interpolFrame, srcFrame, refFrame, byte_sad_weight,
			m_FMVX, m_FMVY, frameWidth, frameHeight, frame_interval, m_diff_table, m_hole_table, true);
			
		//fill pixel in the interpolated frame using backward motion vector prev<-next, source is next)
		/*
		fillPixelFromVector2(dpMapB.getDiffPixelMap(), sadmapB, OUT_interpolFrame, refFrame, srcFrame, byte_sad_weight,
			m_BMVX, m_BMVY, frameWidth, frameHeight, frame_interval, m_diff_table, m_hole_table, false);
		*/
		
		//if (doesFBClassifierUse == true)
		
		//if (doesHoleProc == true)
		
		//this->fillbackground(OUT_interpolFrame, srcFrame, refFrame, fbClassifier, frameWidth, frameHeight);
		
		//this->hole_processing(m_hole_table, OUT_interpolFrame, frameWidth, frameHeight, frameWidth / 40, frameHeight / 40);
		//BDHI(m_hole_table, OUT_interpolFrame, frameWidth, frameWidth, frameHeight, frameWidth / 40, frameHeight / 40);
		IPHI(m_hole_table, frameWidth, frameHeight, OUT_interpolFrame);
		delete[] m_diff_table;
		
	}

	void fillPixelFromVector2(uchar* dpPixelMap, uchar* SADMap, uchar* OUT_interpolFrame, uchar* sourceFrame, uchar* referenceFrame, uint8 byte_sad_weight,
		int* mvX, int* mvY, int frameWidth, int frameHeight, int frame_interval, uchar* diff_table, int* hole_table, bool isForward) {
	
		uchar* dpmap = dpPixelMap, *sadmap = SADMap, *prev = sourceFrame, *ref = referenceFrame;
		int* vx = mvX, *vy = mvY;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {

				//int ridx_x = std::max(std::min(x + 2 * vx[x], frameWidth-1),0);
				//int ridx_y = std::max(std::min(y + 2 * vy[x], frameHeight - 1), 0);

				int ridx_x = x + 2 * vx[x];
				int ridx_y = y + 2 * vy[x];


				int ridx = x + 2 * vx[x] + (y + 2 * vy[x])*frameWidth;

				int idx = x + vx[x] + (y + vy[x])*frameWidth;
				int sad = sadmap[x];
				int dp = dpmap[x];
				int diff = (((255 - byte_sad_weight)*(dp)+byte_sad_weight*(sad)) >> 8);
				// the fast version of Equation : ((1-w)*diff_pixel)/2 + ((w)*sad)/2 , w is weight value from 0.0 to 1.0
				if (diff_table[idx] > diff && m_hole_table[idx] != 2 && diff < 8 ) {
					
					if (std::abs(sourceFrame[idx] - referenceFrame[idx]) < diff) {
						OUT_interpolFrame[idx] = (sourceFrame[idx] + referenceFrame[idx]) >> 1;
						diff_table[idx] = abs(prev[idx] - ref[idx]);
						m_hole_table[idx] = PEL_FILLED;
					}
					else {
						OUT_interpolFrame[idx] = (3 * prev[x] + referenceFrame[ridx]) >> 2;
						diff_table[idx] = diff;
						m_hole_table[idx] = PEL_FILLED;
					}

					if (vx[x] == 0 && vy[y] == 0) {
						int p = (int)prev[x], r = (int)ref[x];
						OUT_interpolFrame[idx] = (uchar)((3 * p + r) >> 2);
						diff_table[idx] = diff;
						m_hole_table[idx] = PEL_FILLED;
					}
					
					else {
						
						
						
					}
					//cout << "fill it" << endl;

				}
			}
			sadmap += frameWidth;
			dpmap += frameWidth;
			vx += frameWidth;
			vy += frameWidth;
			prev += frameWidth;
			ref += frameWidth;
		}
		
	
	}

	void fillPixelFromVector(uchar* dpPixelMap, uchar* SADMap, uchar* OUT_interpolFrame, uchar* sourceFrame, uchar* referenceFrame, uint8 byte_sad_weight,
		int* mvX, int* mvY, int frameWidth, int frameHeight, int frame_interval, uchar* diff_table, int* hole_table, bool isForward) {
		/*			frame sequence is source -> reference		*/
		uchar* dpmap = dpPixelMap, *sadmap = SADMap, *prev = sourceFrame, *ref = referenceFrame;
		int* vx = mvX, *vy = mvY;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int idx = x + vx[x] + (y + vy[x])*frameWidth;
				int sad = sadmap[x];
				int dp = dpmap[x];
				int diff = (((255 - byte_sad_weight)*(dp) + byte_sad_weight*(sad)) >> 8); 
				// the fast version of Equation : ((1-w)*diff_pixel)/2 + ((w)*sad)/2 , w is weight value from 0.0 to 1.0
				if (diff_table[idx] > diff && m_hole_table[idx] != 2) {
					if (vx[x] == 0 && vy[y]==0) {
						int p = (int)prev[x], r = (int)ref[x];
						OUT_interpolFrame[idx] = (uchar)((3 * p + r) >> 2);
						diff_table[idx] = diff;
						m_hole_table[idx] = PEL_FILLED;
					}
					else {
						OUT_interpolFrame[idx] = (3 * prev[x] + ref[x]) >> 2;
						//OUT_interpolFrame[idx] = prev[x];
						diff_table[idx] = diff;
						m_hole_table[idx] = PEL_FILLED;
					}
					//cout << "fill it" << endl;
					
				}
			}
			sadmap += frameWidth;
			dpmap += frameWidth;
			vx += frameWidth;
			vy += frameWidth;
			prev += frameWidth;
			ref += frameWidth;
		}
	}
	void hole_processing(int* LUT, uchar* pdst, int width, int height, int W_threshold, int H_threshold) {
		BDHI(LUT, pdst, width, width, height, W_threshold, H_threshold);
	};

};

#endif