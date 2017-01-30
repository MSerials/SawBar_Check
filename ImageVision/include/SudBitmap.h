#pragma once

#define DEG2RAD (float)0.017453292519943295769236907684886	// PI*2/360
#define SQRT2 (float)1.4142135623730950488016887242097		// sqr(2)


// Result of the Hough transform
class Accumulator
{
public:
	Accumulator(const int imgWidth, const int imgHeight);	// constructor
	~Accumulator()											// destructor
	{
		delete [] bins;
	}

	// Increase votes for pixel x,y
	inline void Add(const int x, const int y)
	{	for (int theta = 0; theta < thetas; theta+=1)
	{
		int rho = (int)(x*coss[theta]) + (int)(y*sins[theta]) + rhoOffset;
		bins[rho*180 + theta]++;
	}
	}

	// Increase votes for pixel x,y with the angle limited (for efficiency) between theta1 and theta2
	inline void Add(const int x, const int y, const int theta1, const int theta2)
	{	for (int theta = theta1; theta <= theta2; theta+=1)
	{
		int rho = (int)(x*coss[theta]) + (int)(y*sins[theta]) + rhoOffset;
		bins[rho*180 + theta]++;
	}
	}

	int rhos;		// Accumulator height
	int thetas;		// Accumulator width
	int rhosLow;	// lower rhos(y axis) limit	because we ignore lines too close to ref.point
	int rhosHigh;	// upper rhos(y axis) limit because we ignore lines too far from ref.point
	int* bins;		// array of votes. Every point is a possible line. Greatest vote is the strongest line.

private:
	static const float sins[180];	// lookup table sin values
	static const float coss[180];	// lookup table cos values
	int rhoOffset;
};

// definition of line
struct Line
{
	int rho;		// distance from the point (0,0)
	int theta;		// angle
};

// single cell of the accumulator.
struct Bin
{
	int rho;
	int theta;
	int vote;
};

// configuration about what to show on the screen
struct DisplaySud
{
	BOOL black;
	BOOL white;
	BOOL rotation;
	BOOL whiteCandidates;
	BOOL whiteArea;
	BOOL lineDetect;
	BOOL grid;
	BOOL OCRresult;
	BOOL tempSolution;
	BOOL finalSolution;
	BOOL smallFont;
};

// 24-bit pixel
typedef struct tagPIX24
{
	BYTE    red;
	BYTE    green;
	BYTE    blue;
} PIX24, *LPPIX24;


class AFX_EXT_CLASS SudBitmap
{
public:
	SudBitmap(DWORD, LPVOID, CSize, const DisplaySud* display);
	~SudBitmap(void);

	void Monochrome(BYTE cc=2,int rectt=5);
	BOOL HoughTransformCenter(Bin* pStrongestLine);
	BOOL DetectRect(int theta);

private:
	LPVOID m_buf;		// original bitmap from webcam
	BYTE* m_pMono;		// monochromed bitmap
	int width;
	int height;
	int m_rhos;
	DWORD m_BitsResolution;

	POINT m_grid[10][10];

	void SumIntegralImage(unsigned int* pIntegral);
	BOOL HoughTransform(Line inputLine, int offset, int loLimit, int hiLimit, int thetaOffset, BOOL isVertical, Line* pStrongestLine);
	void DrawRect(const Line& left, const Line& top, const Line& right, const Line& bottom, const POINT& lt, const POINT& rt, const POINT& lb, const POINT& rb, COLORREF color);
	void SetPixel(int x, int y, COLORREF color);
	COLORREF GetPixel(int x, int y);
	BYTE GetPixelGray(int x, int y);
};
