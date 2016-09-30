

#include "FRUC.h"
#include "FrameInterpol_fbClassify.h"
#include "math_util.h"
inline void test_frame_interpolation(int idx, char* load_dir, char* save_dir, char* file_ext, 
	int blkSizeW, int blkSizeH, int searchRange, int frame_interval, int overlap_size)
{

	FrameRateUpConverter converter;
	converter.setup(blkSizeW, blkSizeH, searchRange, frame_interval, overlap_size, idx, load_dir, save_dir, file_ext);
	converter.do_fruc();

}

inline void test_frame_interpolation_fbClassify(int idx, char* load_dir, char* save_dir, char* file_ext,
	int blkSizeW, int blkSizeH, int searchRange, int frame_interval, int overlap_size)
{

	FrameRateUpConverter converter;
	converter.setup(blkSizeW, blkSizeH, searchRange, frame_interval, overlap_size, idx, load_dir, save_dir, file_ext);
	converter.do_fruc_with_fbclassify();

}

inline void makeGroundTruth(int idx, char* load_dir, char* save_dir, char* file_ext) {
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
	Mat curFrame, curYFrame;
	curFrame = imread(curFrame_path);
	

	while (curFrame.data) {
		curYFrame = Mat(Size(curFrame.cols, curFrame.rows), CV_8UC1, Scalar(0));
		ColorConverter::BGR2YUV(curFrame, curFrame);
		ColorConverter::YUV2Y(curFrame, curYFrame);
		imwrite(saveFrame_path, curYFrame);
		idx+=2;
		sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
		sprintf(saveFrame_path, "%s/%d.%s", save_directory, idx, save_extension);
		curFrame = imread(curFrame_path);
	}
}

inline void checkPSNR(int idx, char* gt_folder, char* check_folder, char* file_ext) {
	char gt_directory[_MAX_PATH];
	char ch_directory[_MAX_PATH];
	char gt_path[_MAX_PATH];
	char ch_path[_MAX_PATH];
	char save_extension[_MAX_PATH];
	sprintf(gt_directory, gt_folder);
	sprintf(ch_directory, check_folder);
	sprintf(save_extension, file_ext);
	sprintf(gt_path, "%s/%d.%s", gt_directory, idx, save_extension);
	sprintf(ch_path, "%s/%d.%s", ch_directory, idx, save_extension);
	Mat gtFrame, chFrame;

	gtFrame = imread(gt_path);
	chFrame = imread(ch_path);
	while (gtFrame.data != NULL && chFrame.data != NULL) {
		cout << "idx." << save_extension << " PSNR : " << getPSNRFrame(gtFrame, chFrame) << endl;
		idx += 2;

		sprintf(gt_path, "%s/%d.%s", gt_directory, idx, save_extension);
		sprintf(ch_path, "%s/%d.%s", ch_directory, idx, save_extension);
		gtFrame = imread(gt_path);
		chFrame = imread(ch_path);
	}
}