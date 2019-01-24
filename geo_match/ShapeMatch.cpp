#include "ShapeMatch.h"

#define MAXFEANUM 1500//最大特征数目

Feature::Feature()
{
}

Feature::Feature(int x, int y, float d_x, float d_y) : x(x), y(y), dx(d_x), dy(d_y)
{
}

Feature::~Feature(void)
{
}

Feature & Feature::operator=(Feature & feature)
{
	x = feature.x;
	y = feature.y;
	dx = feature.dx;
	dy = feature.dy;
	return *this;
}

ShapeMatch::ShapeMatch(Mat image)
{
	if (image.channels() == 3)
		cvtColor(image, template_gray_image, CV_BGR2GRAY);
	else
		template_gray_image = image.clone();
}

ShapeMatch::~ShapeMatch()
{
}

vector<Feature> ShapeMatch::make_template(bool reverse_flag)
{
	int threshold_value = max(25, TresholdOtsu(template_gray_image));
	Mat edge;
	int bestLowThresh = 0, bestHighThresh = 0;
	GaussianBlur(template_gray_image, template_gray_image, cvSize(3, 3), 0);
	GetImageThreshold(template_gray_image, bestLowThresh, bestHighThresh);
	Canny(template_gray_image, edge, bestLowThresh, bestHighThresh);
	vector<vector<Point>> pre_contours;
	findContours(edge, pre_contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	vector<Feature> after_contours = cacluate_contours(pre_contours);
	vector<Feature> final_contours = CalGradientFeatures(template_gray_image, after_contours, threshold_value);
	// 得到轮廓
	base_rect = GetBoundingRectangle(final_contours);
	// 处理一下
	template_gray_image = template_gray_image(Range(base_rect.y, base_rect.y + base_rect.height),
		Range(base_rect.x, base_rect.x + base_rect.width));
	for (int i = 0; i < final_contours.size(); ++i) {
		final_contours[i].x = final_contours[i].x - base_rect.x;
		final_contours[i].y = final_contours[i].y - base_rect.y;
	}
	// 赋值
	contours = final_contours;
	return final_contours;
}

int ShapeMatch::TresholdOtsu(Mat im)
{
	int height = im.rows;
	int width = im.cols;

	//histogram  
	float histogram[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			histogram[im.at<uchar>(i,j)]++;
		}
	}
	//normalize histogram  
	int size = height * width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	//average pixel value  
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];//整幅图像的平均灰度
	}

	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];//假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例      
		u += i * histogram[i];// 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值 

		float t = avgValue * w - u;
		float variance = t * t / (w*(1 - w));
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}

	return threshold;
}

void ShapeMatch::GetImageThreshold(Mat image, int & bestLowThresh, int & bestHighThresh)
{
	int nGrayHistogram[256];
	double por_nGrayHistogram[256];
	double var = 0;
	double maxVar = 0;

	const int GrayLevel = 256;
	double allEpt = 0;
	double Ept[3] = { 0,0,0 };
	double por[3] = { 0,0,0 };
	int lowThresh = 0;
	int highThresh = 0;



	for (int i = 0; i < GrayLevel; i++)
	{
		nGrayHistogram[i] = 0;
	}

	int nPixel;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			nPixel = image.at<uchar>(i,j);
			nGrayHistogram[nPixel]++;
		}
	}

	int nSum = 0;
	for (int i = 0; i < GrayLevel; i++)
	{
		nSum += nGrayHistogram[i];
	}

	for (int i = 0; i < GrayLevel; i++)
	{
		por_nGrayHistogram[i] = 1.0*nGrayHistogram[i] / nSum;
	}

	for (int i = 0; i < GrayLevel; i++)
	{
		allEpt = i * por_nGrayHistogram[i];
	}

	for (lowThresh = 0; lowThresh < (GrayLevel - 1); lowThresh++)
		for (highThresh = (lowThresh + 1); highThresh < GrayLevel; highThresh++)
		{

			var = 0;
			Ept[0] = Ept[1] = Ept[2] = 0;
			por[0] = por[1] = por[2] = 0;

			for (int i = 0; i < lowThresh; i++)
			{
				por[0] += por_nGrayHistogram[i];
				Ept[0] += i * por_nGrayHistogram[i];
			}
			Ept[0] /= por[0];

			for (int i = lowThresh; i < highThresh; i++)
			{
				por[1] += por_nGrayHistogram[i];
				Ept[1] += i * por_nGrayHistogram[i];
			}
			Ept[1] /= por[1];

			for (int i = highThresh; i < GrayLevel; i++)
			{
				por[2] += por_nGrayHistogram[i];
				Ept[2] += i * por_nGrayHistogram[i];
			}
			Ept[2] /= por[2];

			for (int i = 0; i < 3; i++)
			{
				var += ((Ept[i] - allEpt)*(Ept[i] - allEpt)*por[i]);
			}

			if (var > maxVar)
			{
				maxVar = var;
				bestLowThresh = lowThresh;
				bestHighThresh = highThresh;
			}
		}
}

vector<Feature> ShapeMatch::cacluate_contours(vector<vector<Point>> pre_contours)
{
	vector<Feature> result_contours;
	for (size_t contourIdx = 0; contourIdx < pre_contours.size(); contourIdx++)
	{
		int pointNum = pre_contours[contourIdx].size();
		if (pointNum < 20)
		{
			if (pre_contours.size() > 2)
			{
				continue;
			}

		}
		int step = MAX(4, pointNum / 50);//特征点之间的间隔
		//////////////////////////////////////////
		for (size_t pointIdx = 0; pointIdx < pointNum; pointIdx += step)
		{
			if (pre_contours[contourIdx][pointIdx].x == 0 
				|| pre_contours[contourIdx][pointIdx].x == template_gray_image.cols - 1
				|| pre_contours[contourIdx][pointIdx].y == 0 
				|| pre_contours[contourIdx][pointIdx].y == template_gray_image.rows - 1)
				continue;
			int x = pre_contours[contourIdx][pointIdx].x;
			int y = pre_contours[contourIdx][pointIdx].y;//原轮廓点（image1图像坐标系）   
			result_contours.emplace_back(Feature(x, y,0,0));
		}
	}
	return result_contours;
}

vector<Feature> ShapeMatch::CalGradientFeatures(Mat image, vector<Feature> contours, int threshold_value)
{
	vector<Feature> result;
	//计算每个轮廓点梯度
	for (int contourIdx = 0; contourIdx < contours.size(); contourIdx++)
	{
		if (contours[contourIdx].x == 0 || contours[contourIdx].x ==image.cols - 1 
			|| contours[contourIdx].y == 0 || contours[contourIdx].y == image.rows - 1)
			continue;
		//continue;
		int x = contours[contourIdx].x;
		int y = contours[contourIdx].y;
		float dx, dy;
		int frameStride = image.step;
		uchar *srcPtr = (uchar*)image.data + y * frameStride + x;
		int sumX = 0, sumY = 0;
		if (result.size() < MAXFEANUM) {
			//extend the loop to speed up
			sumX = *(srcPtr - frameStride - 1) - *(srcPtr - frameStride + 1)
				+ *(srcPtr - 1) * 2 - *(srcPtr + 1) * 2
				+ *(srcPtr + frameStride - 1) - *(srcPtr + frameStride + 1);
			sumY = *(srcPtr - frameStride - 1) + *(srcPtr - frameStride) * 2 + *(srcPtr - frameStride + 1)
				- *(srcPtr + frameStride - 1) - *(srcPtr + frameStride) * 2 - *(srcPtr + frameStride + 1);
			int grad = sumX * sumX + sumY * sumY;
			double sqrtgrad = sqrt(grad);
			if (sqrtgrad > threshold_value)
			{
				dx = sumX / sqrtgrad;
				dy = sumY / sqrtgrad;
			}
			else
			{
				continue;
			}
			result.push_back(Feature{x, y, dx, dy});
		}

	}
	return result;


}

Rect ShapeMatch::GetBoundingRectangle(vector<Feature> contours)
{
	int minx = numeric_limits<int>::max(), miny = numeric_limits<int>::max();
	int maxx = numeric_limits<int>::min(), maxy = numeric_limits<int>::min();


	for (int i = 0; i < contours.size(); ++i)
	{
		if (minx > contours[i].x)
		{
			minx = contours[i].x;
		}
		else if (maxx < contours[i].x)
		{
			maxx = contours[i].x;
		}

		if (miny > contours[i].y)
		{
			miny = contours[i].y;
		}
		else if (maxy < contours[i].y)
		{
			maxy = contours[i].y;
		}
	}
	return Rect(minx, miny, maxx - minx + 1, maxy - miny + 1);
}

Mat ShapeMatch::find_object(Mat gray_image,vector<Mat> merge_im , Point & result_point, float& score)
{
	score = 0;
	double *m_dxImg = merge_im[0].ptr<double>(0);
	double *m_dyImg = merge_im[1].ptr<double>(0);
	int width = gray_image.cols;

	//计算相似度
	if (template_gray_image.rows> gray_image.rows||
		template_gray_image.cols > gray_image.cols)
		return Mat{};

	int Match_Greediness = 1;
	Feature feature;
	int x0, y0;
	float similarity = 0;
	Rect searchArea = Rect(0, 0, gray_image.cols - template_gray_image.cols,
		gray_image.rows - template_gray_image.rows);
	float min_score = 0.5;
	for (int i = searchArea.y; i < searchArea.y + searchArea.height/2; i++)
	{
		for (int j = searchArea.x; j < searchArea.x + searchArea.width; j++)
		{
			similarity = 0.0;
			for (int feaIdx = 0; feaIdx < contours.size(); feaIdx++)
			{
				feature = contours[feaIdx];
				x0 = feature.x + j;
				y0 = feature.y + i;
				if (!(x0 < gray_image.cols && y0 < gray_image.rows))
				{
					break;
				}
				similarity += *(m_dxImg + y0 * width + x0) * feature.dx
					+ *(m_dyImg + y0 *width + x0) * feature.dy;
				float curMinScore;
				if (Match_Greediness == 1)
				{
					curMinScore = min_score * feaIdx;
				}
				else if (Match_Greediness == 0)
				{
					curMinScore = (min_score - 1)* contours.size() + feaIdx;
				}
				else
				{
					curMinScore = min((min_score - 1)*contours.size() + (1 - Match_Greediness * min_score) / (1 - Match_Greediness)*feaIdx, min_score*feaIdx);
				}
				if (similarity < curMinScore)
				{
					break;
				}


			}
			similarity /= contours.size();
			if (similarity > score)
			{
				score = similarity;
				result_point.x = j;
				result_point.y = i;
			}
		}
	}
	Mat temp = draw_contours(gray_image, contours, result_point);

	return temp;
}

Mat ShapeMatch::draw_contours(Mat im, vector<Feature> contors, Point xy, Scalar color, int width)
{
	Mat result;
	
	if (im.channels() == 1) {
		vector<Mat> splt;
		for (int i = 0; i < 3; ++i)
			splt.push_back(im.clone());
		merge(splt, result);
	}
	else {
		result = im.clone();
	}
	Point temp_point;
	for (int i = 0; i < contors.size(); i++)
	{
		temp_point.x = contors[i].x+xy.x;
		temp_point.y = contors[i].y+xy.y;
		line(result, temp_point, temp_point, color, width);
	}
	return result;
}

vector<Mat> cal_search_feature(Mat gray_image, int minGradientMagnitude)
{
	int minSqrMagnitude = minGradientMagnitude * minGradientMagnitude;
	Mat dxImage = Mat::zeros(gray_image.size(), CV_64F);
	Mat dyImage = Mat::zeros(gray_image.size(), CV_64F);
	int kernelRadius = 1;
	int x0 = gray_image.cols - 2 * kernelRadius;     //kernelRadius = 1
	int y0 = gray_image.rows - 2 * kernelRadius;
	uchar *frame = gray_image.ptr<uchar>(0);
	double *dxPtr = dxImage.ptr<double>(0);
	double *dyPtr = dyImage.ptr<double>(0);
	int width = gray_image.cols;
#pragma omp parallel for
	for (int y = 0; y < y0; y++)
	{
		int idx0 = (y + kernelRadius)* width + kernelRadius;
		for (int x = 0; x < x0; x++)
		{
			uchar *srcPtr = frame + y * width + x;

			//extend the loop to speed up
			int sumX = *srcPtr - *(srcPtr + 2)
				+ *(srcPtr + width) * 2 - *(srcPtr + width + 2) * 2
				+ *(srcPtr + width * 2) - *(srcPtr + width * 2 + 2);
			int sumY = *srcPtr + *(srcPtr + 1) * 2 + *(srcPtr + 2)
				- *(srcPtr + width * 2) - *(srcPtr + width * 2 + 1) * 2 - *(srcPtr + width * 2 + 2);
			int idx = idx0 + x;
			int grad = sumX * sumX + sumY * sumY;


			if (grad >= minSqrMagnitude)
			{
				double sqrtgrad = sqrt(grad);
				*(dxPtr + idx) = sumX / sqrtgrad;
				*(dyPtr + idx) = sumY / sqrtgrad;
			}
			else
			{
				*(dxPtr + idx) = 0;
				*(dyPtr + idx) = 0;
			}


		}
	}
	vector<Mat> result;
	result.push_back(dxImage);
	result.push_back(dyImage);
	return result;
}
