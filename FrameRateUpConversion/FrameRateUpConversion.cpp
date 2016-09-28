// FrameRateUpConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FRUC_tester.h"

#include "TrainDataGenerator.h"
int _tmain(int argc, _TCHAR* argv[])
{
	test_frame_interpolation(286, "d:/mvmap_db/1", "d:/phd/fruc/1", "png", 16, 16, 32, 2, 3);



}

											


/*
mvmap_gen(286, 8, 8, "d:/mvmap_db/1", "d:/mv_map_cnn/1/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(286, 8, 8, "d:/mvmap_db/1", "d:/mv_map_cnn/1/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(286, "d:/mvmap_db/1", "d:/mv_map_cnn/1/avg", 2);

mvmap_gen(2202, 8, 8, "d:/mvmap_db/2", "d:/mv_map_cnn/2/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(2202, 8, 8, "d:/mvmap_db/2", "d:/mv_map_cnn/2/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(2202, "d:/mvmap_db/2", "d:/mv_map_cnn/2/avg", 2);

mvmap_gen(2436, 8, 8, "d:/mvmap_db/3", "d:/mv_map_cnn/3/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(2436, 8, 8, "d:/mvmap_db/3", "d:/mv_map_cnn/3/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(2436, "d:/mvmap_db/3", "d:/mv_map_cnn/3/avg", 2);

mvmap_gen(4046, 8, 8, "d:/mvmap_db/4", "d:/mv_map_cnn/4/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(4046, 8, 8, "d:/mvmap_db/4", "d:/mv_map_cnn/4/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(4046, "d:/mvmap_db/4", "d:/mv_map_cnn/4/avg", 2);

mvmap_gen(11610, 8, 8, "d:/mvmap_db/5", "d:/mv_map_cnn/5/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(11610, 8, 8, "d:/mvmap_db/5", "d:/mv_map_cnn/5/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(11610, "d:/mvmap_db/5", "d:/mv_map_cnn/5/avg", 2);

mvmap_gen(3352, 8, 8, "d:/mvmap_db/8", "d:/mv_map_cnn/8/mvmap", 64, 1.0, 0.5, 2, 1, WRTIE_INTER_MV_MAP);
mvmap_gt_gen(3352, 8, 8, "d:/mvmap_db/8", "d:/mv_map_cnn/8/mvmap_gt", 32, 1.0, 2, 1, WRTIE_INTER_MV_MAP);
avgimg_gen(3352, "d:/mvmap_db/8", "d:/mv_map_cnn/8/avg", 2);
*/

/*
mvmapgen(2202, 8, 8, "d:/mvmap_db/2", "d:/mvmap_db/2/mvmap", 64);
mvmap_gen(2202, 8, 8, "d:/mvmap_db/2", "d:/mvmap_db/2/mvmap_downscale", 32, 0.5f);
avgimg_gen(2202, "d:/mvmap_db/2", "d:/mvmap_db/2/avg");
cout << "2 set done" << endl;

mvmap_gen(2436, 8, 8, "d:/mvmap_db/3", "d:/mvmap_db/3/mvmap", 64);
mvmap_gen(2436, 8, 8, "d:/mvmap_db/3", "d:/mvmap_db/3/mvmap_downscale", 32, 0.5f);
avgimg_gen(2436, "d:/mvmap_db/3", "d:/mvmap_db/3/avg");
cout << "3 set done" << endl;

mvmap_gen(4046, 8, 8, "d:/mvmap_db/4", "d:/mvmap_db/4/mvmap", 64);
mvmap_gen(4046, 8, 8, "d:/mvmap_db/4", "d:/mvmap_db/4/mvmap_downscale", 32, 0.5f);
avgimg_gen(4046, "d:/mvmap_db/4", "d:/mvmap_db/4/avg");
cout << "4 set done" << endl;

mvmap_gen(11610, 8, 8, "d:/mvmap_db/5", "d:/mvmap_db/5/mvmap", 64);
mvmap_gen(11610, 8, 8, "d:/mvmap_db/5", "d:/mvmap_db/5/mvmap_downscale", 32, 0.5f);
avgimg_gen(11610, "d:/mvmap_db/5", "d:/mvmap_db/5/avg");
cout << "5 set done" << endl;
*/
/*
mvmap_gen(6609, 8, 8, "d:/mvmap_db/6", "d:/mvmap_db/6/mvmap", 128);
mvmap_gen(6603, 8, 8, "d:/mvmap_db/6", "d:/mvmap_db/6/mvmap_downscale", 64, 0.5f);
avgimg_gen(6603, "d:/mvmap_db/6", "d:/mvmap_db/6/avg");
cout << "6 set done" << endl;
*/
/*
mvmap_gen(1188, 8, 8, "d:/mvmap_db/7", "d:/mvmap_db/7/mvmap", 128);
mvmap_gen(1167, 8, 8, "d:/mvmap_db/7", "d:/mvmap_db/7/mvmap_downscale", 64, 0.5f);
avgimg_gen(1167, "d:/mvmap_db/7", "d:/mvmap_db/7/avg");
cout << "7 set done" << endl;
*/
/*
mvmap_gen(3352, 8, 8, "d:/mvmap_db/8", "d:/mvmap_db/8/mvmap", 128);
mvmap_gen(3352, 8, 8, "d:/mvmap_db/8", "d:/mvmap_db/8/mvmap_downscale", 64, 0.5f);
avgimg_gen(3352, "d:/mvmap_db/8", "d:/mvmap_db/8/avg");
cout << "8 set done" << endl;
*/