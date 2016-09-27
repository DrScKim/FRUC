

#ifndef __ME_UTIL_H__
#define __ME_UTIL_H__

typedef unsigned char uchar;
typedef unsigned char uint8;

/*
void writeDiffMaps(string path, char* src) {
	VideoIO tmp;
	tmp.readFromFile(path);
	int i = 0;
	char framePath[255];
	cout << path << endl;
	tmp.read();
	Mat mat1 = tmp.getFrame();
	Mat sub;
	while (1) {
		i++;
		tmp.read();

		Mat mat2 = tmp.getFrame();
		if (mat2.empty()) {
			break;
		}

		Mat mat1Gray, mat2Gray;
		cvtColor(mat1, mat1Gray, CV_BGR2GRAY);
		cvtColor(mat2, mat2Gray, CV_BGR2GRAY);
		absdiff(mat1Gray, mat2Gray, sub);
		sprintf(framePath, "E:/FRUC_VIDEO/ROK_Diff/frame_%d.jpg", i);

		imwrite(string(framePath), sub);
		cout << "!::" << mat1.empty() << endl;
		mat1 = mat2.clone();
	}
}
*/
#endif