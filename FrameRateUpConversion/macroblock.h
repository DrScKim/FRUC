#ifndef __MACROBLOCK_H__
#define __MACROBLOCK_H__

#include "me_util.h"
#include "motionVector.h"

class MacroBlockData {
	int x;
	int y;
	int w;
	int h;
	VECTOR vector;
public:
	MacroBlockData() : x(0), y(0), w(0), h(0) {}
	MacroBlockData(int _x, int _y, int _w, int _h) { 
		this->x = _x; this->y = _y; this->w = _w; this->h = _h; 
		vector.coord[0] = _x; vector.coord[1] = _y; vector.sad = INT_MAX; 
	}
	~MacroBlockData() {}
	inline int initialize(int _x, int _y, int _w, int _h) { 
		this->x = _x; this->y = _y; this->w = _w; this->h = _h; 
		vector.sad = INT_MAX; 
		return 0;
	}
	inline int X() const { return x; }
	inline int Y() const { return y; }
	inline int W() const { return w; }
	inline int H() const { return h; }
	inline VECTOR GetMV() const { return vector; }
	inline int GetSAD() const { return vector.sad; }
	inline void updateMV(int _dx, int _dy) { this->vector.x = _dx; this->vector.y = _dy; }
	inline void updateMV(int _dx, int _dy, int _sad) { this->vector.x = _dx; this->vector.y = _dy; this->vector.sad = _sad; }
};



#endif // __MACROBLOCK_H__