#ifndef __PAMC_H__
#define __PAMC_H__
#include <iostream>
#include <vector>
#include "macroblock.h"

using namespace std;
class PMVS
{
	int checkSmall() {
		int i = 0;

		return i;
	}
public:
	static int getMVPred(std::vector<MacroBlockData>& mbs, int x, int y, int nBlkX, int &MVPredX, int &MVPredY) {
		MacroBlockData v1 = mbs[x - 1 + (y - 1)*nBlkX];
		MacroBlockData v2 = mbs[x + (y - 1)*nBlkX];
		MacroBlockData v4 = mbs[x + y * nBlkX];
		MVPredX = (v1.GetMV().x + v2.GetMV().x + v4.GetMV().x) / 3;
		MVPredY = (v1.GetMV().y + v2.GetMV().y + v4.GetMV().y) / 3;
		return 0;
	}
	static int getMVRefined(std::vector<MacroBlockData>& mbs, int MVpredX, int MVpredY, int x, int y, int nBlkX, int thr) {
		int nCnt = 0, vxRefined=0, vyRefiend=0;
		int diff[4] = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };
		int small_idx[4] = { -1, -1, -1, -1 };
		for (int y1 = y - 1; y1 <= y + 1; y1++) {
			for (int x1 = x - 1; x1 < x + 1; x1++) {
				int idx = x1 + y1 * nBlkX;
				int vx = mbs[idx].GetMV().x;
				int vy = mbs[idx].GetMV().y;
				
				int dmv = std::abs(vx - MVpredX) + std::abs(vy - MVpredY);

				for (int i = 0; i < 4; i++) {
					if (diff[i] > dmv) {
						diff[i] = dmv;
						small_idx[i] = idx;
						break;
					}
				}
				
			}
		}
		int vx = 0, vy = 0;
		int ncnt = 0;
		for (int i = 0; i < 4; i++) {
			if (diff[i] > thr) continue;
			vx += mbs[small_idx[i]].GetMV().x;
			vy += mbs[small_idx[i]].GetMV().y;
			ncnt++;
		}
		if ( ncnt >0)
			mbs[x + y * nBlkX].updateMV(vx / ncnt, vy / ncnt);
		return 0;
	}

	static void smoothing(std::vector<MacroBlockData>& mbs, int nBlkX, int nBlkY, int threshold) {
		for (int y = 1; y < nBlkY - 1; y++) {
			for (int x = 1; x < nBlkX - 1; x++){
				int MVPredX = 0, MVPredY = 0;
				getMVPred(mbs, x, y, nBlkX, MVPredX, MVPredY);
				getMVRefined(mbs, MVPredX, MVPredY, x, y, nBlkX, threshold);
				//cout << x << ", " << y << " vector smoothed " << endl;
			}
		}
	}

};


#endif