// ImgPro.h: interface for the CImgPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPRO_H__E343E698_66B5_11D6_9386_00E04C395D54__INCLUDED_)
#define AFX_IMGPRO_H__E343E698_66B5_11D6_9386_00E04C395D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;


#define PI                                  3.141592653589793
#define GRIPER_SIZE                         8

enum SHOWWNDRECT{SHOWWNDRECT_NONE = 0 , SHOWWNDRECT_PATN = 1 , SHOWWNDRECT_SEARCH = 2};
enum LEARNINGMODE{IMAG_LEARN_ALL, IMAG_LEARN_SHIFT_INFORMATION, IMAG_LEARN_ROTATION_INFORMATION , IMAG_LEARN_MODE_SIZE_GUARD};
enum RAKE_DIRECTION{IMAG_LEFT_TO_RIGHT , IMAG_RIGHT_TO_LEFT , IMAG_TOP_TO_BOTTOM , IMAG_BOTTOM_TO_TOP , IMAG_RAKE_DIRECTION_SIZE_GUARD};
enum WEIGHT_FUNCTION{NONE_WEIGHT,HUBER_WEIGHT,TUKEY_WEIGHT,RANSAC_WEIGHT};
enum SPOKE_DIRECTION{IMAG_OUTSIDE_TO_INSIDE=0,IMAG_INSIDE_TO_OUTSIDE,IMAG_SPOKE_DIRECTION_SIZE_GUARD=0xFFFFFFFF};
enum MATCHINGMODE {IMAG_MATCH_SHIFT_INVARIANT = 1,IMAG_MATCH_ROTATION_INVARIANT = 2,IMAG_MATCHING_MODE_SIZE_GUARD = 0xFFFFFFFF};


typedef struct DOUBLE_LINE
{
	double x1;   
	double y1; 
	double x2;   
	double y2; 
}DoubleLine;

typedef struct DOUBLE_CIRCLE
{
	double x1;   
	double y1; 
	double r; 
}DoubleCircle;

typedef struct RESULT_POINT
{
	double x;   
	double y; 
	double angle; 
	double	rate;
	BOOL bstatus;
	DoubleLine line1;
	DoubleLine line2;
	DoubleLine line3;
	DoubleLine line4;
	DoubleCircle circle1;
	DoubleCircle circle2;
	DoubleCircle circle3;
	DoubleCircle circle4;
	double x2;   
	double y2; 
}PointResult;

//add by wei gy
typedef struct RESULT_RECT
{
	POINTFLOAT corner[4];
}RectResult;


typedef struct GRAVITY_STRUCTURE
{
	int m_bRectangle;   //true: Search rectangle gravity, false: Search circle gravity
	RECT rectTemp; //rectangle gravity window
	POINT center;  //circle gravity center
	int m_iRadius;//circle gravity radius
	int m_iThreshold;//threshold
	int m_iWhite;
}GravityStructure;

typedef struct MATCH_STRUCTURE
{
	int m_bRectangle;   //true: Search rectangle template, false: Search circle template
	int m_iMatchRate; //match rate
	RECT rectSearch;	//Search window		
	RECT rectTemp; //rectangle template window
	RECT rectSubTemp; //rectangle template window
	POINT center;  //circle template center
	int m_iRadius;//circle template radius
	int m_iThreshold;//threshold
}MatchStructure;

typedef struct DUAL_TEMP_PROPERTIES
{
	MatchStructure Temp1;
	MatchStructure Temp2;
}DualTempProperties;


typedef struct LINE_PROPERTIES
{
	int m_bHLine;  //true: Search horizontoal line, false:Search vertical line
	int m_bWtoB;   //true: Search color from white to black, false: Search color from black to white
	int m_bUDorLR; //true: Search direction from up to down(Horizontal line) or from left to right(vertical line)
	//false:Search direction from down to up(Horizontal line) or from right to left(vertical line)
	int m_iNoiseLv; //Noise level
	int m_iEdgeWidth;//Edge width
	int m_iEdgeLv;//Edge level
	RECT	rect;
}LineProperties;


typedef struct LINE_STRUCTURE
{
	int m_iLineNumber;
	MatchStructure MatchTemp;
	LineProperties SearchLine1;
	LineProperties SearchLine2;
	LineProperties SearchLine3;
	LineProperties SearchLine4;
}LineStructure;


typedef struct CIRCLE_PROPERTIES
{
	int m_bWtoB;   //true: Search color from white to black, false: Search color from black to white
	int m_bIntoOut; //true: Search direction from in to out,false:Search direction from out to in
	int m_iNoiseLv; //Noise level
	int m_iEdgeWidth;//Edge width
	int m_iEdgeLv;//Edge level
	CPoint center;
	int m_iOutR;
	int m_iInR;
}CircleProperties;

typedef struct CIRCLE_STRUCTURE
{
	int m_iCircleNumber;
	MatchStructure MatchTemp;
	CircleProperties SearchCircle1;
	CircleProperties SearchCircle2;
	CircleProperties SearchCircle3;
	CircleProperties SearchCircle4;
}CircleStructure;

typedef struct BLOB_STRUCTURE
{	
	RECT rectTemp; //rectangle blob window
	int m_iMinArea;//Min area blob
	int m_iThreshold;//threshold
	int m_iWhite;
}BlobStructure;

typedef struct RUN_COMMAND
{
	int m_iCommandNo;
	int m_iChannelNo;
	long m_iFunctionNo;
	GravityStructure gravity;//重心
	MatchStructure match;//模板
	LineStructure line;//线
	CircleStructure Circle;//圆
	BlobStructure Blob;//斑点
	DualTempProperties DualTemp;//双模板
}RUNCOMMAND;

typedef struct COMMAND_LIST
{
	RUNCOMMAND list0;
	RUNCOMMAND list1;
	RUNCOMMAND list2;
	RUNCOMMAND list3;
	RUNCOMMAND list4;
	RUNCOMMAND list5;
	RUNCOMMAND list6;
}COMMANDLIST;



struct Coord {
	int x;
	int y;
};

struct Element {
	vector<struct Element> neighbs;
	struct Coord coord;        
};

struct Blob {
	unsigned int elements_number;
	vector<struct Element> elements;
	unsigned int area;
	RECT bounding_box;      //[top,left; right,bottom)    
};

/*
class CParameter : public CObject  
{
	DECLARE_SERIAL(CParameter)
public:	
	void Serialize(CArchive& ar);
	COMMANDLIST list;
	CParameter();
	virtual ~CParameter();
public:
	void SetDefaultValue();
	void SaveSetting(CString szFilePath);
	void LoadSetting(CString szFilePath);
};
*/
typedef struct tagLineFittingOptions {
	float           tolerance;
    WEIGHT_FUNCTION weight;
	BOOL            bHorizontal;
}LineFittingOptions,*PLineFittingOptions;

typedef struct tagCircleFittingOptions {
	float           tolerance;
    WEIGHT_FUNCTION weight;
}CircleFittingOptions,*PCircleFittingOptions;

typedef struct tagFindEdgeOptions {
	int                threshold;
	int                width;
	int                steepness;
	int                subsamplingRatio;
	BOOL               showSearchArea;
	BOOL               showSearchLines;
	BOOL               showEdgesFound;
	BOOL               showResult;
}FindEdgeOptions,*PFindEdgeOptions;

typedef struct tagEdgePoint {
	POINTFLOAT point;
	float      distance;
	float      weight;
} EdgePoint,*PEdgePoint;

typedef struct tagStraightEdgeReport {
	POINTFLOAT start;
	POINTFLOAT end;
	float      meanDistance;
	float      alpha;
}StraightEdgeReport,*PStraightEdgeReport;

typedef struct tagCircularEdgeReport {
	POINTFLOAT center;
	float      radius;
	float      roundness;
	float      meanDistance;
}CircularEdgeReport,*PCircularEdgeReport;

typedef struct tagAnnulus{
	POINT  center;
	int    innerRadius;
	int    outerRadius;
	double startAngle;
	double endAngle;
}Annulus,*PAnnulus;

typedef struct tagLearnPatternOptions
{
	BOOL bLearnForRotation;//是否检测角度
	int initialStepSize;//起始步长
	int initialSampleSize;//起始采样大小
	int finalSampleSize;//最终采样大小
	int initialAngularAccuracy;//起始调整精度
    int	finalAngularAccuracy;//最终调整精度
	int subpixelSampleSize;//子像素采样大小
	int maxAngular;
	int minAngular;
}LearnPatternOptions,*PLearnPatternOptions;

typedef struct tagHaltonPoint {
    int x;
	int y;
	int nStableSize;
} HaltonPoint,*PHaltonPoint;

typedef struct tagHaltonSample {
    DWORD       hsSize;
	int         hsWidth;
	int         hsHeight;
	int         hsSamplePointCount;
	HaltonPoint hsHaltonPoints[1];
} HaltonSample,*PHaltonSample;

typedef struct tagRotationAngleRange {
	float lower;
	float upper;
} RotationAngleRange,*PRotationAngleRange;

typedef struct tagMatchPatternOptions {
	DWORD               size;
	MATCHINGMODE        mode;
	int                 minContrast;
	BOOL                subpixelAccuracy;
	RotationAngleRange  angleRanges;
	int                 numRanges;
	int                 numMatchesRequested;
	float               minMatchScore;
	int                 subpixelIterations;
	double              subpixelTolerance;
	int                 initialMatchListLength;
	int                 matchListReductionFactor;
	int                 initialStepSize;
	int                 intermediateAngularAccuracy;
} MatchPatternOptions,*PMatchPatternOptions;

typedef struct tagPatternMatch {
	POINTFLOAT position;
	float      rotation;
	float      scale;
	float      score;
	POINTFLOAT corner[4];
}PatternMatch,*PPatternMatch;

typedef struct tagCurveOptions {
	int  threshold;
	int  filterSize;
	int  minLength;
	int  maxLength;
	int  rowStepSize;
	int  columnStepSize;
	int  maxEndPointGap;
	BOOL  onlyClosed;
	BOOL subpixelAccuracy;
}CurveOptions,*PCurveOptions;

typedef CArray<FLOAT, FLOAT> CFloatArray;
typedef CArray<EdgePoint, EdgePoint&> CEdgePointsArray;
typedef vector<POINTFLOAT> Curve;
typedef vector<Curve> CurveArray;

/////////////////////////
typedef struct{
	int Height;	
	int Width;
	int Gray;
}Seed;

typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;

const double	Pi = 3.1415926535897932384626433832795;
const double	DEG_TO_RAD = Pi / 180;
#define RADIAN(angle) ((angle)*Pi/180.0);
#define BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x)));


class CTemplate  
{
public:
	BOOL imageNormalize(double* pVector,const int nCount , int* const pContrast , int* const pMean);
	void KillParent();
	BOOL Load(LPCTSTR lpszFileName);
	BOOL Save(LPCTSTR lpszFileName);
	double* GetExtractionBuffer();
	BOOL Create(BYTE *pParent, int nWidth, int nHeight, int nBitCount, int nRowLength , CRect rect, BOOL bInitialize);
	void OffsetSample(HaltonSample* const pHaltonSample , const int xOffset , const int yOffset);
	int GetAngularStepSize();
	BOOL DrawSample(CDC* pDC);
	BOOL Learn(const LearnPatternOptions& learnoption);
	int GetStepSize();

	HGLOBAL GetSubSample();
	BOOL SubSample(int nSampleCount);
	BOOL Rotate(const double alpha);
	void Destroy();
	BOOL Create(HBITMAP hParent,const CRect rect,BOOL bInitialize);
	CTemplate();
	virtual ~CTemplate();	

	int m_meanGrayScale;//平均灰度等级
	int m_maxContrast;//最大对比度
	int m_minContrast;//最小对比度

	int m_nSampleCount;//采样数据大小
	double* m_pSample;//采样数据值

	int m_nRowLength;//每行大小
	BYTE* m_pTmp;//模板图像数据
	int m_nHeight;
	int m_nWidth;
	HBITMAP m_hTmp;//模板图像

private:

	BOOL PrivateCreate(const CRect rect , BOOL bInitialize);
	void CopyParent(BYTE* pParent , int nWidth , int nHeight , int nBitCount  , int nRowLength);
	void CopyParent(HBITMAP hParentBitmap);
	int CalcStepSize(HaltonSample* pHaltonSample);
	void CalcStableSize(HaltonSample* const pHaltonSample , int nNoiseLevel);
	void OptimizeStepSize();

	int m_nHaltonSamplesCount;
	double* m_pExtraction;
	int m_nAngularStepSize;
	int m_upperMeanGrayScale;
	int m_lowerMeanGrayScale;
	int m_nStepSize;
	HGLOBAL m_hSubsample;
	CRect m_rectTmp;
	HBITMAP m_hParent;
};
BOOL SaveMatchPattern(LONG m_lHeight,LONG m_lWidth,LPBYTE m_lpBits,CRect rectTmp,int nCommand, CTemplate &m_objTemplate);
BOOL FindMatchPattern(LONG m_lHeight,LONG m_lWidth,LPBYTE m_lpBits,int nCommand, CRect SearchRect, MatchPatternOptions m_options,PatternMatch &m_PatternMatch);

class CImgPro  
{
public:
/*	Mat watershedSegment(Mat & image, int & noOfSegments);
	void mergeSegments(Mat & image, Mat & segments, int & numOfSegments);
	Mat createSegmentationDisplay(Mat & segments,int numOfSegments,Mat & image=Mat());
*/
	unsigned char ImprovedThreshold();
	unsigned char EqualityThreshold();
	unsigned char NormalThreshold();
	unsigned char EntropyThreshold();
	BOOL LaplacianThin(int n);
	BOOL SobelThin(int n);
	BOOL ImpPalking(int FuzzyNum);
	BOOL PalKing(float fd,unsigned char FuzzyNum);
	unsigned char OstuThreshold();
	BOOL LineMod();
	BOOL EdgeMod();
	BOOL ColOrth();
	BOOL ShenJun(float a);
	BOOL Laplace();
	unsigned char FuzzyThreshold(unsigned char iWin);
	BOOL Gauss();
	BOOL Kirsch();
	BOOL Prewitt();
	unsigned char IterativeThreshold();
	BOOL FaceFit();
	void Reserve();
	BOOL ThresholdA(unsigned char iThreshold,int &sumPixel);
	BOOL Threshold(unsigned char iThreshold,int &sumPixel);
	BOOL Threshold(unsigned char iThresholdLow,unsigned char iThresholdHi,unsigned char iThreshold,int &sumPixel);
	CImgPro(LPBYTE lpBits,LONG lWidth,LONG lHeight);
	BOOL Roberts();
	BOOL Template(LPSTR lpBits,int iTempH, int iTempW, int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef);
	BOOL Sobel();
	BOOL imageMatchPattern(const BYTE* pImage,
								   const int nImageWidth,
								   const int nImageHeight,
								   const int nImageRowLength,
								   CTemplate& tmpPattern,
								   const MatchPatternOptions& options,
								   const CRect& rectSearch,
								   PatternMatch* const pPatternMatch,
								   int* const numMatches);

	LONG m_lHeight;
	LONG m_lWidth;
	LPBYTE m_lpBits;
	LPBYTE m_lpBitsObjectflag;
	virtual ~CImgPro();

private:
	float Zadeh(unsigned char iGrayXY,unsigned char p,float fWin);

public:
	void DrawPoint(POINT pt,byte gray,int linelength);
	void SetPixelValue(BYTE* pData,int w,int h,POINT pt,byte gray);

	void reset()
	{
		if(m_lpBitsObjectflag!=NULL)
		{
			
			memset(m_lpBitsObjectflag,255,m_lWidth*m_lHeight);
		}
		else
		{
			m_lpBitsObjectflag=new BYTE[m_lWidth*m_lHeight];
			memset(m_lpBitsObjectflag,1,m_lWidth*m_lHeight);
		}
	}
};

class ImageBlobs
{
public:
        ImageBlobs();
        //ImageBlobs(const ImageBlobs& blobs);
        ~ImageBlobs();

// Operators
        //const ImageBlobs& operator=(const ImageBlobs& blobs);

// Operations
        void init(unsigned int width, unsigned int height);
        int find_blobs(LPBYTE image,int w,int h, unsigned int min_elements_per_blob = 0);
        void find_bounding_boxes();
        void delete_blobs();

// Access
// Inquiry
        inline unsigned int get_blobs_number() const;
        inline const struct Blob* get_blob(unsigned int i) const;
        //inline const vec2Dc* get_image() const;
		const LPBYTE get_image(int &w,int &h) const;

protected:
private:
        ImageBlobs(const ImageBlobs& blobs);
        const ImageBlobs& operator=(const ImageBlobs& blobs);
        

        //vec2Dc* m_image;   
		//image data info
		LONG m_lHeight;
		LONG m_lWidth;
		LPBYTE m_lpBits;

		//image with blobs 1, 2, ... N after find_blobs()        
        vector<struct Blob> m_blobs;


        void remove_blob_from_image(const struct Blob& blob);
        void mark_blobs_on_image();

        //if element has new_element as neighbour
        inline bool has_neighbour(const struct Element& element, const struct Element& new_element) const; 
        inline int is_element_present(const struct Blob& blob, const struct Element& new_element) const;
        inline unsigned int add_up_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_right_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_down_neighbour(struct Blob& blob, unsigned int i);
        inline unsigned int add_left_neighbour(struct Blob& blob, unsigned int i);
                       
};

#include "ImgPro.inl"
#include "Template.inl"
#endif // !defined(AFX_IMGPRO_H__E343E698_66B5_11D6_9386_00E04C395D54__INCLUDED_)
