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
	Mat mat;				//��һ����ͼ��
	Mat mat1;				//�ڶ�����ͼ��
	Mat mat2;				//��������ͼ��
	Mat mat3;				//���Ļ���ͼ��
	uchar *mat_data;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat
	uchar *mat_data1;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat1
	uchar *mat_data2;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat2
	uchar *mat_data3;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat3


	inline void Convert2GRAY(const Mat &src, Mat& dst);

	inline void Convert2BGR(const Mat &src, Mat& dst);

	void CreateDoubleMatrix(double **&matrix, cv::Size size);

	int CreateGeoMatchModel(const Mat& templateArr, double maxContrast, double minContrast);

	void LoadMatImg(string str);				//����ͼ��

	void MonoInvert(Mat& src);					//ͼ��תһ��

	void swapValue(uchar& a, uchar& b);			//����ֵ

	LPBITMAPINFO CreateMapInfo(const Mat& src);	//����ͼ��ͷ

	bool CopyMatDataToMem(const Mat& src, uchar	**data);//���ֽڶ���ͼ������

	bool BmpToMat(BITMAPINFO* bmpinfo, BYTE* imageData, Mat& dst);//bmpתmat

	void get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method = 1, int thickness = -1);

	CString SaveImage(const Mat& src, CString str = L"");	//����ͼƬ

	int ProcessImg(Mat& src);
	// ԭʼͼƬ
	Mat m_RawImg;
	// ������ͼƬ
	Mat m_ResultImg;

	vector<KeyPoint> keypoints;
};

extern opencv* opcv;