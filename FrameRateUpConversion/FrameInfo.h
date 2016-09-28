#ifndef __FRAME_INFO__H__
#define __FRAME_INFO__H__


#include "MatUtil.h"
#include "MotionEstiamte.h"

#include "timeUtil.h"
using namespace cv;
class FrameInfo {

	MotionEstimator m_forward_mv;
	MotionEstimator m_backward_mv;
	int m_blkW, m_blkH, m_searchRange;
	Mat curYframe;
	Mat nextYframe;
	void setPixel(Mat& output, int x, int y, int value, int blkW, int blkH, int frameW) {
		int idx = x + y * frameW;
		for (int j = 0; j < blkH; j++) {
			for (int i = 0; i < blkW; i++) {
				output.data[idx + i] = value;
			}
			idx += frameW;
		}
	}

public:
	FrameInfo() {}
	~FrameInfo() {}
	int getBlkWidth() { return m_blkW; }
	int getBlkHeight() { return m_blkH; }
	int getSearchRange() { return m_searchRange; }
	void initialize(int blkW, int blkH, int searchRange) {
		m_blkW = blkW;
		m_blkH = blkH;
		m_searchRange = searchRange < 0 ? 1 : searchRange;
		m_searchRange = searchRange >= 128 ? 128 : searchRange;

	}
	uchar* getCurYFrameData() { return curYframe.data; }
	uchar* getNextYFrameData() { return nextYframe.data; }

	void motion_estimate(Mat& curFrame, Mat& nextFrame, bool forward=true, bool backward=true) {
		curYframe = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
		nextYframe = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
		Mat curYUVFrame, nextYUVFrame;
		ColorConverter::BGR2YUV(curFrame, curYUVFrame);
		ColorConverter::BGR2YUV(nextFrame, nextYUVFrame);
		ColorConverter::YUV2Y(curYUVFrame, curYframe);
		ColorConverter::YUV2Y(nextYUVFrame, nextYframe);
		int nBlkX = curYframe.cols / m_blkW;
		int nBlkY = curYframe.rows / m_blkH;
		m_forward_mv.reset();
		m_backward_mv.reset();
		m_forward_mv.initBlock(nBlkX, nBlkY);
		m_backward_mv.initBlock(nBlkX, nBlkY);
		
		const clock_t begin_time = clock();
		if (forward)
			m_forward_mv.estimate(curYframe.data, nextYframe.data, curYframe.cols, curYframe.rows, m_blkW, m_blkH, m_searchRange);
		if (backward)
			m_backward_mv.estimate(nextYframe.data, curYframe.data, nextYframe.cols, nextYframe.rows, m_blkW, m_blkH, m_searchRange);
		
		std::cout << "Elapsed: " << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;;
	}

	
	Mat get_motion_vector_map_interpol(int nCols, int nRows, int frameW, bool isForward, bool isHorizontal, float mv_scale = 1.0) {
		Mat output = Mat(Size(nCols, nRows), CV_8UC1, Scalar(0));
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {
				vector<MacroBlockData>* mbs;
				if (isForward == true) mbs = m_forward_mv.getMacroBlockList(); else mbs = m_backward_mv.getMacroBlockList();
				for (vector<MacroBlockData>::iterator iter = mbs->begin(); iter != mbs->end(); iter++) {
					int x = iter->X() / m_blkW;
					int y = iter->Y() / m_blkH;
					int value = 0;
					if (isHorizontal)
						value = iter->GetMV().x * mv_scale + 128;
					else
						value = iter->GetMV().y * mv_scale + 128;
					output.data[x + y * nCols] = value;
				}
			}
		}
		resize(output, output, Size(nCols*m_blkW, nRows*m_blkH));
		return output;
	}

	void write_motion_vector_map_interpol(const char* path, int nCols, int nRows, int frameW, bool isForward, bool isHorizontal, float mv_scale = 1.0) {
		Mat output = get_motion_vector_map_interpol(nCols, nRows, frameW, isForward, isHorizontal, mv_scale);
		/*
		Mat output = Mat(Size(nCols, nRows), CV_8UC1, Scalar(0));
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {
				vector<MacroBlockData>* mbs;
				if (isForward == true) mbs = m_forward_mv.getMacroBlockList(); else mbs = m_backward_mv.getMacroBlockList();
				for (vector<MacroBlockData>::iterator iter = mbs->begin(); iter != mbs->end(); iter++) {
					int x = iter->X() / m_blkW;
					int y = iter->Y() / m_blkH;
					int value = 0;
					if (isHorizontal)
						value = iter->GetMV().x * mv_scale + 128;
					else
						value = iter->GetMV().y * mv_scale + 128;
					output.data[x + y * nCols] = value;
				}
			}
		}
		resize(output, output, Size(nCols*m_blkW, nRows*m_blkH));
		*/
		imwrite(path, output);
	}


	void write_motion_vector_map(const char* path, int nCols, int nRows, int frameW, bool isForward, bool isHorizontal, float mv_scale=1.0) {
		m_blkW;
		m_blkH;
		cout << path << endl;
		Mat output = Mat(Size(nCols*m_blkW, nRows*m_blkH), CV_8UC1, Scalar(0));
		#pragma omp parallel for
		for (int i = 0; i < nCols; i++) {
			for (int j = 0; j < nRows; j++) {
				vector<MacroBlockData>* mbs;
				if (isForward == true) mbs = m_forward_mv.getMacroBlockList(); else mbs = m_backward_mv.getMacroBlockList();
				for (vector<MacroBlockData>::iterator iter = mbs->begin(); iter != mbs->end(); iter++) {
					int x = iter->X();
					int y = iter->Y();
					int value = 0;
					if (isHorizontal)
						value = iter->GetMV().x * mv_scale + 128;
					else
						value = iter->GetMV().y * mv_scale + 128;
					//cout << "x: " << x << ", y: " << y << ", value: " << value << endl;
					setPixel(output, x, y, value, m_blkW, m_blkH, frameW);
				}
			}
		}
		imwrite(path, output);
	}
};


#endif//__FRAME_INFO__H__