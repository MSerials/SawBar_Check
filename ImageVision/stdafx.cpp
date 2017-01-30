// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.cpp : 只包括标准包含文件的源文件
// ImageVision.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


__global g;

#ifdef _HALCON
void CPPExpDefaultExceptionHandler(const Halcon::HException& except)
{
	throw except;
}
#endif

CString show_roi_msg(UINT id)
{
	switch (id)
	{
	case TROI_RECT:return L"请画顶部相机物料模板框";
	case TCHAR_RECT:return L"请画顶部相机字符模板框";
	case BROI_RECT:return L"请画底部相机物料模板框";
	case BCHAR_RECT:return L"请画底部相机字符模板框";
	default:return L""; break;
	}
}