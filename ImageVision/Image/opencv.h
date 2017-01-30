#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "E:\sunkun\AET\ImageVision\ImageVision\include\opencv2\core\mat.hpp"

using namespace std;
using namespace cv;

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
	Mat mat;				//第一缓存图像
	Mat mat1;				//第二缓存图像
	Mat mat2;				//第三缓存图像
	Mat mat3;				//第四缓存图像
	uchar *mat_data;	//由于图像问题需要四字节对齐,对应给mat
	uchar *mat_data1;	//由于图像问题需要四字节对齐,对应给mat1
	uchar *mat_data2;	//由于图像问题需要四字节对齐,对应给mat2
	uchar *mat_data3;	//由于图像问题需要四字节对齐,对应给mat3


	inline void Convert2GRAY(const Mat &src, Mat& dst);

	inline void Convert2BGR(const Mat &src, Mat& dst);

	void CreateDoubleMatrix(double **&matrix, cv::Size size);

	int CreateGeoMatchModel(const Mat& templateArr, double maxContrast, double minContrast);

	void LoadMatImg(string str);				//载入图像

	void MonoInvert(Mat& src);					//图像翻转一下

	void swapValue(uchar& a, uchar& b);			//交换值

	LPBITMAPINFO CreateMapInfo(const Mat& src);	//创建图像头

	bool CopyMatDataToMem(const Mat& src, uchar	**data);//四字节对齐图像数据

	bool BmpToMat(BITMAPINFO* bmpinfo, BYTE* imageData, Mat& dst);//bmp转mat

	void get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method = 1, int thickness = -1);

	CString SaveImage(const Mat& src, CString str = L"");	//保存图片

	int ProcessImg(Mat& src);
	// 原始图片
	Mat m_RawImg;
	// 处理后的图片
	Mat m_ResultImg;

	vector<KeyPoint> keypoints;
};

extern opencv* opcv;