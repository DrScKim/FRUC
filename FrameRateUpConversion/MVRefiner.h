
#include "MotionEstiamte.h"


class MVRefiner
{
	MotionEstimator m_blk16_x16;
	MotionEstimator m_blk8_x8;


	void refine(MotionEstimator& largeBlks, MotionEstimator& smallBlks){


	}

	vector<MacroBlockData> neighbourBlock(int i, int j, int width, vector<MacroBlockData>& largeBlks) {
		vector<MacroBlockData> nblks;
		for(int x = i-1; x <= i+1; x++) {
			for (int y = j-1; y <= j+1; y++) {
				MacroBlockData b(largeBlks[x + y*width]);
				nblks.push_back(b);
			}
		}
		return nblks;
	}

	MacroBlockData small_region_search(vector<MacroBlockData>& nvBlks, int x, int y, int smallBlkW, int smallBlkH, uchar* srcFrame, uchar* refFrame, int width, int blkSizeX, int blkSizeY, int search_range = 4) {
		MacroBlockData b(x, y, blkSizeX, blkSizeY);
		float d = (float)(blkSizeX*blkSizeY);
		for (vector<MacroBlockData>::iterator iter = nvBlks.begin(); iter != nvBlks.end(); iter++) {
			MacroBlockData blk = *iter;
			int mvx = blk.GetMV().x, mvy = blk.GetMV().y;
			for (int x1 = x + mvx - search_range; x1 <= x + mvx + search_range; x1++) {
				for (int y1 = y + mvy - search_range; y1 <= y + mvy + search_range; y1++) {
					int smallSAD = 0;
					for (int i = 0; i < blkSizeX; i++) {
						for (int j = 0; j < blkSizeY; j++) {
							smallSAD += std::abs(srcFrame[(x+i) + (y+j)*width] - refFrame[(x1+i) + (y1+j)*width]);
						}
					}
					float SADW = smallSAD / d;
					if (b.GetSAD() > SADW) {
						b.updateMV(x1, y1, SADW);
					}
				}
			}
		}
		return b;

	}





};