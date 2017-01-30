#pragma once

/// 设备接入/拔出状态更新定义
typedef enum DEVICE_CHANGE_STATUS
{
	STATUS_STEADY              = -1,       ///< 稳定状态:拔出设备或接入设备
	STATUS_FIRST_ARRIVAL       = 1,        ///< 标识设备首次接入
	STATUS_MID_REMOVE          = 2,        ///< 标识设备接入过程的中间状态
}DEVICE_CHANGE_STATUS;

//----------------------------------------------------------------------------------
/**
\Class  CUSBPort

\brief  辅助类:标识USB端口,记录/更新设备的连接状态
*/
//----------------------------------------------------------------------------------
class CUSBPort
{
public:
	CUSBPort();
	~CUSBPort();
	DEVICE_CHANGE_STATUS  m_emStatus;    ///< USB端口当前连接设备处于的状态:参见DEVICE_CHANGE_STATUS定义

	/// 修改当前状态
	void ChangeCurStatus(UINT message);

	/// 获取当前状态
	DEVICE_CHANGE_STATUS GetCurStatus();
};




// CGxUSBSnapPnpDlg 对话框

class CGxUSBSnapPnpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGxUSBSnapPnpDlg)

public:
	CGxUSBSnapPnpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGxUSBSnapPnpDlg();
	/// 初始化列表表头
	void InitListHeader();

	/// 注册相机型号，在指定窗口返回相关信息
	BOOL RegHVSnapPnpGUID(HANDLE handle, GUID stGuid);

	/// 获取设备信息，比较设备拔出插入状态
	void UpdateDeviceList();

	/// 判断是否有设备已成功接入
	BOOL IsSnapPnpSuccess(CString str, UINT message);

	CUSBPort *GetUSBPortObject(CString strUSBPortName);
	
	std::map<CString, CUSBPort*>      m_mapUSBPortList;      ///< 保存已识别到的USB端口信息List表
public:
// 对话框数据
	enum { IDD = IDD_GxUSBSnapPnp_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listResult;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
