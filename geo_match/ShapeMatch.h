#pragma once
#include <omp.h>

#include <iomanip>
#include <math.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/core/core.hpp"
using namespace cv;
class Feature
{
public:
	/// X location.
	int x;
	/// Y location.
	int y;
	///X normalized first derivative
	float dx;
	///Y normalized first derivative
	float dy;


public:
	Feature();
	Feature(int x, int y, float dx, float dy);
	~Feature(void);
	// Clones the feature.
	Feature& operator= (Feature& feature);

};
class ShapeMatch
{
public:
	ShapeMatch(Mat image);
	~ShapeMatch();
	//// 模板制作
	vector<Feature> make_template(bool reverse_flag=false);
	// 图像阈值 自适应
	int TresholdOtsu(Mat im);
	// 得到合适的阈值
	void  GetImageThreshold(Mat image,int &bestLowThresh,int &bestHighThresh);
	// 筛选点
	vector<Feature> cacluate_contours(vector<vector<Point>> pre_contours);
	// 计算梯度等特征
	vector<Feature> CalGradientFeatures(Mat image,vector<Feature> contours,int threshold_value);
	// 得到范围
	Rect GetBoundingRectangle(vector<Feature> contours);
	// 功能函数
	Mat draw_contours(Mat im, vector<Feature> contors, Point xy = Point(0,0),Scalar color = Scalar{ 0,255,0 }, int width = 1);
	//// 模板匹配
	Mat find_object(Mat gray_image,vector<Mat> merge_im,Point& result_point, float& score);
	
private:
	Mat template_gray_image;
	Rect base_rect;
	vector<Feature> contours;
};

vector<Mat> cal_search_feature(Mat gray_image, int minGradientMagnitude = 35);