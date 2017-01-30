/*
	this code is for study
	and use it should be installed opencv in your computer
*/
#include "opencv2/opencv.hpp"
#include "math.h"
//#include "..\imgDeal.h"



#ifdef _DEBUG
#include <iostream>
	using namespace std;
#endif

class Splearnging
{
public:
	Splearnging(){};
	~Splearnging(){};

protected:
	
private:
public:
	//所有函数都是用double来计算
#if 0
	template<typename _Tp> void logistic_regression(const Mat& theta, Mat& X, Mat& y, double &f, Mat& g);
	template<typename _Tp> void linear_regression(Mat& theta, Mat& X, Mat& y, double &f, Mat& g);
	template<typename _Tp> void linear_regression_vec(Mat& theta, Mat& X, Mat& y, double &f, Mat& g);
#endif

//对矩阵每一个元素都进行幂运算
template<typename _Tp> inline
void mat_pow(Mat& src, int i)
{
		size_t width = (int)src.cols;
		size_t height = (int)src.rows;
		_Tp* data = (_Tp*) src.data;
			
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				_Tp t = data[w];
				data[w] = pow(t, i);
			}
			data += width;
		}
}

//对矩阵的每一个元素都进行sigmoid计算
template<typename _Tp> inline
Mat& sigmoid_mat(Mat &src)
{
	size_t width = (int)src.cols;
	size_t height = (int)src.rows;
	_Tp* data = (_Tp*)src.data;
	 
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			data[w] = (_Tp)sigmoid((double)data[w]); 
		}
		data += width;
	}
	return src;
}



//this code segment need opencv 
//内存校验 b
template<typename _Tp>
void gradient(Mat &theta, Mat &X, Mat &y, Mat &grad)
{
	try{
		Mat t = theta.t();
		grad = X*(y - sigmoid_mat<_Tp>(t)).t();		
	}
	catch (...)
	{
#ifdef _DEBUG
		cout << "matrix calculate wrong!" << endl;
#endif // _DEBUG
		grad.release();
		return;
	}
}

template<typename _Tp> inline
void  covariance(Mat &src1, Mat &src2, Mat& dst)
{
	const int rowsize = src1.rows();
	try
	{
	}
	catch (...)
	{
		
	}
	

}

double template_match_NCC(Mat& src, Mat& tpl, double *AngleLimts, Rect rect, double &Anlge, int /*&t*/*range)
{
	Mat gx, gy;
	Sobel(src, gx, 1, 0, 3);
	Sobel(src, gy, 0, 1, 3);
	imshow("src",src);
	imshow("gx",gx);
	imshow("gy",gy);


}

template<typename _Tp> inline
double covariance(Mat &theta, Mat &h, Mat &y)
{


}




};
