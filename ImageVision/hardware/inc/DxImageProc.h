//--------------------------------------------------------------- 
/** 
\file      DxImageProc.h
\brief     Image Processing Library
\version   v1.0.1305.9311 
\date      2013-5-31
\author    Software Department 
<p>Copyright (c) 2012-2013 China Daheng Group, Inc. Beijing Image 
Vision Technology Branch and all right reserved.</p> 
*/ 
//--------------------------------------------------------------- 

#if !defined (_DXIMAGEPROC_H)         
#define _DXIMAGEPROC_H 			///< pre-compiled macro define

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32                   
#include "stdafx.h"
#define DHDECL __stdcall

#else                          
#include <stdlib.h>
#include <string.h>
#define  DHDECL

typedef unsigned char   BYTE;     ///< 8λ�޷�������
typedef unsigned short  WORD;     ///< 16λ�޷�������
typedef unsigned long   DWORD;    ///< 32λ�޷�������
#endif

/// ���Ͷ���
typedef char            VxInt8;      ///< 8λ�з�������
typedef short           VxInt16;     ///< 16λ�з�������
typedef long            VxInt32;     ///< 32λ�з�������
typedef __int64         VxInt64;     ///< 64λ�з�������
typedef unsigned char   VxUint8;     ///< 8λ�޷�������
typedef unsigned short  VxUint16;    ///< 16λ�޷�������
typedef unsigned int    VxUint32;    ///< 32λ�޷�������


/// ״̬�붨��
typedef enum  tagDX_STATUS
{
	DX_OK                       = 0,  ///< �����ɹ�
	DX_PARAMETER_INVALID        = -1, ///< ���������Ч
	DX_PARAMETER_OUT_OF_BOUND   = -2, ///< ����Խ��
	DX_NOT_ENOUGH_SYSTEM_MEMORY = -3, ///< ϵͳ�ڴ治��
} DX_STATUS;

/// ����bayer��ʽ pixel color filter
typedef enum  tagDX_PIXEL_COLOR_FILTER
{
	_NONE    = 0,   ///< ��Bayer��ʽ
	BAYERRG = 1,   ///< ��һ����RG��ʼ
	BAYERGB = 2,   ///< ��һ����GB��ʼ
	BAYERGR = 3,   ///< ��һ����GR��ʼ
	BAYERBG = 4    ///< ��һ����BG��ʼ
} DX_PIXEL_COLOR_FILTER;

/// ����Bayerת������
typedef enum tagDX_BAYER_CONVERT_TYPE
{
	RAW2RGB_NEIGHBOUR = 0,   ///< ����ƽ����ֵ�㷨
	RAW2RGB_ADAPTIVE  = 1    ///< ��Ե����Ӧ��ֵ�㷨
} DX_BAYER_CONVERT_TYPE;

/// ��Ч����λ
typedef enum tagDX_VALID_BIT
{ 
	DX_BIT_0_7	    = 0,    ///< bit 0~7
	DX_BIT_1_8	    = 1,    ///< bit 1~8
	DX_BIT_2_9	    = 2,    ///< bit 2~9
	DX_BIT_3_10	    = 3,    ///< bit 3~10
	DX_BIT_4_11	    = 4     ///< bit 4~11
} DX_VALID_BIT;

/// ͼ��ʵ��λ��
typedef enum tagDX_ACTUAL_BITS
{
	DX_ACTUAL_BITS_10 = 10,    ///< 10λ
	DX_ACTUAL_BITS_12 = 12,    ///< 12λ 
	DX_ACTUAL_BITS_14 = 14,    ///< 14λ
	DX_ACTUAL_BITS_16 = 16     ///< 16λ
} DX_ACTUAL_BITS;

//--------------------------------------------------
/**
\brief  Raw8ͼ��ת����RGB24ͼ��
\param  pInputBuffer   	[in] ����ͼ�񻺳���
\param  pOutputBuffer   [out]���ͼ�񻺳�����new buffer��
\param  nWidth  	    [in] ͼ����
\param  nHeight   	    [in] ͼ��߶�
\param  cvtype          [in] ��ֵ�㷨����
\param  nBayerType      [in] bayer����
\param  bFlip           [in] ͼ��ת��� true:��ת false:����ת

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw8toRGB24(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_BAYER_CONVERT_TYPE cvtype, DX_PIXEL_COLOR_FILTER nBayerType, bool bFlip);

//--------------------------------------------------
/**
\brief  Convert Raw12Packed to Raw16
\param  pInputBuffer   	[in] ����ͼ�񻺳���
\param  pOutputBuffer   [out]���ͼ�񻺳�����new buffer��
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�                      

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw12PackedToRaw16(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//-------------------------------------------------
/**
\brief  Convert Raw10Packed to Raw16
\param  pInputBuffer   	[in] ����ͼ�񻺳���
\param  pOutputBuffer   [out]���ͼ�񻺳�����new buffer��
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�

\return  status
*/
//-------------------------------------------------
VxInt32 DHDECL DxRaw10PackedToRaw16(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//------------------------------------------------
/**
\brief  8λͼ��˳ʱ����ת90��
\param  pInputBuffer  	[in] ����ͼ�񻺳���
\param  pOutputBuffer	[out]���ͼ�񻺳�����new buffer��  
\param  nWidth        	[in] ͼ����
\param  nHeight       	[in] ͼ��߶�

\return  status
*/
//------------------------------------------------
VxInt32 DHDECL DxRotate90CW8B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//------------------------------------------------
/**
\brief  8λͼ����ʱ����ת90��
\param  pInputBuffer    [in] ����ͼ�񻺳���
\param  pOutputBuffer	[out]���ͼ�񻺳�����new buffer��  
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�

\return  status
*/
//------------------------------------------------
VxInt32 DHDECL DxRotate90CCW8B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//-----------------------------------------------
/**
\brief  ͼ�����ȵ���(24λRGBͼ�����8λ�Ҷ�ͼ��)
\param  pInputBuffer  	[in] ����ͼ�񻺳���      
\param  pOutputBuffer	[out]���ͼ�񻺳���  
\param  nImagesize    	[in] ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ(RGB:ͼ���xͼ���x3)
\param  nFactor        	[in] �������ӣ���Χ��-150~150

\return  status
*/
//-----------------------------------------------
VxInt32 DHDECL DxBrightness(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  ͼ��Աȶȵ���(24λRGBͼ�����8λ�Ҷ�ͼ��)
\param  pInputBuffer	[in] ����ͼ�񻺳���      
\param  pOutputBuffer	[out]���ͼ�񻺳���
\param  nImagesize      [in] ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ(RGB:ͼ���xͼ���x3)
\param  nFactor	        [in] �������ӣ���Χ��-50~100

\return  status
*/
//--------------------------------------------------
VxInt32 DHDECL DxContrast(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  24λRGBͼ����
\param  pInputBuffer  	[in] ����ͼ�񻺳���      
\param  pOutputBuffer 	[out]���ͼ�񻺳���
\param  nWidth        	[in] ͼ����
\param  nHeight       	[in] ͼ��߶�
\param  fFactor        	[in] �������ӣ���Χ��0.1~5.0

\return  status
*/
//--------------------------------------------------
VxInt32 DHDECL DxSharpen24B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, float fFactor);

//--------------------------------------------------
/**
\brief  24λRGBͼ�񱥺Ͷȵ��ں���
\param  pInputBuffer   	[in] ��������bufferָ��
\param  pOutputBuffer  	[out]�������bufferָ��
\param  nImageSize     	[in] RGBͼ��ߴ� (ͼ���xͼ���)
\param  nFactor        	[in] ���ڲ���,��Χ��0 ~ 128

\return  status   
*/
//--------------------------------------------------
VxInt32 DHDECL DxSaturation(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  ��ȡ��ƽ��ϵ��������Ϊ�˼���׼ȷ������տ͹ۡ��ס����򣬻��ߴ���ͼ��Ϊ����ɫ����
\param  pInputBuffer   	[in]��������bufferָ�루24bitRGBͼ��
\param  nWidth        	[in]ͼ����
\param  nHeight       	[in]ͼ��߶�
\param  dRatioR         [out]R������ƽ��ϵ��
\param  dRatioG         [out]G������ƽ��ϵ��
\param  dRatioB         [out]B������ƽ��ϵ��

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetWhiteBalanceRatio(void *pInputBuffer, VxUint32 nWidth, VxUint32 nHeight, double* dRatioR, double* dRatioG, double* dRatioB);

//-----------------------------------------------------
/**
\brief  Rawͼ���Զ�������У������,֧��8 ~ 16 λ���ݣ����λ��ָʵ��λ����������8λʱ������ռ�����ֽڣ�
        ��ʵ����Чλ�������� 9 ~ 16�����������ú���������ʱ���У����ÿ֡�������,����Packed��ʽ������ת����raw16
\param  pRawImgBuf      [in,out]Rawͼ����bufferָ��
\param  nWidth        	[in]ͼ����
\param  nHeight       	[in]ͼ��߶�
\param  nBitNum         [in]ͼ�����ݵ�ʵ��λ����ע������10λ����������10��12λ��������12���Դ����ƣ�ֵ��Χ8 ~ 16�����ο�˵����

\return  status  
*/
//-----------------------------------------------------
VxInt32 DHDECL DxAutoRawDefectivePixelCorrect(void* pRawImgBuf, VxUint32 nWidth, VxUint32 nHeight, VxInt32 nBitNum);

//--------------------------------------------------
/**
\brief  Raw16ͼ��(ʵ��λ��16λ��������ЧλΪ10λ��12λ)ת����Raw8ͼ��ʵ��λ��8λ��������Чλͬ��Ϊ8λ��
\param  pInputBuffer   	[in] ����ͼ�񻺳�������������СΪW*H*2�ֽڣ�
\param  pOutputBuffer   [out]���ͼ�񻺳�������������СΪW*H�ֽڣ�
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�
\param  nValidBits      [in] ��Чλ��

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw16toRaw8(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_VALID_BIT nValidBits);

//--------------------------------------------------
/**
\brief  RGB48ͼ��ת����RGB24ͼ��ÿ��RGB������ռ16λ��
\param  pInputBuffer   	[in] ����ͼ�񻺳�������������СΪW*H*3*2�ֽڣ�
\param  pOutputBuffer   [out]���ͼ�񻺳�������������СΪW*H*3�ֽڣ�
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�
\param  nValidBits      [in] ��Чλ��

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRGB48toRGB24(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_VALID_BIT nValidBits);

//--------------------------------------------------
/**
\brief  Raw16ͼ��(ÿ������ռ16λ)ת����RGB48ͼ��ÿ��RGB������ռ16λ��
\param  pInputBuffer   	[in] ����ͼ�񻺳�������������СΪW*H*2�ֽڣ�
\param  pOutputBuffer   [out]���ͼ�񻺳�������������СΪW*H*3*2�ֽڣ�
\param  nWidth          [in] ͼ����
\param  nHeight         [in] ͼ��߶�
\param  nActualBits     [in] ͼ��ʵ��λ��
\param  cvtype          [in] ��ֵ�㷨����
\param  nBayerType      [in] bayer����
\param  bFlip           [in] ͼ��ת��� true:��ת false:����ת

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw16toRGB48(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_ACTUAL_BITS nActualBits, DX_BAYER_CONVERT_TYPE cvtype, DX_PIXEL_COLOR_FILTER nBayerType, bool bFlip);

//--------------------------------------------------
/**
\brief  ����ԱȶȲ����֧��24λRGBͼ��
\param  nContrastParam  [in] �Աȶȵ��ڲ�������Χ��-50~100
\param  pContrastLut    [out]�ԱȶȲ��
\param  pLutLength      [out]��ȡ�Ĳ��ұ��ȣ���λ���ֽڣ�

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetContrastLut(int nContrastParam, void *pContrastLut, int *pLutLength);

//--------------------------------------------------
/**
\brief  ����Gamma�����֧��24λRGBͼ��
\param  dGammaParam   [in] Gamma���ڲ�������Χ��(0.1 ~ 10) 
\param  pGammaLut     [out]Gamma���ұ�
\param  pLutLength    [out]��ȡ�Ĳ��ұ��ȣ���λ���ֽڣ�

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetGammatLut(double dGammaParam, void *pGammaLut, int *pLutLength);

//--------------------------------------------------
/**
\brief  ͼ������������������֧��24λRGBͼ��
\param  pInputBuffer   	      [in] ����ͼ�񻺳���
\param  pOutputBuffer         [out]���ͼ�񻺳���
\param  nWidth                [in] ͼ����
\param  nHeight               [in] ͼ��߶�
\param  nColorCorrectionParam [in] ��ɫУ������
\param  pContrastLut          [in] �ԱȶȲ��ұ�
\param  pGammaLut             [in] Gamma���ұ�

\return  status  
*/
//--------------------------------------------------
VxInt32 DHDECL DxImageImprovment(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, VxInt64 nColorCorrectionParam, void *pContrastLut, void *pGammaLut);


#ifdef __cplusplus
}
#endif

#endif




