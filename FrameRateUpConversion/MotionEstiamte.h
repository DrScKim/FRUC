
#ifndef __ESTIMATE_H__
#define __ESTIMATE_H__

#include <iostream>
#include <vector>
#include "macroblock.h"
#include "math_util.h"
#include "PMVS.h"

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
	inline void smoothing(int nBlkX, int nBlkY) {


		for (int y = 0; y < nBlkY; y++) {
			for (int x = 0; x < nBlkX; x++) {
				m_mbs[x + y*nBlkX];
				//m_mbs[]
			}
		}
	}
	inline int estimate(const uint8* srcFrame, const uint8* dstFrame, int frameW, int frameH, int blkW, int blkH, int search_range, int threshold = 10) {
		if (m_mbs.size() == 0) {
			cout << "No Align Macroblock!" << endl;
			return ERROR_NO_ALIGN_MACROBLOCKS;
		}
		#pragma omp parallel for
		for (int y = 0; y < frameH; y += blkH) {
			for (int x = 0; x < frameW; x += blkW) {
				int blkIdx = x / blkW + (y / blkH) * m_nBlkX;
				m_mbs[blkIdx].initialize(x, y, blkW, blkH);
				for (int dx = -search_range; dx <= search_range; dx++) {
					for (int dy = -search_range; dy <= search_range; dy++) {
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
		// TODO: PMV
		//PMVS::smoothing(m_mbs, frameW / blkW, frameH / blkH, 3);
		return DONE;
	}
};


#endif // __ESTIMATE_H__