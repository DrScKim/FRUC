#ifndef __DIFFPIXELMAP_H__
#define __DEFFPIXELMAP_H__

#include <iostream>
#include <omp.h>
typedef unsigned char uchar;
typedef unsigned char uint8;
class DiffPixelMap {

	int m_width;
	int m_height;
	uchar *m_diffPixelMap;


public:
	DiffPixelMap(int width, int height) : m_width(width), m_height(height) {
		m_diffPixelMap = new uchar[width*height];
		memset(m_diffPixelMap, 0, sizeof(int) * m_width * m_height);
	}
	~DiffPixelMap() {
		if (m_diffPixelMap) {
			delete[]  m_diffPixelMap;
		}
	}
	void resetMap() { 
		memset(m_diffPixelMap, 0, sizeof(int) * m_width * m_height); 
	}
	
	inline void calc_diff_pixel_map(uchar* ref_frame, uchar* src_frame, uchar* mvxMap, uchar* mvyMap) {
		uchar* mvx = mvxMap; uchar* mvy = mvyMap;
		for (int y = 0; y < m_height; y++) {
			#pragma parallel omp for
			for (int x = 0; x < m_width; x++) {
				int vx = mvx[x] - 128;
				int vy = mvy[x] - 128;
				int ridx = vx + x + (y + vy)*m_width;
				int sidx = x + y + m_width;
				m_diffPixelMap[x] = std::abs(src_frame[sidx] - ref_frame[ridx]);
			}
			mvx += m_width;
			mvy += m_width;
		}
	}
	uchar* getDiffPixelMap() {
		return m_diffPixelMap;
	}
};




#endif