#pragma once
#include "stdafx.h"
#include "halcon.h"

// CImageDeal ÃüÁîÄ¿±ê
#ifndef __DEAL_IMAGE___
#define  __DEAL_IMAGE___


#include <vector>
using namespace std;

class CImageDeal : public CObject
{
public:
	CImageDeal();
	virtual ~CImageDeal();
public:
	
	bool Back_Train(const std::vector<std::string>& imgpath, const HTuple& WinHandle, const _Params &params);// { return true; };
	bool Front_Train(const std::vector<std::string>& imgpath, const HTuple& WinHandle, const _Params &params);
	int Front_Deal(const Hobject src, const HTuple& WinHandle, const _Params &params);//image roi,result
	int Back_Deal(const Hobject src, const HTuple& WinHandle, const _Params &params);// { return 0; }//image roi,result

private:

//public:
	Hobject  TopoImage, TopbImage, TopROI_0, TopImage, TopRegion;
	Hobject  TopRegionFillUp, TopRegionDifference, TopRegionTrans, TopRegionDilation;
	Hobject  TopImageReduced, TopModelImages, TopModelRegions, TopModel;
	Hobject  TopImageTrans, TopMeanImage, TopVarImage, TopRegionROI, TopRegionDiff;
	Hobject  TopConnectedRegions, TopRegionsError;


	Hobject  BottomoImage, BottombImage, BottomROI_0, BottomImage, BottomRegion;
	Hobject  BottomRegionFillUp, BottomRegionDifference, BottomRegionTrans, BottomRegionDilation;
	Hobject  BottomImageReduced, BottomModelImages, BottomModelRegions, BottomModel;
	Hobject  BottomImageTrans, BottomMeanImage, BottomVarImage, BottomRegionROI, BottomRegionDiff;
	Hobject  BottomConnectedRegions, BottomRegionsError;

	// Local control variables 
	HTuple  Topnumofmodel, Topimagepath, Topirow, Topicol, Topiwidth;
	HTuple  Topiheight, TopWidth, TopHeight, TopWindowHandle, TopOS, TopArea;
	HTuple  TopRowRef, TopColumnRef, TopShapeModelID, TopVariationModelID;
	HTuple  TopI, TopRow, TopColumn, TopAngle, TopScore, TopHomMat2D, Topdetectpath;
	HTuple  Topnumdetect, TopNumError;

	HTuple  Bottomnumofmodel, Bottomimagepath, Bottomirow, Bottomicol, Bottomiwidth;
	HTuple  Bottomiheight, BottomWidth, BottomHeight, BottomWindowHandle, BottomOS, BottomArea;
	HTuple  BottomRowRef, BottomColumnRef, BottomShapeModelID, BottomVariationModelID;
	HTuple  BottomI, BottomRow, BottomColumn, BottomAngle, BottomScore, BottomHomMat2D, Bottomdetectpath;
	HTuple  Bottomnumdetect, BottomNumError;
};


#endif