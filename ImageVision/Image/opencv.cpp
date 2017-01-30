#include "..\\stdafx.h"
#include "opencv.h"


opencv* opcv = NULL;

opencv::opencv()
: mat_data(NULL)
, mat_data1(NULL)
, mat_data2(NULL)
, mat_data3(NULL)
{
}


int opencv::ProcessImg(Mat& src)
{
	try{
		if (src.empty()) return 1;
		Mat color, gray, binimg, bingray;
		Convert2BGR(src, color);
		Convert2GRAY(src, gray); Convert2GRAY(src, bingray);

		exColor_Invert(gray);

		vector<vector<Point>> contours;

		int trd = get_OTSU_value(gray);

		threshold(bingray, binimg, trd, 255, THRESH_BINARY);



		Mat Fiter;

#if 1
		//双边滤波
		bilateralFilter(gray, Fiter, 15, 50, 2.0, BORDER_DEFAULT);

#else

		//中值滤波
		int kenerlsize = gray.rows / 150;
		if (0 == kenerlsize % 2) kenerlsize += 1;
		kenerlsize = kenerlsize > 2 ? kenerlsize : 3;
		medianBlur(gray, gray, kenerlsize);
#endif
		//Histogram(gray, gray);

		findContours(binimg, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		Mat imgmask(binimg.rows, binimg.cols, CV_8UC1, Scalar(255));
		//drawContours(imgmask, contours, -1, Scalar(255), CV_FILLED/*2*/);   // -1 表示所有轮廓
		//drawContours(imgmask, contours, -1, Scalar(0), 2);   // -1 表示所有轮廓
#ifdef _DEBUG
		for (size_t i = 0; i < contours.size(); i++)
			cout << contours[i].size()<<endl;
#endif
		get_mask_image(src, imgmask, contours, -1,1,2);
		src = imgmask.clone();
		return 1;

		Convert2GRAY(gray, gray);
		Mat edge;
		Canny(gray, edge, 0, 255, 5);
		src = edge.clone();
		return NoError;


		//模式识别
	//	ANN_MLP ann;
	//	ann.train();




	}
	catch (...)
	{
#ifdef _DEBUG
		cout << "img deal wrongly" << endl;

#endif
		return -1;
	}
}


opencv::~opencv()
{
}

void opencv::CreateDoubleMatrix(double **&matrix, cv::Size size)
{
	matrix = new double*[size.height];
	for (int iInd = 0; iInd < size.height; iInd++)
		matrix[iInd] = new double[size.width];
}

int opencv::CreateGeoMatchModel(const Mat& templateArr, double maxContrast, double minContrast)
{
	if (CV_8UC1 != templateArr.type())
	{
		return 0;
	}
#if 0

	Mat src = templateArr.clone();
	CvSize Ssize;

	// Convert IplImage to Matrix for integer operations
	//CvMat srcstub, *src = (CvMat*)templateArr;
  


	// set width and height
	Ssize.width = src.cols;
	Ssize.height = src.rows;
	modelHeight = src.rows;		//Save Template height
	modelWidth = src.cols;			//Save Template width

	noOfCordinates = 0;											//initialize
	if (cordinates) delete cordinates;
	cordinates = new Point[modelWidth *modelHeight];		//Allocate memory for coorinates of selected points in template image

	if (edgeMagnitude) delete edgeMagnitude;
	edgeMagnitude = new double[modelWidth *modelHeight];		//Allocate memory for edge magnitude for selected points

	if (edgeDerivativeX) delete edgeDerivativeX;
	edgeDerivativeX = new double[modelWidth *modelHeight];			//Allocate memory for edge X derivative for selected points

	if (edgeDerivativeY) delete edgeDerivativeY;
	edgeDerivativeY = new double[modelWidth *modelHeight];			////Allocate memory for edge Y derivative for selected points


	// Calculate gradient of Template
	Mat gx(Ssize.height, Ssize.width, CV_16SC1);		//create Matrix to store X derivative
	Mat gy(Ssize.height, Ssize.width, CV_16SC1);		//create Matrix to store Y derivative
	cvSobel(src, gx, 1, 0, 3);		//gradient in X direction			
	cvSobel(src, gy, 0, 1, 3);	//gradient in Y direction

	Mat nmsEdges(Ssize.height, Ssize.width, CV_32F);		//create Matrix to store Final nmsEdges
	const short* _sdx;
	const short* _sdy;
	double fdx, fdy;
	double MagG, DirG;
	double MaxGradient = -99999.99;
	double direction;

	int *orients = new int[Ssize.height *Ssize.width];
	int count = 0, i, j; // count variable;

	double **magMat;
	CreateDoubleMatrix(magMat, Ssize);

	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width - 1; j++)
		{
			_sdx = (short*)(gx->data.ptr + gx->step*i);
			_sdy = (short*)(gy->data.ptr + gy->step*i);
			fdx = _sdx[j]; fdy = _sdy[j];        // read x, y derivatives

			MagG = sqrt((float)(fdx*fdx) + (float)(fdy*fdy)); //Magnitude = Sqrt(gx^2 +gy^2)
			direction = cvFastArctan((float)fdy, (float)fdx);	 //Direction = invtan (Gy / Gx)
			magMat[i][j] = MagG;

			if (MagG > MaxGradient)
				MaxGradient = MagG; // get maximum gradient value for normalizing.


			// get closest angle from 0, 45, 90, 135 set
			if ((direction>0 && direction < 22.5) || (direction >157.5 && direction < 202.5) || (direction>337.5 && direction < 360))
				direction = 0;
			else if ((direction > 22.5 && direction < 67.5) || (direction >202.5 && direction <247.5))
				direction = 45;
			else if ((direction >67.5 && direction < 112.5) || (direction>247.5 && direction<292.5))
				direction = 90;
			else if ((direction >112.5 && direction < 157.5) || (direction>292.5 && direction < 337.5))
				direction = 135;
			else
				direction = 0;

			orients[count] = (int)direction;
			count++;
		}
	}

	count = 0; // init count
	// non maximum suppression
	double leftPixel, rightPixel;

	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width - 1; j++)
		{
			switch (orients[count])
			{
			case 0:
				leftPixel = magMat[i][j - 1];
				rightPixel = magMat[i][j + 1];
				break;
			case 45:
				leftPixel = magMat[i - 1][j + 1];
				rightPixel = magMat[i + 1][j - 1];
				break;
			case 90:
				leftPixel = magMat[i - 1][j];
				rightPixel = magMat[i + 1][j];
				break;
			case 135:
				leftPixel = magMat[i - 1][j - 1];
				rightPixel = magMat[i + 1][j + 1];
				break;
			}
			// compare current pixels value with adjacent pixels
			if ((magMat[i][j] < leftPixel) || (magMat[i][j] < rightPixel))
				(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
			else
				(nmsEdges->data.ptr + nmsEdges->step*i)[j] = (uchar)(magMat[i][j] / MaxGradient * 255);

			count++;
		}
	}


	int RSum = 0, CSum = 0;
	int curX, curY;
	int flag = 1;

	//Hysterisis threshold
	for (i = 1; i < Ssize.height - 1; i++)
	{
		for (j = 1; j < Ssize.width; j++)
		{
			_sdx = (short*)(gx->data.ptr + gx->step*i);
			_sdy = (short*)(gy->data.ptr + gy->step*i);
			fdx = _sdx[j]; fdy = _sdy[j];

			MagG = sqrt(fdx*fdx + fdy*fdy); //Magnitude = Sqrt(gx^2 +gy^2)
			DirG = cvFastArctan((float)fdy, (float)fdx);	 //Direction = tan(y/x)

			////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]= MagG;
			flag = 1;
			if (((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j]) < maxContrast)
			{
				if (((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j]) < minContrast)
				{

					(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
					flag = 0; // remove from edge
					////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
				}
				else
				{   // if any of 8 neighboring pixel is not greater than max contraxt remove from edge
					if ((((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i - 1)))[j + 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*i))[j + 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j - 1]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j]) < maxContrast) &&
						(((double)((nmsEdges->data.ptr + nmsEdges->step*(i + 1)))[j + 1]) < maxContrast))
					{
						(nmsEdges->data.ptr + nmsEdges->step*i)[j] = 0;
						flag = 0;
						////((uchar*)(imgGDir->imageData + imgGDir->widthStep*i))[j]=0;
					}
				}

			}

			// save selected edge information
			curX = i;	curY = j;
			if (flag != 0)
			{
				if (fdx != 0 || fdy != 0)
				{
					RSum = RSum + curX;	CSum = CSum + curY; // Row sum and column sum for center of gravity

					cordinates[noOfCordinates].x = curX;
					cordinates[noOfCordinates].y = curY;
					edgeDerivativeX[noOfCordinates] = fdx;
					edgeDerivativeY[noOfCordinates] = fdy;

					//handle divide by zero
					if (MagG != 0)
						edgeMagnitude[noOfCordinates] = 1 / MagG;  // gradient magnitude 
					else
						edgeMagnitude[noOfCordinates] = 0;

					noOfCordinates++;
				}
			}
		}
	}

	centerOfGravity.x = RSum / noOfCordinates; // center of gravity
	centerOfGravity.y = CSum / noOfCordinates;	// center of gravity

	// change coordinates to reflect center of gravity
	for (int m = 0; m < noOfCordinates; m++)
	{
		int temp;

		temp = cordinates[m].x;
		cordinates[m].x = temp - centerOfGravity.x;
		temp = cordinates[m].y;
		cordinates[m].y = temp - centerOfGravity.y;
	}

	////cvSaveImage("Edges.bmp",imgGDir);

	// free alocated memories
	delete[] orients;
	////cvReleaseImage(&imgGDir);
	cvReleaseMat(&gx);
	cvReleaseMat(&gy);
	cvReleaseMat(&nmsEdges);

	ReleaseDoubleMatrix(magMat, Ssize.height);

	modelDefined = true;

	
#endif
	return 0;

}

void opencv::Convert2GRAY(const Mat &src, Mat& dst)
{
	if (CV_8UC3 == src.type())
		cv::cvtColor(src, dst, CV_BGR2GRAY);
	else
		dst = src.clone();
}

void opencv::Convert2BGR(const Mat &src, Mat& dst)
{
	if (CV_8UC1 == src.type())
		cv::cvtColor(src, dst, CV_GRAY2BGR);
	else
		dst = src.clone();

}

void opencv::LoadMatImg(string str)
{
	mat = imread(str, -1);
}

//CreateGeoMatchModel(const void *templateArr,double maxContrast,double minContrast)

void opencv::MonoInvert(Mat& src)
{
	size_t widthStep = mat.step;
	size_t rows = mat.rows - 1;
	for (size_t r = 0; r < rows; r++, rows--)
	for (size_t step = 0; step<widthStep; step++)
	{
		swapValue(mat.data[r*widthStep + step], mat.data[rows*widthStep + step]);
	}
}


void opencv::swapValue(uchar& a, uchar& b)
{
	a = a^b;
	b = a^b;
	a = a^b;
}


LPBITMAPINFO opencv::CreateMapInfo(const Mat& src)
{
	BITMAPINFOHEADER BIH = { 40, 1, 1, 1, 8, 0, 0, 0, 0, 0, 0 };
	LPBITMAPINFO lpBmi;
	int          wid, hei, bits, colors, i, depth, channels;
	RGBQUAD  ColorTab[256];
	wid = mat.cols;     hei = mat.rows;
	if (CV_8UC1 == src.type())
	{
		depth = 8; channels = 1;
	}
	else if (CV_8UC3 == src.type())
	{
		depth = 8; channels = 3;
	}
	else return NULL;
	bits = depth * channels;
	if (bits>8) colors = 0;
	else colors = 1 << bits;
	lpBmi = (LPBITMAPINFO)malloc(40 + 4 * colors);
	BIH.biWidth = wid;     BIH.biHeight = hei;
	BIH.biBitCount = (BYTE)bits;
	memcpy(lpBmi, &BIH, 40);                   //  复制位图信息头
	if (bits == 8) {                           //  256 色位图
		for (i = 0; i<256; i++)  {                //  设置灰阶调色板
			ColorTab[i].rgbRed = ColorTab[i].rgbGreen = ColorTab[i].rgbBlue = (BYTE)i;
		}
		memcpy(lpBmi->bmiColors, ColorTab, 1024);
	}
	return(lpBmi);
}


bool opencv::CopyMatDataToMem(const Mat& src, uchar **data)
{
	//四字节对齐
	if (src.empty()) return false;
	if (*data) free(*data);
	size_t height = src.rows;
	size_t step = src.step;
	size_t offset = 0 == src.step % 4 ? 0 : 4 - src.step % 4;
	size_t size_i = (offset + mat.step) * height;
	*data = (uchar*)malloc(size_i);
	if (NULL == *data) return false;
	uchar *idata = src.data;
	uchar *ddata = *data;
	for (int h = 0; h < height; h++)
	{
		memcpy(ddata, idata, step);
		ddata = ddata + step + offset;
		idata = idata + step;
	}
	return true;
}


bool opencv::BmpToMat(BITMAPINFO* bmpinfo, BYTE* imageData, Mat& dst)
{
	try{
		if (!imageData) return false;
		int size = bmpinfo->bmiHeader.biWidth*bmpinfo->bmiHeader.biHeight;	
		dst.create(Size(bmpinfo->bmiHeader.biWidth, bmpinfo->bmiHeader.biHeight), CV_8UC1);
		memcpy(dst.data, (uchar*)imageData, size);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void opencv::get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method, int thickness)
{
	if (src.empty()) return;

	const size_t widthStep = src.step;
	const size_t height = src.rows;
	uchar *sdata = src.data;
	if (0 == method)
	{
		Mat Mask(src.rows,src.cols, src.type(), Scalar(0,0,0));
		uchar *ddata = Mask.data;
		drawContours(Mask, contours, ID/*获得的轮廓序号*/, Scalar(255, 255, 255), thickness);

		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < widthStep; w++)
			{
				ddata[w] &= sdata[w];
			}
			sdata += widthStep;
			ddata += widthStep;
		}
		dst = Mask.clone();
	}
	else
	{
		Mat Mask(src.rows, src.cols, src.type(), Scalar(255, 255, 255));
		uchar *ddata = Mask.data;
		drawContours(Mask, contours, ID/*获得的轮廓序号*/, Scalar(0, 0, 0), thickness);

		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < widthStep; w++)
			{
				ddata[w] |= sdata[w];
			}
			sdata += widthStep;
			ddata += widthStep;
		}
		dst = Mask.clone();
	}	
}

CString opencv::SaveImage(const Mat& src, CString str)
{
	//写在一起了 供以后参考
	if (src.empty()) return L"";

	//获取时间
	if (L"" == str)
	{
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%d-%d_"), st.wYear, st.wMonth, st.wDay);
		strTime.Format(_T("%d-%d-%d"), st.wHour, st.wMinute, st.wSecond);
		str = strDate + strTime;
	}
	//获取路径
	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	AppPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = AppPath + L"\\" + L"Image";
	CreateDirectory(AppPath, NULL);
	str = AppPath + L"\\" + str + L".bmp";

	//CString转成char*
	std::string pstr = (LPCSTR)CStringA(str);
	imwrite(pstr, src);
	return str;
}

