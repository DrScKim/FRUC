
#ifndef __ESTIMATE_H__
#define __ESTIMATE_H__

#include <iostream>
#include <vector>
#include "macroblock.h"
#include "math_util.h"

#include <omp.h>

#define DONE	0

#define ERROR_NO_ALIGN_MACROBLOCKS	1
#define ERROR_NOT_AVAILABLE_MACROBLOCK_SIZE	2
typedef unsigned char uint8_t;
class MotionEstimator {
	std::vector<MacroBlockData> m_mbs;
	int m_nBlkX;
	int m_nBlkY;

	inline bool isAvailableRange(int x, int y, int dx, int dy, int blkW, int blkH, int frameW, int frameH) {
		if (x + dx < 0)
			return false;
		else if (x + dx + blkW > frameW)
			return false;
		else if (y + dy < 0)
			return false;
		else if (y + dy + blkH > frameH)
			return false;
		return true;
	}
public:
	MotionEstimator() {}
	~MotionEstimator() {}

	inline int initBlock(int blkX, int blkY) {
		if (blkX == 0 || blkY == 0)
			return ERROR_NOT_AVAILABLE_MACROBLOCK_SIZE;
		m_nBlkX = blkX; m_nBlkY = blkY;
		for (int i = 0; i < m_nBlkX*m_nBlkY; i++){
			MacroBlockData block;
			m_mbs.push_back(block);
		}
		return DONE;
	}
	inline std::vector<MacroBlockData>* getMacroBlockList() { return &m_mbs; }
	inline MacroBlockData getBlock(int col, int row) { return m_mbs[col + row*m_nBlkX]; }
	inline void reset() {
		m_mbs.clear();
	}
	inline int estimate(const uint8* srcFrame, const uint8* dstFrame, int frameW, int frameH, int blkW, int blkH, int search_range, int threshold = 10) {
		if (m_mbs.size() == 0) {
			cout << "No Align Macroblock!" << endl;
			return ERROR_NO_ALIGN_MACROBLOCKS;
		}
		#pragma omp parallel for
		for (int y = 0; y < frameH; y += blkH)
		{
			for (int x = 0; x < frameW; x += blkW) 
			{
				int blkIdx = x / blkW + (y / blkH) * m_nBlkX;
				m_mbs[blkIdx].initialize(x, y, blkW, blkH);
				for (int dx = -search_range; dx <= search_range; dx++) 
				{
					for (int dy = -search_range; dy <= search_range; dy++)
					{
						if (!isAvailableRange(x, y, dx, dy, blkW, blkH, frameW, frameH)) 
							continue;
						int SAD = calcSAD(srcFrame + x + (y *frameW), dstFrame + x + dx + (y + dy)*frameW, blkW, blkH, frameW);
						
						if (m_mbs[blkIdx].GetSAD() > SAD && SAD > 3) {
							m_mbs[blkIdx].updateMV(dx, dy, SAD);
						}
					}
				}
			}
		}
		return DONE;
	}

	// it is old pre 1.8 version
	void MakeVectorOcclusionMask(int nBlkX, int nBlkY,  uint8_t * occMask, int width)
	{	// analyse vectors field to detect occlusion
		_mm_empty();
		double occnorm = 10; // empirical


		for (int by = 0; by < nBlkY; by++) {
			for (int bx = 0; bx < nBlkX; bx++) {
				
			}
		}

		for (std::vector<MacroBlockData>::iterator iter = m_mbs.begin(); iter != m_mbs.end(); iter++) {
			
		}
		/*
		for (int by = 0; by<nBlkY; by++)
		{
			for (int bx = 0; bx<nBlkX; bx++)
			{
				int occlusion = 0;
				int i = bx + by*nBlkX; // current block
				const FakeBlockData &block = mvClip.GetBlock(0, i);
				int vx = block.GetMV().x;
				int vy = block.GetMV().y;
				if (bx > 0) // left neighbor
				{
					int i1 = i - 1;
					const FakeBlockData &block1 = mvClip.GetBlock(0, i1);
					int vx1 = block1.GetMV().x;
					//int vy1 = block1.GetMV().y;
					if (vx1>vx) occlusion += (vx1 - vx); // only positive (abs)
				}
				if (bx < nBlkX - 1) // right neighbor
				{
					int i1 = i + 1;
					const FakeBlockData &block1 = mvClip.GetBlock(0, i1);
					int vx1 = block1.GetMV().x;
					//int vy1 = block1.GetMV().y;
					if (vx1<vx) occlusion += vx - vx1;
				}
				if (by > 0) // top neighbor
				{
					int i1 = i - nBlkX;
					const FakeBlockData &block1 = mvClip.GetBlock(0, i1);
					//int vx1 = block1.GetMV().x;
					int vy1 = block1.GetMV().y;
					if (vy1>vy) occlusion += vy1 - vy;
				}
				if (by < nBlkY - 1) // bottom neighbor
				{
					int i1 = i + nBlkX;
					const FakeBlockData &block1 = mvClip.GetBlock(0, i1);
					//int vx1 = block1.GetMV().x;
					int vy1 = block1.GetMV().y;
					if (vy1<vy) occlusion += vy - vy1;
				}
				
				occMask[bx + by*width] =
						std::min(int(255 * occlusion*occnorm), 255);
				
			}
		}
		*/
	}


};


#endif // __ESTIMATE_H__