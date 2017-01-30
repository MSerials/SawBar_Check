#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "opencv2\core\mat.hpp"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;



#ifndef ____OPENCV_DEAL____
#define ____OPENCV_DEAL____

class opencv
{

private:
	int				noOfCordinates;		//Number of elements in coordinate array
	Point			*cordinates;		//Coordinates array to store model points	
	int				modelHeight;		//Template height
	int				modelWidth;			//Template width
	double			*edgeMagnitude;		//gradient magnitude
	double			*edgeDerivativeX;	//gradient in X direction
	double			*edgeDerivativeY;	//radient in Y direction	
	Point			centerOfGravity;	//Center of gravity of template 
	bool			modelDefined;

public:
	opencv();
	~opencv();
	Mat OldMat;             //如果发现图像和上一次一模一样，说明采集不正确，报警
	Mat mat0;				//第一缓存图像
	Mat mat1;				//第二缓存图像
	Mat mat2;				//第三缓存图像
	Mat mat3;				//第四缓存图像
	Mat Cameramat;			//用于摄像机图像缓存
	bool CameramatOK;		//取图片判断;
	uchar *mat_data0;	//由于图像问题需要四字节对齐,对应给mat
	uchar *mat_data1;	//由于图像问题需要四字节对齐,对应给mat1
	uchar *mat_data2;	//由于图像问题需要四字节对齐,对应给mat2
	uchar *mat_data3;	//由于图像问题需要四字节对齐,对应给mat3


	struct PointAndSerial
	{
		cv::Point pt;
		int Serial;
	};
	// 原始图片
	Mat m_RawImg;
	// 处理后的图片
	Mat m_ResultImg;

	void LoadMatImg(string str);

	void swapValue(uchar& a, uchar& b);			//交换值

	LPBITMAPINFO CreateMapInfo(const Mat& src);	//创建图像头

	//bool CopyMatDataToMem(const Mat& src, uchar	**data);//四字节对齐图像数据

	bool BmpToMat(BITMAPINFO* bmpinfo, BYTE* imageData, Mat& dst);//bmp转mat

	CString SaveImage(const Mat& src1, CString str = L"");	//保存图片
	CString SaveImage(const Mat& src1, CString dir, CString str);

	float ProcessImg(Mat& src);
	float ProcessImg(Mat& src, Rect rect);
	void Convert2GRAY(const Mat &src, Mat& dst);
	void Convert2BGR(const Mat &src, Mat& dst);
	void MonoInvert(Mat& src);					//图像翻转一下
	bool CopyMatDataToMem(const Mat& src, uchar	**data);//四字节对齐图像数据
	void get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method = 1, int thickness = -1);
	void CopyROIToImage(Mat& src, Mat& ROI, Rect rect);
	bool cvInit();
	



	Ptr<SVM> svm;
	vector<KeyPoint> keypoints;

	Ptr<KNearest> knn;
	

	// 获取白色最大值
	int GetMaxGrayValue(const Mat& src);
	int hog_svm_train();
	float hog_svm_predict(const Mat& src);
	bool load_train_data();
	void showInfo(Mat& src, string str);
	int getSizeContours(vector<vector<Point>> &contours, size_t cmin, size_t cmax);
	void getAllContoursCenterPoint(const vector<vector<Point>>& contours, vector<PointAndSerial>& contoursCenter);
	void drawAllContoursByColor(Mat& src, vector<vector<Point>>& contours);
	int caculate_distance(Point& st, Point& ed);
	void get_all_rect(const Mat& bin, const Mat& dialate, vector<cv::Rect>& rect);
	int sign_all_rectangle(Mat& sign, Scalar color, vector<cv::Rect>& rt);
	void Findcontours(const Mat& src, vector<vector<cv::Point>>& Pt);
	void delete_noneed_contours(const Mat& src, Mat& dst, vector<vector<cv::Point>>& contours, int cmin, int cmax);
};


#endif


#if !defined WATERSHS
#define WATERSHS


class WatershedSegmenter {

private:

	cv::Mat markers;

public:

	void setMarkers(const cv::Mat& markerImage) {

		// Convert to image of ints
		markerImage.convertTo(markers, CV_32S);
	}

	cv::Mat process(const cv::Mat &image) {

		// Apply watershed
		cv::watershed(image, markers);
		markers.convertTo(markers, CV_8U);
		return markers;
	}

	// Return result in the form of an image
	cv::Mat getSegmentation() {

		cv::Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp, CV_8U);

		return tmp;
	}

	// Return watershed in the form of an image
	cv::Mat getWatersheds() {

		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U, 255, 255);

		return tmp;
	}
};


#endif
