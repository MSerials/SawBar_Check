#include "stdafx.h"
#include "opencv.h"

//注意图像训练的时候是否翻转！！！！！！
#pragma warning(disable:4267)

float opencv::ProcessImg(Mat& src, Rect rect)
{
	try{
		
		if (src.empty()) return 0;
		if ((rect.width + rect.x) > src.cols ||
			(rect.height + rect.y) > src.rows)
		{
			AfxMessageBox(L"感兴趣区域范围超过图像尺寸，无法检测！");
				return 0.0;
		}
		CString str;
		string tmp;
		cv::Mat gray, svm_gray, rect_gray, bin_img, Fiter;
		cv::Mat mask;//获得经过连通区域的掩码图像
		cv::Mat Matdilate;
		Convert2GRAY(src, gray);
		src(rect).copyTo(rect_gray);
			
if (g.ini.m_imageDealMode)
{
		int maxtrd = GetMaxGrayValue(rect_gray); int trd = get_OTSU_value(rect_gray);	
		int thres = (uchar)(trd + (maxtrd - trd) / 4);

		//cv::medianBlur(rect_gray, Fiter, 7);
		//cv::bilateralFilter(rect_gray, Fiter, g.mytest.val1, g.mytest.val2, g.mytest.val3, BORDER_DEFAULT);
		cv::bilateralFilter(rect_gray, Fiter, 16, 32, 8, BORDER_DEFAULT);
#ifdef TEST
		cv::adaptiveThreshold(Fiter, bin_img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 15, trd/2);
#else
		cv::threshold(Fiter, bin_img, thres, 255, THRESH_BINARY);
#endif
	

		vector<vector<cv::Point>> contours;
		Findcontours(bin_img,contours);
		delete_noneed_contours(bin_img, bin_img, contours, g.ini.m_iExistMax, g.ini.m_iExistMin); //从二值化里的图取出小斑点
		int MaxSerial = getSizeContours(contours, g.ini.m_iExistMax, g.ini.m_iExistMin);
		
		//AfxMessageBox(L"df");
		//第一次获得结果
		if (MaxSerial < 0) {
			CopyROIToImage(gray, bin_img, rect);
			showInfo(gray, "FRONT no rectangle found");
			src = gray.clone();
			return 1.0;
		}
	

		get_mask_image(bin_img, mask, contours ,-1,0);

		cv::dilate(mask, Matdilate, cv::Mat(), cv::Point(-1, -1), g.ini.m_blocksize);

		vector<cv::Rect> rt;
		cv::Mat dialate = Matdilate.clone();
		get_all_rect(bin_img, dialate, rt);

		m_ResultImg = bin_img.clone();
		
		int retangle_num = sign_all_rectangle(m_ResultImg, Scalar(255,0,255),rt);

		//第二次获得结果
		if (retangle_num > 1)
		{
			CopyROIToImage(gray, m_ResultImg, rect);
			showInfo(gray, "FRONT mutlti rectangle exist");
			src = gray.clone();
			return 1.0;
		}
		
		int imgwidth = rt[0].width - rt[0].x;
		int imgheight = rt[0].height - rt[0].y;

		if (imgheight > imgwidth)
		{
			imgheight ^= imgwidth;
			imgwidth ^= imgheight;
			imgheight ^= imgwidth;
		}
		float ratio = (float)(1.0*imgwidth / imgheight);
		str.Format(_T(" W=%d H=%d Ratio=%f"), imgwidth, imgheight, ratio);
		tmp = (string)(CStringA)(str);

		//第三次获得结果
		if (imgheight > g.ini.m_iExistHeightMax ||
			imgheight < g.ini.m_iExistHeightMin ||
			imgwidth > g.ini.m_iExistWidthMax ||
			imgwidth < g.ini.m_iExistWidthMin)
		{
			CopyROIToImage(gray, m_ResultImg, rect);
			showInfo(gray, "FRONT" + tmp);
			src = gray.clone();
			return 1.0;
		}

		//第四次获得结果
		if (ratio > g.ini.m_nWHRatio)
		{
			CopyROIToImage(gray, m_ResultImg, rect);
			showInfo(gray, "FRONT" + tmp);
			src = gray.clone();
			return 1.0;
		}
		CopyROIToImage(gray, m_ResultImg, rect);
}

		svm_gray = src.clone();
		Convert2GRAY(svm_gray, svm_gray);
		float result = hog_svm_predict(svm_gray);
		if (1.0 == result) showInfo(gray, "FRONT" + tmp);
		else if(-1.0 == result)  showInfo(gray, "BACK" + tmp);
		else showInfo(gray, "INVALID");
		src = gray.clone();
		return result;
	}
	catch (...)
	{
		AfxMessageBox(L"参数设置错误，请检查");
		return 0.0;
	}
}














opencv::opencv()
: mat_data0(NULL)
, mat_data1(NULL)
, mat_data2(NULL)
, mat_data3(NULL)
{
	cvInit();
	CameramatOK = false;
}

bool opencv::cvInit()
{
	//对svm进行初始化
	svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);//注意必须使用线性SVM进行训练，因为HogDescriptor检测函数只支持线性检测！！！
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON));
	return false;
}


float opencv::ProcessImg(Mat& src)
{
	try{
		if (src.empty()) return 1;
		Mat color, gray, binimg, bingray;
		Convert2BGR(src, color);
		Convert2GRAY(src, gray); Convert2GRAY(src, bingray);


		//exColor_Invert(gray);

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

		cv::findContours(binimg, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

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
		cv::Canny(gray, edge, 0, 255, 5);
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



void opencv::LoadMatImg(string str)
{
	mat0 = imread(str, -1);
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
	wid = src.cols;     hei = src.rows;
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

CString opencv::SaveImage(const Mat& src1, CString str)
{
	//写在一起了 供以后参考
	if (src1.empty()) return L"";
	
	Mat src = src1.clone();
	MonoInvert(src);//翻转图片
	//MonoInvert(src);
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

CString opencv::SaveImage(const Mat& src1,CString dir, CString str)
{
	//写在一起了 供以后参考
	if (src1.empty()) return L"";

	Mat src = src1.clone();
	MonoInvert(src);//翻转图片
	//MonoInvert(src);
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
	AppPath = AppPath + L"\\" + dir;
	CreateDirectory(AppPath, NULL);
	str = AppPath + L"\\" + str + L".bmp";

	std::string pstr = (LPCSTR)CStringA(str);
	imwrite(pstr, src);
	return str;
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


void opencv::MonoInvert(Mat& src)
{
	size_t widthStep = src.step;
	size_t rows = src.rows - 1;
	for (size_t r = 0; r < rows; r++, rows--)
		for (size_t step = 0; step < widthStep; step++)
		{
		src.data[r*widthStep + step] ^= src.data[rows*widthStep + step];
		src.data[rows*widthStep + step] ^= src.data[r*widthStep + step];
		src.data[r*widthStep + step] ^= src.data[rows*widthStep + step];
		}
}



bool opencv::CopyMatDataToMem(const Mat& src, uchar **data)
{
	//四字节对齐
	if (src.empty()) return false;
	if (*data) free(*data);
	size_t height = src.rows;
	size_t step = src.step;
	size_t offset = 0 == src.step % 4 ? 0 : 4 - src.step % 4;
	size_t size_i = (offset + src.step) * height;
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


void opencv::get_mask_image(const Mat& src, Mat& dst, vector<vector<Point>>& contours, int ID, int method, int thickness)
{
	if (src.empty()) return;

	const size_t widthStep = src.step;
	const size_t height = src.rows;
	uchar *sdata = src.data;
	if (0 == method)
	{
		Mat Mask(src.rows, src.cols, src.type(), Scalar(0, 0, 0));
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

void opencv::CopyROIToImage(Mat& src, Mat& ROI, Rect rect)
{
	int type = CV_8UC3, channel = 3;
	if (CV_8UC1 == src.type() && CV_8UC1 == ROI.type()) { type = CV_8UC1; channel = 1; }

	if (CV_8UC3 == type) { Convert2BGR(src, src); Convert2BGR(ROI, ROI); }
	else { Convert2GRAY(src, src); Convert2GRAY(ROI, ROI); }

	MonoInvert(src); MonoInvert(ROI);

	size_t widthBeginStep = src.step*rect.x / src.cols; 
	const size_t widthStep = src.step < (ROI.step + widthBeginStep) ? src.step : (ROI.step + widthBeginStep);
	const size_t height = src.rows < (rect.y + rect.height) ? src.rows : (rect.y + rect.height);

	uchar *sdata = src.data;
	sdata += src.step*(src.rows - rect.y - rect.height);
	uchar *rdata = ROI.data;

	for (int h = rect.y; h < height; h++)
		{
		for (int w = widthBeginStep; w < widthStep; w++)
		{
			sdata[w] = rdata[w - widthBeginStep];
		}
		sdata += src.step;
		rdata += ROI.step;
		}
	MonoInvert(src); MonoInvert(ROI);

}

// 获取白色最大值
int opencv::GetMaxGrayValue(const Mat& src)
{
	int value = 0;
	const size_t widthStep = src.step;
	const size_t height = src.rows;
	uchar *sdata = src.data;
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < widthStep; w++)
			{
				if(value < sdata[w])
					value = sdata[w];
			}
			sdata += widthStep;
			
		}
	return value;
}


int opencv::hog_svm_train()
{
	if (0 == g.TopSideImgPath.size()) { AfxMessageBox(L"正面图片信息的文件夹没有发现图片，只支持jpg和bmp格式！"); return -1; } //MFC所写，g.TopSideImgPath包含了所有正面图片的路径
	if (0 == g.BottomSideImgPath.size()) { AfxMessageBox(L"反面图片信息的文件夹没有发现图片，只支持jpg和bmp格式！"); return -1; } //类似上面
	try{
	int PosSamNO = g.TopSideImgPath.size();
	int NegSamNO = g.BottomSideImgPath.size();

	HOGDescriptor hog(Size(g.hog_width, g.hog_height), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG检测器，用来计算HOG描述子的  
	int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定  
	
	Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数      
	Mat sampleLabelMat;//训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人  


	//依次读取正样本图片，生成HOG描述子 
	for (int num = 0; num < PosSamNO; num++)
	{
		string ImgName = (LPCSTR)CStringA(g.TopSideImgPath[num]);
		Mat srcimg = imread(ImgName);//读取图片
		MonoInvert(srcimg);
		Mat src;
		srcimg(g.ImgROI).copyTo(src);	//的定义是Rect，复制感兴趣区域 Rect ImgROI;
		resize(src,src,Size(g.hog_width,g.hog_height)); 
	
		
#ifdef _DEBUG
		SaveImage(src);
		Sleep(1000);
#endif // _DEBUG

	

		vector<float> descriptors;//HOG描述子向量  
		hog.compute(src, descriptors, Size(8, 8));

#ifdef _DEBUG
		cout << descriptors.size() << endl;
#endif // _DEBUG

		//处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵  
		if (0 == num)
		{
			DescriptorDim = descriptors.size();//HOG描述子的维数  
			//初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat  
			sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO, DescriptorDim, CV_32FC1);
			//初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人  
			sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO, 1, CV_32SC1);
		}

		//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
		for (int i = 0; i<DescriptorDim; i++)
			sampleFeatureMat.at<float>(num, i) = descriptors[i];//第num个样本的特征向量中的第i个元素  
		sampleLabelMat.at<int>(num, 0) = 1;//正样本类别为1，有人  
	}



	//依次读取负样本图片，生成HOG描述子  
	for (int num = 0; num<NegSamNO; num++)
	{
		string ImgName = (LPCSTR)CStringA(g.BottomSideImgPath[num]);
		Mat srcimg = imread(ImgName);//读取图片
		MonoInvert(srcimg);
		Mat src;
		srcimg(g.ImgROI).copyTo(src);
		resize(src, src, Size(g.hog_width, g.hog_height));
		
		vector<float> descriptors;//HOG描述子向量  
		hog.compute(src, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)  


		//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
		for (int i = 0; i<DescriptorDim; i++)
			sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//第PosSamNO+num个样本的特征向量中的第i个元素  
		sampleLabelMat.at<int>(num + PosSamNO, 0) = -1;//负样本类别为-1，无人  
	}

	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	AppPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = AppPath + L"\\" + L"data";
	CreateDirectory(AppPath, NULL);
	AppPath = AppPath + L"\\traindata" +  + L".xml";
	g.train_data_path = AppPath;
	string trainPath = (LPCSTR)CStringA(AppPath);

	//使用SVM学习         
	svm->train(sampleFeatureMat, ROW_SAMPLE, sampleLabelMat);
	svm->save(trainPath);
	AfxMessageBox(L"训练完毕");
	}
	catch (...)
	{
		AfxMessageBox(L"错误提示：\n 1.请确认所有的图片可读，切为黑白，并且大小一致！\
\n2.没有设置感兴趣区域");
		return -1;
	}
	return 0;
}


float opencv::hog_svm_predict(const Mat& src)
{
	try{
	if (src.empty()) return 0;
	Mat img = src.clone();
	src(g.ImgROI).copyTo(img);
	resize(img, img, Size(g.hog_width, g.hog_height));

	//SaveImage(img);
	//Sleep(500);

	HOGDescriptor hog(Size(g.hog_width, g.hog_height), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG检测器，用来计算HOG描述子的  

	//	HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG检测器，用来计算HOG描述子的  
		int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定  
		Mat res;
		Mat sampleFeatureMat;

#ifdef _DEBUG
#endif
		vector<float> descriptors;//HOG描述子向量 
		descriptors.clear();
		hog.compute(img, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)  

#ifdef _DEBUG
		cout << descriptors.size() << endl;
#endif

		DescriptorDim = descriptors.size();//HOG描述子的维数  
		//初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat  
		sampleFeatureMat = Mat::zeros(1, DescriptorDim, CV_32FC1);
		//初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人  

		//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
		for (int i = 0; i < DescriptorDim; i++)
			sampleFeatureMat.at<float>(0, i) = descriptors[i];//第num个样本的特征向量中的第i个元素  

		svm->predict(sampleFeatureMat, res);
		if (res.empty()) return 0;
		return res.at<float>(0, 0);
		
	}
	catch (...)
	{
		AfxMessageBox(L"分类器调用失败");
		return 0;
	}
		
	return 0;
}





bool opencv::load_train_data()
{
	String path = "data\\traindata.xml";
	try{
	svm = Algorithm::load<SVM>(path);
	}
	catch (...)
	{
		AfxMessageBox(L"无法载入data\\traindata.xml文件，请重新训练！");
		return false;
	}
	return true;
}


void opencv::showInfo(Mat& src, string str)
{
	MonoInvert(src);
	if (CV_8UC1 == src.type())
		cvtColor(src, src, CV_GRAY2BGR);
	Point pstart;
	pstart.x = 10;
	pstart.y = 50;
	putText(src, str, pstart, 1, 2, Scalar(64, 64, 255), 2, 6, false);
	MonoInvert(src);
	return ;
}

int opencv::getSizeContours(vector<vector<Point>> &contours, size_t cmin, size_t cmax)
{
	if (cmax <= cmin)
	{
		cmax ^= cmin;
		cmin ^= cmax;
		cmax ^= cmin;
	}

	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) < cmin || (itc->size()) > cmax)
		{
			itc = contours.erase(itc);
		}
		else ++itc;
	}

	int Max = 0, MaxSerial = -2;
	
	for (int i = 0; i < contours.size(); ++i)
	{
		if (Max < contours[i].size())
		{
			Max = contours[i].size();
			MaxSerial = i;
		}
	}
	return MaxSerial;
}

void opencv::getAllContoursCenterPoint(const vector<vector<Point>>& contours, vector<PointAndSerial>& contoursCenter)
{
	for (int cnum = 0; cnum < contours.size(); cnum++)
	{
		Point pt;
		int sum = 1;
		int x = 0;
		int y = 0;
		for (int j = 0; j < contours[cnum].size(); ++j)
		{
			//bug
			pt += contours[cnum][j];
			++sum;
		}
		pt.x /= sum;
		pt.y /= sum;
		PointAndSerial PAS;
		PAS.pt = pt;
		PAS.Serial = cnum;
		contoursCenter.push_back(PAS);
	}
}



void opencv::drawAllContoursByColor(Mat& src, vector<vector<Point>>& contours)
{
	Convert2BGR(src, src);
	for (int i = 0; i < contours.size();++i)
		drawContours(src, contours, i/*获得的轮廓序号*/, Scalar(rand()%256,rand()%256,rand()%256), CV_FILLED/*2*/);
}


int opencv::caculate_distance(Point& st, Point& ed)
{
	return abs(st.x - ed.x + st.y - ed.y);
}


inline void opencv::get_all_rect(const Mat& bin, const Mat& dialate, vector<cv::Rect>& rect)
{
	if (bin.type() != dialate.type() && bin.type() != CV_8UC1) return;
	if (bin.size() != dialate.size()) return;
	
	vector<vector<Point>> contours;
	static cv::Mat b;
	cv::threshold(dialate, b, 2, 255, THRESH_BINARY);
	cv::findContours(b, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	const size_t widthStep = bin.step;
	const size_t height = bin.rows;
	const size_t width = bin.cols;

	for (int i = 0; i < contours.size(); ++i)
	{
		cv::Mat mask(bin.rows, bin.cols, CV_8UC1, Scalar(0));
		uchar *mdata = mask.data;
		uchar *bdata = bin.data;
		drawContours(mask, contours, i, Scalar(255), CV_FILLED/*2*/);

		Rect rt(width,height,0,0);
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < widthStep; w++)
			{
					mdata[w] &= bdata[w];
			}
			mdata += widthStep;
			bdata += widthStep;
		}
		
		mdata = mask.data;
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < widthStep; w++)
			{
				if (mdata[w])
				{
					if (w<rt.x) rt.x = w;
					if (h<rt.y) rt.y = h;
					if (h >rt.height) rt.height = h;
					if (w >rt.width) rt.width = w;
				}
			}
			mdata += widthStep;
		}	
		rect.push_back(rt);
	}
}


int opencv::sign_all_rectangle(Mat& sign, Scalar color, vector<cv::Rect>& rt)
{
	Convert2BGR(sign, sign);
	for (int rtCounter = 0; rtCounter < rt.size(); rtCounter++)
	{
		Point st, ed;
		st.x = rt[rtCounter].x; st.y = rt[rtCounter].y; ed.x = rt[rtCounter].width; ed.y = rt[rtCounter].height;
		rectangle(sign, st, ed, color, 2, 1, 0);
	}
	return rt.size();
}


void opencv::Findcontours(const Mat& src, vector<vector<cv::Point>>& Pt)
{
	cv::Mat bin = src.clone();
	try{
		Convert2GRAY(bin,bin);
		cv::findContours(bin, Pt, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	}
	catch (...)
	{
		AfxMessageBox(L"BUG");
	}
}


void opencv::delete_noneed_contours(const Mat& src, Mat& dst, vector<vector<cv::Point>>& contours, int cmin, int cmax)
{
	cv::Mat tmp = src.clone();
	if (cmin > cmax)
	{
		cmin ^= cmax;
		cmax ^= cmin;
		cmin ^= cmax;
	}

	for (int i = 0; i < contours.size(); ++i)
	{
		int l_size = contours[i].size();
		if (l_size < cmin || l_size>cmax)
			drawContours(tmp, contours, i/*获得的轮廓序号*/, Scalar(0), CV_FILLED/*2*/);
	}
	dst = tmp.clone();
}
