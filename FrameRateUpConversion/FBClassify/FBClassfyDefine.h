#pragma once

#define PEL_OCCLUSION	0
#define PEL_FILLED	1
#define PEL_FILLED_FROM_FORWARD	2
#define PEL_FILLED_FROM_BACKWARD	3
#define PEL_HOLE	4
#define PEL_UNFILLED	-1
#define PEL_FILLED_BY_BDHI	5

#define OCCLUSION	1
#define NOT_OCCLUSION	0

typedef unsigned char BYTE;

#define SGFRUC_OCCLUSION_DETECT	11
#define SGFRUC_OCCLUSION_INTERPOLATION 12

#define SGMIN(x,y)	(x>y ? y:x)
#define SGMAX(x,y)	(x<y ? y:x)
#define SGABS(x)	(x<0 ? -x:x)

#define THR(x, thr) (x > thr ? x : 0)

#define	FROM_NEXT_BACKGROUND		1000001
#define	FROM_BACK_BACKGROUND		1000002
#define SG_FOREGROUND				1000003
#define SG_BACKGROUND				1000004
#define SG_NOT_MOVED				1000005
#define SG_OBJECT					1000006
#define SG_NOT_CERTIFIED_FROM_BACKWARD			1000007


#define SG_OBJECT_FROM_BACKWARD		1000008
#define SG_OBJECT_FROM_FORWARD		1000009
#define SG_BACKGROUND_FROM_BACKWARD	1000010
#define SG_BACKGROUND_FROM_FORWARD	1000011
#define SG_FOR_BLENDING				1000012	
#define SG_NOT_CERTIFIED_FROM_FORWARD			1000013
#define SG_NOT_CERTIFIED_APPEAR						1000014
#define SG_FOR_BLENDING_MV			1000015
#define SG_FOR_AVERAGING			1000016

#define PARENTIDX(x,y,blkx)	x/2 + (y/2)*(blkx/2)
#define IDX(idx,maximum)	SGMAX(0,SGMIN(idx,maximum-1))
#define PIXEL_IDX(w,vx,h,vy,width)		(w+vx + width*(h+vy))
#define IDX2(w,h,width)	(w+h*width);

#define FRUC_MODE_INTERPOLATION	0
#define FRUC_MODE_BLENDING	1
#define FRUC_MODE_COPYING	2
