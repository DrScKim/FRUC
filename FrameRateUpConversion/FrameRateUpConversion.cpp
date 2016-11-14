// FrameRateUpConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FRUC_tester.h"

#include "TrainDataGenerator.h"
int _tmain(int argc, _TCHAR* argv[])
{
	int blkSize = 32;
	/*
	makeGroundTruth(0, "d:/fruc_bench/akiyo/1/orig/frame", "d:/fruc_bench/akiyo/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/akiyo/1/orig/gt", "d:/fruc_bench/akiyo/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/akiyo/1/orig/frame", "d:/fruc_bench/akiyo/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/akiyo/1/orig/gt", "d:/fruc_bench/akiyo/1/orig/y_gt", "png");

	makeGroundTruth(0, "d:/fruc_bench/bridge/1/orig/frame", "d:/fruc_bench/bridge/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/bridge/1/orig/gt", "d:/fruc_bench/bridge/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/bridge/1/orig/frame", "d:/fruc_bench/bridge/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/bridge/1/orig/gt", "d:/fruc_bench/bridge/1/orig/y_gt", "png");

	makeGroundTruth(0, "d:/fruc_bench/coastguard/1/orig/frame", "d:/fruc_bench/coastguard/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/coastguard/1/orig/gt", "d:/fruc_bench/coastguard/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/coastguard/1/orig/frame", "d:/fruc_bench/coastguard/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/coastguard/1/orig/gt", "d:/fruc_bench/coastguard/1/orig/y_gt", "png");

	makeGroundTruth(0, "d:/fruc_bench/foreman/1/orig/frame", "d:/fruc_bench/foreman/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/foreman/1/orig/gt", "d:/fruc_bench/foreman/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/foreman/1/orig/frame", "d:/fruc_bench/foreman/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/foreman/1/orig/gt", "d:/fruc_bench/foreman/1/orig/y_gt", "png");

	makeGroundTruth(0, "d:/fruc_bench/mobile/1/orig/frame", "d:/fruc_bench/mobile/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/mobile/1/orig/gt", "d:/fruc_bench/mobile/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/mobile/1/orig/frame", "d:/fruc_bench/mobile/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/mobile/1/orig/gt", "d:/fruc_bench/mobile/1/orig/y_gt", "png");
	
	makeGroundTruth(0, "d:/fruc_bench/stefan/1/orig/frame", "d:/fruc_bench/stefan/1/half/y_frame", "png", 0.5);
	makeGroundTruth(1, "d:/fruc_bench/stefan/1/orig/gt", "d:/fruc_bench/stefan/1/half/y_gt", "png", 0.5);
	makeGroundTruth(0, "d:/fruc_bench/stefan/1/orig/frame", "d:/fruc_bench/stefan/1/orig/y_frame", "png");
	makeGroundTruth(1, "d:/fruc_bench/stefan/1/orig/gt", "d:/fruc_bench/stefan/1/orig/y_gt", "png");
	*/

	
	//test_fruc_fb_dp(1, "d:/fruc_bench/mobile/1/orig/frame", "d:/fruc_bench/mobile/1/half/fruc_srcnn", "png", blkSize, blkSize, 32, 2, 0);
	/*
	test_fruc_fb_dp(1, "d:/fruc_bench/coastguard/1/half/frame", "d:/fruc_bench/coastguard/1/half/fruc", "png", 16, 16, 16, 2, 0);

	test_fruc_fb_dp(1, "d:/fruc_bench/akiyo/1/half/frame", "d:/fruc_bench/akiyo/1/half/fruc", "png", 16, 16, 32, 2, 0);
	test_fruc_fb_dp(1, "d:/fruc_bench/bridge/1/half/frame", "d:/fruc_bench/bridge/1/half/fruc", "png", blkSize, blkSize, 16, 2, 0);
	
	test_fruc_fb_dp(1, "d:/fruc_bench/mobile/1/half/frame", "d:/fruc_bench/mobile/1/half/fruc", "png", blkSize, blkSize, 16, 2, 0);
	
	test_fruc_fb_dp(1, "d:/fruc_bench/stefan/1/half/frame", "d:/fruc_bench/stefan/1/half/fruc", "png", blkSize, blkSize, 16, 2, 0);
	*/
	test_fruc_fb_dp(1, "d:/fruc_bench/coastguard/1/orig/frame", "d:/fruc_bench/coastguard/1/orig/fruc", "png", 16, 16, 32, 2, 0);
	checkPSNR(4, "d:/fruc_bench/coastguard/1/orig/y_gt", "d:/fruc_bench/coastguard/1/orig/fruc", "png", 1.0, false, 0, 0);
	
	//test_fruc_fb_dp(1, "d:/fruc_bench/mobile/1/orig/frame", "d:/fruc_bench/mobile/1/orig/fruc1", "png", blkSize, blkSize, 64, 2, 0);
	//test_frame_interpolation_fbClassify(286, "d:/mvmap_db/1", "d:/phd/fruc/1", "png", 16, 16, 32, 2, 0);
	
	//test_frame_interpolation(286, "d:/mvmap_db/1", "d:/phd/fruc/1", "png", 16, 16, 32, 2, 0);
	//test_frame_interpolation_fbClassify(2202, "d:/mvmap_db/2", "d:/phd/fruc/2", "png", 16, 16, 32, 2, 0);
	//makeGroundTruth(2202, "d:/mvmap_db/2", "d:/phd/fruc/2/gt", "png");
	//test_frame_interpolation_fbClassify(2436, "d:/mvmap_db/3", "d:/phd/fruc/3", "png", 16, 16, 32, 2, 0);
	//makeGroundTruth(2436, "d:/mvmap_db/3", "d:/phd/fruc/3/gt", "png");
	//test_frame_interpolation_fbClassify(4046, "d:/mvmap_db/4", "d:/phd/fruc/4", "png", 16, 16, 32, 2, 0);
	//makeGroundTruth(4046, "d:/mvmap_db/4", "d:/phd/fruc/4/gt", "png");
	//test_frame_interpolation_fbClassify(11610, "d:/mvmap_db/5", "d:/phd/fruc/5", "png", 16, 16, 32, 2, 0);
	//makeGroundTruth(11610, "d:/mvmap_db/5", "d:/phd/fruc/5/gt", "png");

	//checkPSNR(287, "d:/phd/fruc/1/gt", "d:/phd/fruc/1", "png");
	//test_frucnn_fb("d:/phd/fruc/1/gt/336.png", "d:/phd/fruc/1/gt/338.png", "d:/phd/fruc/1/gt/340.png", "d:/phd/fruc/1/gt/342.png", "d:/phd/fruc/1/BLUR.png", "d:/phd/fruc/1/gt/339.png", "d:/phd/fruc/1/result.png");
	//test_fruc_fb_dp(286, "d:/mvmap_db/1", "d:/phd/fruc/1", "png", 16, 16, 64, 2, 0);
	
	//blur("d:/phd/fruc/1/result.png", "d:/phd/fruc/1/BLUR.png", "d:/phd/fruc/1/gt/315.png");
}

											

