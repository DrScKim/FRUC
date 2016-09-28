

#include "FRUC.h"

inline void test_frame_interpolation(int idx, char* load_dir, char* save_dir, char* file_ext, 
	int blkSizeW, int blkSizeH, int searchRange, int frame_interval, int overlap_size)
{

	FrameRateUpConverter converter;
	converter.setup(blkSizeW, blkSizeH, searchRange, frame_interval, overlap_size, idx, load_dir, save_dir, file_ext);
	converter.do_fruc();

}