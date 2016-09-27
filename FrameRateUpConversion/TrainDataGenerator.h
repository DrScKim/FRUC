
#ifndef __TRAINDATAGEN_H__
#define __TRAINDATAGEN_H__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "FRUC.h"

using namespace std;
using namespace cv;
#define WRTIE_BASIC_MV_MAP	0
#define WRTIE_INTER_MV_MAP	1

inline int mvmap_gen(int startIDX, int blkW, int blkH, const char* file_path, const char* save_path, int searchRange=64, 
	float scale = 1.0, float mvscale = 1.0, int frame_interval = 1, int ref_frame_interval = 1, int writemode = WRTIE_BASIC_MV_MAP)
{
	int idx = startIDX;
	int nBlkW = blkW, nBlkH = blkH;
	char curpath[260];
	char nextpath[260];
	char mvmappath_fh[260];//forward_horizontal
	char mvmappath_fv[260];//forward_vertical
	char mvmappath_bh[260];//backward_horizontal
	char mvmappath_bv[260];//backward_vertical

	sprintf(curpath, "%s/%d.png", file_path, idx);
	sprintf(mvmappath_fh, "%s/fh/%d-%d.png", save_path, idx, idx + frame_interval);
	sprintf(mvmappath_fv, "%s/fv/%d-%d.png", save_path, idx, idx + frame_interval);
	sprintf(mvmappath_bv, "%s/bv/%d-%d.png", save_path, idx + frame_interval, idx);
	sprintf(mvmappath_bh, "%s/bh/%d-%d.png", save_path, idx + frame_interval, idx);
	idx+=frame_interval;
	sprintf(nextpath, "%s/%d.png", file_path, idx);

	Mat curOriFrame = imread(curpath);
	Mat nextOritFrame = imread(nextpath);
	Mat curFrame, nextFrame;
	while (!(curOriFrame.data == NULL || nextOritFrame.data == NULL)) {
		resize(curOriFrame, curFrame, Size(curOriFrame.cols * scale, curOriFrame.rows * scale), 0, 0, CV_INTER_CUBIC);
		resize(nextOritFrame, nextFrame, Size(nextOritFrame.cols * scale, nextOritFrame.rows * scale), 0, 0, CV_INTER_CUBIC);
		FrameInfo frame;
		frame.initialize(nBlkW, nBlkH, searchRange);

		frame.motion_estimate(curFrame, nextFrame);
		if (writemode == WRTIE_BASIC_MV_MAP) {
			frame.write_motion_vector_map((const char*)mvmappath_fh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, true, mvscale);
			frame.write_motion_vector_map((const char*)mvmappath_fv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, false, mvscale);
			frame.write_motion_vector_map((const char*)mvmappath_bh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, false, true, mvscale);
			frame.write_motion_vector_map((const char*)mvmappath_bv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, false, false, mvscale);
		}
		else if (writemode == WRTIE_INTER_MV_MAP) {
			frame.write_motion_vector_map_interpol((const char*)mvmappath_fh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, true, mvscale);
			frame.write_motion_vector_map_interpol((const char*)mvmappath_fv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, false, mvscale);
			frame.write_motion_vector_map_interpol((const char*)mvmappath_bh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, false, true, mvscale);
			frame.write_motion_vector_map_interpol((const char*)mvmappath_bv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, false, false, mvscale);
		}
		

		sprintf(curpath, "%s/%d.png", file_path, idx);
		sprintf(mvmappath_fh, "%s/fh/%d-%d.png", save_path, idx, idx + frame_interval);
		sprintf(mvmappath_fv, "%s/fv/%d-%d.png", save_path, idx, idx + frame_interval);
		sprintf(mvmappath_bv, "%s/bv/%d-%d.png", save_path, idx + frame_interval, idx);
		sprintf(mvmappath_bh, "%s/bh/%d-%d.png", save_path, idx + frame_interval, idx);
		idx+=frame_interval;
		sprintf(nextpath, "%s/%d.png",file_path, idx);

		curOriFrame = imread(curpath);
		nextOritFrame = imread(nextpath);

		cout << "Motion Vector map generation Phase::" << idx - frame_interval << "-" << idx << "frame written!" << endl;
		
	}
	return 0;
}

inline int mvmap_gt_gen(int startIDX, int blkW, int blkH, const char* file_path, const char* save_path, 
	int searchRange = 64, float scale = 1.0, int frame_interval = 1, int ref_frame_interval = 1, int writemode = WRTIE_BASIC_MV_MAP)
{
	int idx = startIDX, refidx=startIDX+ref_frame_interval;
	int nBlkW = blkW, nBlkH = blkH;
	char curpath[260];
	char refpath[260];
	char nextpath[260];
	char mvmappath_fh[260];//forward_horizontal
	char mvmappath_fv[260];//forward_vertical
	char mvmappath_bh[260];//backward_horizontal
	char mvmappath_bv[260];//backward_vertical

	sprintf(curpath, "%s/%d.png", file_path, idx);
	sprintf(mvmappath_fh, "%s/fh/%d-%d.png", save_path, idx, idx + frame_interval);
	sprintf(mvmappath_fv, "%s/fv/%d-%d.png", save_path, idx, idx + frame_interval);
	sprintf(mvmappath_bv, "%s/bv/%d-%d.png", save_path, idx + frame_interval, idx);
	sprintf(mvmappath_bh, "%s/bh/%d-%d.png", save_path, idx + frame_interval, idx);
	idx += frame_interval;
	sprintf(refpath, "%s/%d.png", file_path, refidx);
	refidx = refidx + frame_interval;
	sprintf(nextpath, "%s/%d.png", file_path, idx);

	Mat curOriFrame = imread(curpath);
	Mat refOriFrame = imread(refpath);
	Mat nextOritFrame = imread(nextpath);
	Mat curFrame, nextFrame, refFrame;
	while (!(curOriFrame.data == NULL || nextOritFrame.data == NULL)) {
		
		resize(curOriFrame, curFrame, Size(curOriFrame.cols * scale, curOriFrame.rows * scale), 0, 0, CV_INTER_CUBIC);
		resize(refOriFrame, refFrame, Size(refOriFrame.cols * scale, refOriFrame.rows * scale), 0, 0, CV_INTER_CUBIC);
		resize(nextOritFrame, nextFrame, Size(nextOritFrame.cols * scale, nextOritFrame.rows * scale), 0, 0, CV_INTER_CUBIC);
		FrameInfo frame_fward, frame_bward;
		frame_fward.initialize(nBlkW, nBlkH, searchRange);
		frame_bward.initialize(nBlkW, nBlkH, searchRange);

		frame_fward.motion_estimate(curFrame, refFrame, true, false);
		frame_bward.motion_estimate(refFrame, nextFrame, false, true);
		
		if (writemode == WRTIE_BASIC_MV_MAP) {
			frame_fward.write_motion_vector_map((const char*)mvmappath_fh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, true);
			frame_fward.write_motion_vector_map((const char*)mvmappath_fv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, false);
			frame_bward.write_motion_vector_map((const char*)mvmappath_bh, nextFrame.cols / nBlkW, nextFrame.rows / nBlkH, nextFrame.cols, false, true);
			frame_bward.write_motion_vector_map((const char*)mvmappath_bv, nextFrame.cols / nBlkW, nextFrame.rows / nBlkH, nextFrame.cols, false, false);
		}
		else if (writemode == WRTIE_INTER_MV_MAP) {
			frame_fward.write_motion_vector_map_interpol((const char*)mvmappath_fh, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, true);
			frame_fward.write_motion_vector_map_interpol((const char*)mvmappath_fv, curFrame.cols / nBlkW, curFrame.rows / nBlkH, curFrame.cols, true, false);
			frame_bward.write_motion_vector_map_interpol((const char*)mvmappath_bh, nextFrame.cols / nBlkW, nextFrame.rows / nBlkH, nextFrame.cols, false, true);
			frame_bward.write_motion_vector_map_interpol((const char*)mvmappath_bv, nextFrame.cols / nBlkW, nextFrame.rows / nBlkH, nextFrame.cols, false, false);
		
		}

		sprintf(curpath, "%s/%d.png", file_path, idx);
		sprintf(mvmappath_fh, "%s/fh/%d-%d.png", save_path, idx, idx + frame_interval);
		sprintf(mvmappath_fv, "%s/fv/%d-%d.png", save_path, idx, idx + frame_interval);
		sprintf(mvmappath_bv, "%s/bv/%d-%d.png", save_path, idx + frame_interval, idx);
		sprintf(mvmappath_bh, "%s/bh/%d-%d.png", save_path, idx + frame_interval, idx);
		idx += frame_interval;
		sprintf(refpath, "%s/%d.png", file_path, refidx);
		refidx = refidx + frame_interval;
		sprintf(nextpath, "%s/%d.png", file_path, idx);
		curOriFrame = imread(curpath);
		refOriFrame = imread(refpath);
		nextOritFrame = imread(nextpath);

		cout << curpath << endl;
		cout << refpath << endl;
		cout << nextpath << endl;
		cout << "Motion Vector GroundTruth map generation Phase::" <<idx - frame_interval << "-" << refidx - frame_interval << "-" << idx << "frame written!" << endl;
	}
	return 0;
}

inline int avgimg_gen(int startIDX, const char* file_path, const char* save_path, int frame_interval = 1) {
	int idx = startIDX;
	FILE* f;
	char cur_path[260];
	char next_path[260];
	char avg_img_path[260];

	sprintf(cur_path, "%s/%d.png", file_path, idx); idx += frame_interval;
	sprintf(next_path, "%s/%d.png", file_path, idx);

	Mat curFrame = imread(cur_path);
	Mat nextFrame = imread(next_path);

	while (!(curFrame.data == NULL || nextFrame.data == NULL)) {
		
		Mat cur_img, next_img;
		//curFrame.convertTo(cur_img, CV_32FC3);
		//nextFrame.convertTo(next_img, CV_32FC3);
		Mat avg_img = Mat::zeros(Size(curFrame.cols, curFrame.rows), CV_32FC3);
		cv::accumulate(curFrame, avg_img);
		cv::accumulate(nextFrame, avg_img);
		avg_img = avg_img / 2;
		avg_img.convertTo(avg_img, CV_8U);

		sprintf(avg_img_path, "%s/avg%d-%d.png", save_path, idx - frame_interval, idx);
		sprintf(cur_path, "%s/%d.png", file_path, idx); idx += frame_interval;
		sprintf(next_path, "%s/%d.png", file_path, idx);
		curFrame = imread(cur_path);
		nextFrame = imread(next_path);

		imwrite(avg_img_path, avg_img);
		cout << idx - frame_interval << "-" << idx << "frame saved" << endl;
	}
	return 0;
}

#endif __TRAINDATAGEN_H__