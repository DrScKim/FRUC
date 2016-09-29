

#ifndef __FRUC_H__
#define __FRUC_H__

#include <direct.h>
#include "FrameInfo.h"
#include "FrameInterpolation.h"
#include "FrameInterpol_fbClassify.h"
//#include "./FBClassify/FBClassifier.h"

class FrameRateUpConverter
{
	Mat curFrame;
	Mat nextFrame;

	char save_directory[_MAX_PATH];
	char load_directory[_MAX_PATH];
	char save_filename[_MAX_PATH];
	char save_extension[_MAX_PATH];
	char interpol_path[_MAX_PATH];
	char prevFrame_path[_MAX_PATH];
	char curFrame_path[_MAX_PATH];
	char nextFrame_path[_MAX_PATH];
	char postFrame_path[_MAX_PATH];
	int m_file_idx;
	int m_frame_interval;
	int m_overlap_size;
	FrameInfo frameInfo;
	
public:
	
	FrameRateUpConverter() : m_file_idx(0), m_frame_interval(2), m_overlap_size(2) {

	}
	~FrameRateUpConverter() {
		if (curFrame.data)
			curFrame.release();
		if (nextFrame.data)
			nextFrame.release();
	}
	void setup(int blkW, int blkH, int searchRange, int frame_interval, int overlap_size,
		int files_start_idx, char* load_dir, char* save_dir, char* file_ext) {
		frameInfo.initialize(blkW, blkH, searchRange);
		m_file_idx = files_start_idx;
		m_frame_interval = frame_interval;
		m_overlap_size = overlap_size;
		sprintf(save_directory, save_dir);
		sprintf(load_directory, load_dir);
		sprintf(save_extension, file_ext);
	}
	void path_setup(int idx) {
		sprintf(prevFrame_path, "%s/%d.%s", load_directory, idx-2, save_extension);
		sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
		sprintf(interpol_path, "%s/%d.%s", save_directory, idx+1, save_extension);
		sprintf(nextFrame_path, "%s/%d.%s", load_directory, idx + 2, save_extension);
		sprintf(postFrame_path, "%s/%d.%s", load_directory, idx + 4, save_extension);
	}
	void do_fruc_with_fbclassify() {
		FrameInterpolation_FBClassify interpolator;
		FrameInterpolation base_interpolator;
		FBClassifier fbClassifier;
		path_setup(m_file_idx);
		
		
		curFrame = imread(curFrame_path);
		nextFrame = imread(nextFrame_path);
		interpolator.init(curFrame.cols, curFrame.rows);

		Mat prevYFrame = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0)),
			postYFrame = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
		Mat prevFrame = imread(prevFrame_path);
		Mat postFrame = imread(postFrame_path);
		/*
		cvtColor(prevFrame, prevFrame, CV_BGR2YUV);
		cvtColor(postFrame, postFrame, CV_BGR2YUV);
		
		ColorConverter::YUV2Y(prevFrame, prevYFrame);
		ColorConverter::YUV2Y(postFrame, postYFrame);
		*/

		//first frame
		if (prevFrame.data == NULL && curFrame.data != NULL && nextFrame.data != NULL) {
			Mat interpol = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
			int nCols = curFrame.cols / frameInfo.getBlkWidth(), nRows = curFrame.rows / frameInfo.getBlkHeight();
			frameInfo.motion_estimate(curFrame, nextFrame);
			Mat mvfx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, true, 0.5);
			Mat mvfy = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, false, 0.5);
			Mat mvbx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, true, 0.5);
			Mat mvby = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, false, 0.5);
			interpolator.basic_interpolation(interpol.data, frameInfo.getCurYFrameData(), frameInfo.getNextYFrameData(),
				mvfx.data, mvfy.data, mvbx.data, mvby.data, curFrame.cols, curFrame.rows,
				frameInfo.getBlkWidth(), frameInfo.getBlkHeight(), m_frame_interval, m_overlap_size, fbClassifier, false);
			imwrite(interpol_path, interpol);
			//assign frames for next sequnce
			m_file_idx += m_frame_interval;
			path_setup(m_file_idx);
			cout << "Frame Interpolated :: " << m_file_idx << endl;
			curFrame = imread(curFrame_path);
			nextFrame = imread(nextFrame_path);
			prevFrame = imread(prevFrame_path);
			cvtColor(prevFrame, prevFrame, CV_BGR2YUV);
			postFrame = imread(postFrame_path);
			cvtColor(postFrame, postFrame, CV_BGR2YUV);
			ColorConverter::YUV2Y(prevFrame, prevYFrame);
			ColorConverter::YUV2Y(postFrame, postYFrame);
		}
		// frames exceptr for first and last
		while (prevFrame.data != NULL && curFrame.data != NULL && nextFrame.data != NULL && postFrame.data != NULL) {
			Mat interpol = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
			int nCols = curFrame.cols / frameInfo.getBlkWidth(), nRows = curFrame.rows / frameInfo.getBlkHeight();
			//classify from forward or backward
			fbClassifier.setTable(curFrame.cols, curFrame.rows);
			fbClassifier.classify((const BYTE*)prevYFrame.data, (const BYTE*)frameInfo.getCurYFrameData(), (const BYTE*)frameInfo.getNextYFrameData(), (const BYTE*)postYFrame.data, 
				curFrame.cols, curFrame.rows, 5, 3);
			//motion estimate
			frameInfo.motion_estimate(curFrame, nextFrame);
			//motion vector correction
			Mat mvfx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, true, 0.5);
			Mat mvfy = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, false, 0.5);
			Mat mvbx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, true, 0.5);
			Mat mvby = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, false, 0.5);
			//frame interpolation
			interpolator.basic_interpolation(interpol.data, frameInfo.getCurYFrameData(), frameInfo.getNextYFrameData(),
				mvfx.data, mvfy.data, mvbx.data, mvby.data, curFrame.cols, curFrame.rows,
				frameInfo.getBlkWidth(), frameInfo.getBlkHeight(), m_frame_interval, m_overlap_size, fbClassifier, true);
			imwrite(interpol_path, interpol);
			//assign frames for next sequnce
			m_file_idx += m_frame_interval;
			path_setup(m_file_idx);
			cout << "Frame Interpolated :: " << m_file_idx << endl;
			curFrame = imread(curFrame_path);
			nextFrame = imread(nextFrame_path);
			Mat prevFrame = imread(prevFrame_path);
			cvtColor(prevFrame, prevFrame, CV_BGR2YUV);
			Mat postFrame = imread(postFrame_path);
			if (postFrame.data == NULL)
				break;
			cvtColor(postFrame, postFrame, CV_BGR2YUV);
			ColorConverter::YUV2Y(prevFrame, prevYFrame);
			ColorConverter::YUV2Y(postFrame, postYFrame);
		}
	}

	void do_fruc() {
		FrameInterpolation interpolator;
		path_setup(m_file_idx);
		curFrame = imread(curFrame_path);
		nextFrame = imread(nextFrame_path);
		interpolator.init(curFrame.cols, curFrame.rows);
		while (curFrame.data != NULL && nextFrame.data != NULL) {
			Mat interpol = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
			int nCols = curFrame.cols/frameInfo.getBlkWidth(), nRows = curFrame.rows/frameInfo.getBlkHeight();
			frameInfo.motion_estimate(curFrame, nextFrame);
			Mat mvfx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, true, 0.5);
			Mat mvfy = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, true, false, 0.5);
			Mat mvbx = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, true, 0.5);
			Mat mvby = frameInfo.get_motion_vector_map_interpol(nCols, nRows, curFrame.cols, false, false, 0.5);
			interpolator.basic_interpolation(interpol.data, frameInfo.getCurYFrameData(), frameInfo.getNextYFrameData(),
				mvfx.data, mvfy.data, mvbx.data, mvby.data, curFrame.cols, curFrame.rows,
				frameInfo.getBlkWidth(), frameInfo.getBlkHeight(), m_frame_interval, m_overlap_size);
			imwrite(interpol_path, interpol);
			m_file_idx += m_frame_interval;
			path_setup(m_file_idx);
			cout << "Frame Interpolated :: "<<m_file_idx << endl;
			curFrame = imread(curFrame_path);
			nextFrame = imread(nextFrame_path);
		}
	
	}





};


#endif //__FRUC_H__