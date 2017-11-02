#include "FindContours.h"
#define MARKED 100 

int USTC_Find_Contours(Mat binaryImg, vector < vector < cv::Point >>& contours) {
 if (NULL == binaryImg.data) {
		cout << "Input Error." << endl;
		return MY_FAIL;
	}
	contours.clear();

	int height = binaryImg.rows, width = binaryImg.cols;
	int bigheight = height + 2, bigwidth = width + 2;
	Mat bigImg = Mat(bigheight, bigwidth, CV_8UC1, Scalar(0));	

	for (int i = 1; i <= height; i++) {
		for (int j = 1; j <= width; j++) {
			bigImg.data[i*bigwidth + j] = binaryImg.data[(i - 1)*width + j - 1];
		}
	}

	int label = 255, background = 0;	
	int startpos[8] = { 6, 7, 0, 1, 2, 3, 4, 5 };	
	int pos_add[8] = { 1, 1 - bigwidth, -bigwidth, -bigwidth - 1, -1, bigwidth - 1, bigwidth, bigwidth + 1 };
	int y_addi[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };	
	int x_addi[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int t = 0, flag = 0;
	int new_x = 0, new_y = 0;
	int pos_code;

	for (int y = 1; y < bigheight - 1; y++) {
		for (int x = 1; x < bigwidth - 1; x++) {
			if (bigImg.data[y*bigwidth + x] == label) {
				if (bigImg.data[y*bigwidth + x - 1] == background) {

					bigImg.data[y*bigwidth + x] = label;	

					int pos = y*bigwidth + x;
					pos_code = startpos[0];	
					
					vector<cv::Point> point_reg;	
					point_reg.push_back(Point(x - 1, y - 1));

					while (t != 2) {
						int count = 5;	

						while (count) {

							if (bigImg.data[pos + pos_add[pos_code]] == label) {
								if (!flag) {
									new_x = point_reg.back().x + x_addi[pos_code] + 1;
									new_y = point_reg.back().y + y_addi[pos_code] + 1;
								}
								else {
									new_x = point_reg.front().x + x_addi[pos_code] + 1;
									new_y = point_reg.front().y + y_addi[pos_code] + 1;
								}
								
								if (point_reg.front() == Point(new_x - 1, new_y - 1)) {
									bigImg.data[pos + pos_add[pos_code]] = MARKED;	
									t = 2;
									break;
								}

								//若非起始点
								else {
									point_reg.push_back(Point(new_x - 1, new_y - 1));
									bigImg.data[pos + pos_add[pos_code]] = MARKED;
									pos = new_y*bigwidth + new_x;
									pos_code = startpos[pos_code];
									count = 5;
									flag = 0;

								}
							}

						
							else {
								count--;
								pos_code = (pos_code + 1) % 8;
							}
						}

						
						if (count == 0) {
							t++;
							
							if (t == 1) {
								pos = y*bigwidth + x;
								flag = 1;
								pos_code = 4;
							}
						}
					}
				

					t = 0;
					contours.push_back(point_reg);
					point_reg.clear();
				}
			}
		}
	} 
 }
