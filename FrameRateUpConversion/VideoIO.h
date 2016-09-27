#ifndef __VIDEO_IO_H__
#define __VIDEO_IO_H__

#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
class VideoIO
{
private:
	VideoCapture m_Capture;
	VideoWriter m_Writer;
	Mat m_curFrame;

	
public:
	VideoIO() {

	}

	~VideoIO() {
		if (m_Capture.isOpened())
			m_Capture.release();
		if (m_Writer.isOpened())
			m_Writer.release();
	}
	
	
	double getFPS() { return m_Capture.get(CV_CAP_PROP_FPS); }
	double getFOURCC() { return m_Capture.get(CV_CAP_PROP_FOURCC); }
	double getWidth() { return m_Capture.get(CV_CAP_PROP_FRAME_WIDTH); }
	double getHeigth() { return m_Capture.get(CV_CAP_PROP_FRAME_HEIGHT); }

	bool isWriterEmpty() { return m_Writer.isOpened(); }
	bool isCaptureEmpty() { return m_Capture.isOpened(); }
	void readFromFile(string path) {
		m_Capture.open(path);
		if (!m_Capture.isOpened()) {
			printf("READ ERROR : not found video file\n");
		}
	}
	
	void read() { m_Capture >> m_curFrame; }
	void writeFrame(Mat& Frame) { m_Writer << Frame; }
	Mat getFrame() { return m_curFrame; }
	void writeAllFrames(string path, char* src) {
		VideoIO tmp;
		tmp.readFromFile(path);
		int i = 0;
		char framePath[255];
		cout << path << endl;
		while (1) {
			i++;
			tmp.read();
			Mat mat1 = tmp.getFrame();
			sprintf(framePath, "%S%d.jpg", src, i);
			if (mat1.empty()) {
				break;
			}
			imwrite(string(framePath), mat1);
			cout << "!::" << mat1.empty() << endl;
		}
	}
	void writeReadyFile(string path, float fpsScale = 1.0, float WScale = 1.0, float HScale = 1.0) {
		if (!m_Capture.isOpened()) {
			printf("WRITE ERROR : not found video file\n");
			return;
		}
		cout << "FOURCC : " << (int)getFOURCC() << endl;
		cout << "Frame Rate : " << getFPS() * fpsScale << endl;
		cout << "Size : " << getWidth()*WScale << ", " << getHeigth()*HScale << endl;
		m_Writer.open(path, 0, getFPS()*fpsScale, cv::Size(getWidth()*WScale, getHeigth()*HScale));
	}

	void writeReadyFile(string path, int width, int height) {
		if (!m_Capture.isOpened()) {
			printf("WRITE ERROR : not found video file\n");
			return;
		}
		m_Writer.open(path, 0, getFPS(), cv::Size(width, height));
	}
	void outVideo(string readVideoFile, string outVideoFile, int startSec, int durationSec) {
		VideoIO vIO;
		vIO.readFromFile(readVideoFile);
		vIO.writeReadyFile(outVideoFile, 3840, 2160);
		int nFrame = 0;
		cout << vIO.getFPS()*startSec << endl;

		Size size(3840, 2160);

		while (nFrame++ < (int)(vIO.getFPS()*startSec)) {
			vIO.read();
			cout << nFrame << endl;
		}
		cout << "write" << endl;
		while (nFrame++ <= (startSec + durationSec)*vIO.getFPS()) {
			Mat frame = vIO.getFrame();
			resize(frame, frame, size);
			vIO.writeFrame(frame);
			vIO.read();
			cout << nFrame << " frame written" << endl;
		}
	}

};

#endif //__VIDEO_IO_H__