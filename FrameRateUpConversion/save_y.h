#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "MatUtil.h"

class saveY {

public:
	static void save(int idx, char* load_dir, char* save_dir, char* file_ext) {
		char save_directory[_MAX_PATH];
		char load_directory[_MAX_PATH];
		char save_filename[_MAX_PATH];
		char save_extension[_MAX_PATH];
		char curFrame_path[_MAX_PATH];
		char saveFrame_path[_MAX_PATH];
		idx++;
		sprintf(save_directory, save_dir);
		sprintf(load_directory, load_dir);
		sprintf(save_extension, file_ext);
		sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
		sprintf(saveFrame_path, "%s/%d.%s", save_directory, idx, save_extension);
		Mat curFrame, curYUVFrame, curYFrame;
		curFrame = imread(curFrame_path);

		while (curFrame.data) {
			curYFrame = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
			ColorConverter::BGR2YUV(curFrame, curFrame);
			ColorConverter::YUV2Y(curFrame, curYFrame);
			imwrite(saveFrame_path, curYFrame);
			idx += 2;
			sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
			sprintf(saveFrame_path, "%s/%d.%s", save_directory, idx, save_extension);
			curFrame = imread(curFrame_path);
		}
	
	}
	
}