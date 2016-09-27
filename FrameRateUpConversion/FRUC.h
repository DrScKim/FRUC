

#ifndef __FRUC_H__
#define __FRUC_H__

#include <direct.h>
#include "FrameInfo.h"
#include "FrameInterpolation.h"
class FRUC
{
	Mat curFrame;
	Mat nextFrame;

	char save_directory[_MAX_PATH];
	char load_directory[_MAX_PATH];
	char save_filename[_MAX_PATH];
	char save_extension[_MAX_PATH];
	char interpol_path[_MAX_PATH];
	char curFrame_path[_MAX_PATH];
	char nextFrame_path[_MAX_PATH];
	int m_file_idx;
	int m_frame_interval;
	int m_overlap_size;
	FrameInfo frameInfo;
	FrameInterpolation interpolator;
public:
	
	FRUC() : m_file_idx(0), m_frame_interval(2), m_overlap_size(2) {
	}
	~FRUC() {
		if (curFrame.data)
			curFrame.release();
		if (nextFrame.data)
			nextFrame.release();
	}
	void setup(int blkW, int blkH, int searchRange, int frame_interval,
		int files_start_idx, char* load_dir, char* save_dir, char* file_ext) {
		frameInfo.initialize(blkW, blkH, searchRange);
	}
	void path_setup(int idx) {
		sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
		sprintf(interpol_path, "%s/%d.%s", save_directory, idx+1, save_extension);
		sprintf(nextFrame_path, "%s/%d.%s", load_directory, idx + 2, save_extension);
	}
	void do_fruc() {
		int file_idx = m_file_idx;
		path_setup(file_idx);
		curFrame = imread(curFrame_path);
		nextFrame = imread(nextFrame_path);
		while (curFrame.data != NULL || nextFrame.data != NULL) {
			Mat interpol = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
			int nCols, nRows;
			Mat mvfx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, true, 0.5);
			Mat mvfy = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, false, 0.5);
			Mat mvbx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, true, 0.5);
			Mat mvby = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, false, 0.5);
			frameInfo.motion_estimate(curFrame, nextFrame);
			interpolator.interpolation(interpol.data, frameInfo.getCurYFrameData(), frameInfo.getNextYFrameData(),
				mvfx.data, mvfy.data, mvbx.data, mvby.data, curFrame.cols, curFrame.rows,
				frameInfo.getBlkWidth(), frameInfo.getBlkHeight(), m_frame_interval, m_overlap_size);
			imwrite(interpol_path, interpol);
			file_idx += m_frame_interval;
			path_setup(file_idx);
		}
	
	}





};


#endif //__FRUC_H__