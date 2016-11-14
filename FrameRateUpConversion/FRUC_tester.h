

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

inline void blur(char* result, char* blur, char* gt) {
	Mat resultMat = imread(result);
	Mat gtMat = imread(gt);
	Mat resultBlur;
	//GaussianBlur(resultMat, resultBlur, Size(5, 5), 0);
	
	medianBlur(resultMat, resultBlur, 5);
	imwrite(blur, resultBlur);
	
	double psnr = getPSNRFrame(gtMat, resultBlur);
	cout << "Blur PSNR : " << psnr << endl;
	
}

inline void test_fruc_fb_dp(int idx, char* load_dir, char* save_dir, char* file_ext,
	int blkSizeW, int blkSizeH, int searchRange, int frame_interval, int overlap_size) {
	FrameRateUpConverter converter;
	converter.setup(blkSizeW, blkSizeH, searchRange, frame_interval, overlap_size, idx, load_dir, save_dir, file_ext);
	converter.do_fruc_with_fb_dp();
	
}

inline void test_frucnn_fb(char* prev, char* cur, char* next, char* post, char* frucnn, char* gt, char* result) {
	Mat curYFrame = imread(cur);
	Mat nextYFrame = imread(next);
	Mat prevYFrame = imread(prev);
	Mat postYFrame = imread(post);
	Mat frucnnYFrame = imread(frucnn);
	Mat gtYFrame = imread(gt);
	Mat resultYFrame = imread(result);
	FBClassifier fbClassifier;
	double psnr0 = getPSNRFrame(gtYFrame, resultYFrame);
	double psnr1 = getPSNRFrame(gtYFrame, frucnnYFrame);
	fbClassifier.setTable(curYFrame.cols, curYFrame.rows);
	fbClassifier.classify((const BYTE*)prevYFrame.data, (const BYTE*)curYFrame.data, (const BYTE*)nextYFrame.data, (const BYTE*)postYFrame.data,
		curYFrame.cols, curYFrame.rows, 50, 50);
	int *table = fbClassifier.getTable();
	for (int y = 0; y < prevYFrame.rows; y++) {
		for (int x = 0; x < curYFrame.cols; x++) {
			
			if (table[x + y*curYFrame.cols] == SG_BACKGROUND) {
				frucnnYFrame.data[x + y * curYFrame.cols] = (curYFrame.data[x + y * curYFrame.cols] >> 1) + (nextYFrame.data[x + y*curYFrame.cols] >> 1);
			}
			
			else if (table[x + y*curYFrame.cols] == SG_BACKGROUND_FROM_BACKWARD) {
				frucnnYFrame.data[x + y * curYFrame.cols] = postYFrame.data[x + y * prevYFrame.cols];
			}
			else if (table[x + y*curYFrame.cols] == SG_BACKGROUND_FROM_FORWARD) {
				frucnnYFrame.data[x + y * curYFrame.cols] = prevYFrame.data[x + y * postYFrame.cols];
			}
			
		}
	}
	double psnr2 = getPSNRFrame(gtYFrame, frucnnYFrame);
	cout << "PSNR of Path based Interpolation: " << psnr0 << endl;
	cout << "PSNR of FRUCNN: " << psnr1 << endl;
	cout << "PSNR of FRUCNN + FBClassifier: " << psnr2 << endl;
	imwrite("D:/phd/fruc/1/frucnn_fb.png", frucnnYFrame);

}
inline void test_frame_interpolation_fbClassify(int idx, char* load_dir, char* save_dir, char* file_ext,
	int blkSizeW, int blkSizeH, int searchRange, int frame_interval, int overlap_size)
{

	FrameRateUpConverter converter;
	converter.setup(blkSizeW, blkSizeH, searchRange, frame_interval, overlap_size, idx, load_dir, save_dir, file_ext);
	converter.do_fruc_with_fbclassify();

}

inline void makeGroundTruth(int idx, char* load_dir, char* save_dir, char* file_ext, float scale=1.0) {
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

		cv::resize(curYFrame, curYFrame, cv::Size(curYFrame.cols*scale, curYFrame.rows*scale), 0, 0, CV_INTER_CUBIC);

		imwrite(saveFrame_path, curYFrame);
		idx+=2;

		sprintf(curFrame_path, "%s/%d.%s", load_directory, idx, save_extension);
		sprintf(saveFrame_path, "%s/%d.%s", save_directory, idx, save_extension);
		curFrame = imread(curFrame_path);
	}
}

inline void checkPSNR(int idx, char* gt_folder, char* check_folder, char* file_ext, float scale=1.0, bool isBorderRemove=false, int borderX=0, int borderY=0) {
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
		if (scale != 1.0)
			cv::resize(chFrame, chFrame, cv::Size(chFrame.cols*scale, chFrame.rows*scale), 0, 0, CV_INTER_CUBIC);

		//cout << "idx." << save_extension << " PSNR :\t" << getPSNRFrame(gtFrame, chFrame) << endl;
		if (isBorderRemove == false)
			cout << getPSNRFrame(gtFrame, chFrame) << endl;
		else
			cout << getPSNRFrame(gtFrame, chFrame, borderX, borderY) << endl;
		idx += 2;

		sprintf(gt_path, "%s/%d.%s", gt_directory, idx, save_extension);
		sprintf(ch_path, "%s/%d.%s", ch_directory, idx, save_extension);
		gtFrame = imread(gt_path);
		chFrame = imread(ch_path);
	}
}