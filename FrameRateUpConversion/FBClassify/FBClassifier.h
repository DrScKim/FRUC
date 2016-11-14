#ifndef __FBCLASSIFIER_H__
#define __FBCLASSIFIER_H__

#include <iostream>
#include "FBClassfyDefine.h"


typedef unsigned char BYTE;

class FBClassifier
{
private:
	int* m_table;
	int* m_uvTable;
	void setUVTable(int width, int height) {
		for (int h = 0; h<height; h += 2) {
			for (int w = 0; w<width; w += 2) {
				int w1 = w >> 1, h1 = h >> 1;
				m_uvTable[w1 + h1*(width / 2)] = m_table[w + h*width];
			}
		}
	}
	void getAbsDiffImage(const BYTE* image1, const BYTE* image2, BYTE* dst, int width, int height) {
		BYTE* tmp = dst;
		const BYTE* image1Tmp = image1;
		const BYTE* image2Tmp = image2;
		for (int h = 0; h<height; h++) {
			for (int w = 0; w<width; w++) {
				dst[w] = std::abs(image1Tmp[w] - image2Tmp[w]);

			}
			image1Tmp += width;
			image2Tmp += width;
			dst += width;
		}
		dst = tmp;
	}
public:
	FBClassifier() : m_table(NULL), m_uvTable(NULL){}
	~FBClassifier() {
		if (m_table != NULL)
			delete[] m_table;
		if (m_uvTable != NULL)
			delete[] m_uvTable;
	}
	void setTable(int width, int height) {
		m_table = new int[width*height];
		m_uvTable = new int[width*height / 4];
		memset(m_table, SG_OBJECT, width*height);
	}
	int* getTable() { return m_table; }
	int* getUVTable() { return m_uvTable; }
	
	void classify(Mat &prev, Mat &src, Mat &ref, Mat& post, int width, int height, uint8 threshold1, uint8 threshold2) {
		Mat diffCur, diffPrev, diffPost;
		cv::absdiff(src, ref, diffCur);
		cv::absdiff(prev, src, diffPrev);
		cv::absdiff(ref, post, diffPost);
		/*
		imshow("prev", prev);
		imshow("src", src);
		imshow("ref", ref);
		imshow("post", post);
		imshow("diffCur", diffCur);
		imshow("diffPrev", diffPrev);
		imshow("diffPost", diffPost);
		waitKey(0);
		*/


		int idx = 0;
		int i = 0, j = 0;
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				int coord = idx + w;
				if (diffCur.data[w + h*width] < threshold1) {
					m_table[w + h*width] = SG_BACKGROUND;
					i++;
				}
				else {
					if (diffPrev.data[w + h*width] > threshold2 && diffPost.data[w + h*width] < threshold2) {// + + - 
						m_table[w + h*width] = SG_BACKGROUND_FROM_FORWARD;
					}
					if (diffPrev.data[w + h*width] < threshold2 && diffPost.data[w + h*width] < threshold2) {// - + -	in or out 
						m_table[w + h*width] = SG_FOR_AVERAGING;
					}
					if (diffPrev.data[w + h*width] < threshold2 && diffPost.data[w + h*width] > threshold2) {// - + +
						m_table[w + h*width] = SG_BACKGROUND_FROM_BACKWARD;
					}
					if (diffPrev.data[w + h*width] > threshold2 && diffPost.data[w + h*width] > threshold2) {// + + + action!
						m_table[w + h*width] = SG_FOR_BLENDING;
					}
					j++;
				}

			}
			idx += height;
		}
		
	}


	void classify(const BYTE* prev, const BYTE* src, const BYTE* ref, const BYTE* post, int width, int height, uint8 threshold1, uint8 threhold2) {
		

		BYTE* diffCur = new BYTE[width* height];
		BYTE* diffPrev = new BYTE[width*height];
		BYTE* diffPost = new BYTE[width*height];
		getAbsDiffImage(src, ref, diffCur,  width, height);
		getAbsDiffImage(prev, src, diffPrev,  width, height);
		getAbsDiffImage(ref, post, diffPost,  width, height);
		simple_classify(diffPrev, diffCur, diffPost, width, height, threshold1, threhold2);
		setUVTable(width, height);
		delete[] diffCur;
		delete[] diffPrev;
		delete[] diffPost;
	}
	
	void simple_classify(BYTE* diffPrev, BYTE* diffCur, BYTE* diffPost, int width, int height, int threshold1, int threshold2)
	{
		int idx = 0;
		int i = 0, j = 0;
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				int coord = idx + w;
				if ((int)diffCur[w + h*width] < threshold1) {
					m_table[w + h*width] = SG_BACKGROUND;
					i++;
				}
				else {
					if(diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] < threshold2) {// + + - 
						m_table[w + h*width] = SG_BACKGROUND_FROM_BACKWARD;
					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] < threshold2) {// - + -	in or out 
						m_table[w + h*width] = SG_FOR_AVERAGING;
					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] > threshold2) {// - + +
						m_table[w + h*width] = SG_BACKGROUND_FROM_FORWARD;
					}
					if (diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] > threshold2) {// + + + action!
						m_table[w + h*width] = SG_FOR_BLENDING;
					}
					j++;
				}

			}
			idx += height;
		}
		cout << "background case : " << i << ", " << "others : " << j << " total pixels : " << width*height << endl;
	}

	void classify(BYTE* diffPrev, BYTE* diffCur, BYTE* diffPost, int width, int height, int threshold1, int threshold2) {
		setTable(width, height);
		for (int h = 0; h<height; h++) {
			for (int w = 0; w<width; w++) {
				


				if (diffCur[w + h*width] < threshold1) {
					if (diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] < threshold2) {// + - -
						m_table[w + h*width] = SG_BACKGROUND;
					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] < threshold2) {// - - -
						m_table[w + h*width] = SG_BACKGROUND;
					}
					if (diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] > threshold2) {// + - +

						if (diffPrev[w + h*width] > diffPost[w + h*width])
							m_table[w + h*width] = SG_BACKGROUND;
						else
							m_table[w + h*width] = SG_BACKGROUND;

					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] > threshold2) {// - - +
						m_table[w + h*width] = SG_BACKGROUND;
					}
				}
				else {

					if (diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] < threshold2) {// + + - 
						m_table[w + h*width] = SG_BACKGROUND_FROM_FORWARD;
					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] < threshold2) {// - + -	in or out 
						if (diffPrev[w + h*width] > diffPost[w + h*width])
							m_table[w + h*width] = SG_FOR_BLENDING;
						else
							m_table[w + h*width] = SG_FOR_BLENDING;
					}
					if (diffPrev[w + h*width] > threshold2 && diffPost[w + h*width] > threshold2) {// + + + blending
						//m_table[w+h*width] = SG_FOR_BLENDING;
						if (diffPrev[w + h*width] > 15 && diffPost[w + h*width] > 15) {
							m_table[w + h*width] = SG_FOR_BLENDING;
						}
						else if (diffPrev[w + h*width] > diffPost[w + h*width])
							m_table[w + h*width] = SG_FOR_BLENDING;
						else
							m_table[w + h*width] = SG_FOR_BLENDING;
					}
					if (diffPrev[w + h*width] < threshold2 && diffPost[w + h*width] > threshold2) {// - + +
						m_table[w + h*width] = SG_BACKGROUND_FROM_BACKWARD;
					}

				}
			}
		}
	}

};

#endif	//__FBCLASSIFY_H__