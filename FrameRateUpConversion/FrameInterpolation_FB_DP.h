#ifndef __FRAMEINTERPOL_FB_DP_H__
#define __FRAMEINTERPOL_FB_DP_H__

#include "FrameInterpol_fbClassify.h"
#include "DiffPixelMap.h"


class FrameInterpolFBDP : public FrameInterpolation_FBClassify
{
	int* m_diff_table;
public:
	FrameInterpolFBDP() {
	}
	~FrameInterpolFBDP() {
	}
	void basic_interpolation(uchar* OUT_interpolFrame, uchar* prevFrame, uchar* nextFrame,
		uchar* forward_mvX, uchar* forward_mvY, uchar* backward_mvX, uchar* backward_mvY,
		uchar* sadmapF, uchar* sadmapB, 
		int frameWidth, int frameHeight, int nBlkWidth, int nBlkHeight, int frame_interval,
		int overlapsize, FBClassifier& fbClassifier, uint8 byte_sad_weight=127, bool doesFBClassifierUse = false, bool doesHoleProc = false
		) {
		DiffPixelMap dpMapF(frameWidth, frameHeight);
		DiffPixelMap dpMapB(frameWidth, frameHeight);
		//SADMAP sadMapF;
		uchar* sadmap;
		unsigned int* pixel = new unsigned int[frameWidth*frameHeight];
		memset(pixel, 0, frameWidth*frameHeight*sizeof(int));
		memset(m_diff_table, 255, frameWidth*frameHeight*sizeof(int));
		this->reshapeMVMap(frameWidth, frameHeight, 1, forward_mvX, forward_mvY, backward_mvX, backward_mvY);
		int idx = 0;
		int* hole_table = new int[frameWidth* frameHeight];

		
		//fill pixel in the interpolated frame using forward motion vector prev->next, source is prev)
		fillPixelFromVector(dpMapF.getDiffPixelMap(), sadmapF, OUT_interpolFrame, prevFrame, byte_sad_weight,
			m_FMVX, m_FMVY, frameWidth, frameHeight, frame_interval, m_diff_table, m_hole_table);
		//fill pixel in the interpolated frame using backward motion vector prev<-next, source is next)
		fillPixelFromVector(dpMapB.getDiffPixelMap(), sadmapB, OUT_interpolFrame, nextFrame, byte_sad_weight,
			m_BMVX, m_BMVY, frameWidth, frameHeight, frame_interval, m_diff_table, m_hole_table);
		

		if (doesFBClassifierUse == true)
			this->fillbackground(OUT_interpolFrame, prevFrame, nextFrame, fbClassifier, frameWidth, frameHeight);
		if (doesHoleProc == true)
			this->hole_processing(m_hole_table, OUT_interpolFrame, frameWidth, frameHeight, frameWidth / 40, frameHeight / 40);

	}
	void fillPixelFromVector(uchar* dpPixelMap, uchar* SADMap, uchar* OUT_interpolFrame, uchar* sourceFrame, uint8 byte_sad_weight,
		int* mvX, int* mvY, int frameWidth, int frameHeight, int frame_interval, int* diff_table, int* hole_table) {
		/*			frame sequence is source -> reference		*/
		uchar* dpmap = dpPixelMap, *sadmap = SADMap, *prev = sourceFrame;
		int* vx = mvX, *vy = mvY;
		for (int y = 0; y < frameHeight; y++) {
			for (int x = 0; x < frameWidth; x++) {
				int idx = x + vx[x] + (y + vy[x])*frameWidth;
				int sad = sadmap[x];
				int dp = dpmap[x];
				int diff = (((255 - byte_sad_weight)*(dp>>1) + byte_sad_weight*(sad >> 1)) >> 8); 
				// the fast version of Equation : ((1-w)*diff_pixel)/2 + ((w)*sad)/2 , w is weight value from 0.0 to 1.0
				if (diff_table[idx] > diff) {
					OUT_interpolFrame[idx] = prev[x];
					diff_table[idx] = diff;
					hole_table[idx] = 1;
				}
			}
			sadmap += frameWidth;
			dpmap += frameWidth;
			vx += frameWidth;
			vy += frameWidth;
			prev += frameWidth;
		}

	}
	void fillPixelFromBackwardVector() {

	}


};

#endif