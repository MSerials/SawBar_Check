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
	Mat OldMat;             //�������ͼ�����һ��һģһ����˵���ɼ�����ȷ������
	Mat mat0;				//��һ����ͼ��
	Mat mat1;				//�ڶ�����ͼ��
	Mat mat2;				//��������ͼ��
	Mat mat3;				//���Ļ���ͼ��
	Mat Cameramat;			//���������ͼ�񻺴�
	bool CameramatOK;		//ȡͼƬ�ж�;
	uchar *mat_data0;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat
	uchar *mat_data1;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat1
	uchar *mat_data2;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat2
	uchar *mat_data3;	//����ͼ��������Ҫ���ֽڶ���,��Ӧ��mat3


	struct PointAndSerial
	{
		cv::Point pt;
		int Serial;
	};
	// ԭʼͼƬ
	Mat m_RawImg;
	// ������ͼƬ
	Mat m_ResultImg;

	void LoadMatImg(string str);

	void swapValue(uchar& a, uchar& b);			//����ֵ

	LPBITMAPINFO CreateMapInfo(const Mat& src);	//����ͼ��ͷ

	//bool CopyMatDataToMem(const Mat& src, uchar	**data);//���ֽڶ���ͼ������

	bool BmpToMat(BITMAPINFO* bmpinfo, BYTE* imageData, Mat& dst);//bmpתmat

	CString SaveImage(const Mat& src1, CString str = L"");	//����ͼƬ
	CString SaveImage(const Mat& src1, CString dir, CString str);

	float ProcessImg(Mat& src);
	float ProcessImg(Mat& src, Rect rect);
	void Convert2GRAY(const Mat &src, Mat& dst);
	void Convert2BGR(const Mat &src, Mat& dst);
	void MonoInvert(Mat& src);					//ͼ��תһ��
	bool CopyMatDataToMem(const Mat& src, uchar	**data);//���ֽڶ���ͼ������
	void get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method = 1, int thickness = -1);
	void CopyROIToImage(Mat& src, Mat& ROI, Rect rect);
	bool cvInit();
	



	Ptr<SVM> svm;
	vector<KeyPoint> keypoints;

	Ptr<KNearest> knn;
	

	// ��ȡ��ɫ���ֵ
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
