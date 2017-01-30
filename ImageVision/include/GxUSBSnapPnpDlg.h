#pragma once

/// �豸����/�γ�״̬���¶���
typedef enum DEVICE_CHANGE_STATUS
{
	STATUS_STEADY              = -1,       ///< �ȶ�״̬:�γ��豸������豸
	STATUS_FIRST_ARRIVAL       = 1,        ///< ��ʶ�豸�״ν���
	STATUS_MID_REMOVE          = 2,        ///< ��ʶ�豸������̵��м�״̬
}DEVICE_CHANGE_STATUS;

//----------------------------------------------------------------------------------
/**
\Class  CUSBPort

\brief  ������:��ʶUSB�˿�,��¼/�����豸������״̬
*/
//----------------------------------------------------------------------------------
class CUSBPort
{
public:
	CUSBPort();
	~CUSBPort();
	DEVICE_CHANGE_STATUS  m_emStatus;    ///< USB�˿ڵ�ǰ�����豸���ڵ�״̬:�μ�DEVICE_CHANGE_STATUS����

	/// �޸ĵ�ǰ״̬
	void ChangeCurStatus(UINT message);

	/// ��ȡ��ǰ״̬
	DEVICE_CHANGE_STATUS GetCurStatus();
};




// CGxUSBSnapPnpDlg �Ի���

class CGxUSBSnapPnpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGxUSBSnapPnpDlg)

public:
	CGxUSBSnapPnpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGxUSBSnapPnpDlg();
	/// ��ʼ���б��ͷ
	void InitListHeader();

	/// ע������ͺţ���ָ�����ڷ��������Ϣ
	BOOL RegHVSnapPnpGUID(HANDLE handle, GUID stGuid);

	/// ��ȡ�豸��Ϣ���Ƚ��豸�γ�����״̬
	void UpdateDeviceList();

	/// �ж��Ƿ����豸�ѳɹ�����
	BOOL IsSnapPnpSuccess(CString str, UINT message);

	CUSBPort *GetUSBPortObject(CString strUSBPortName);
	
	std::map<CString, CUSBPort*>      m_mapUSBPortList;      ///< ������ʶ�𵽵�USB�˿���ϢList��
public:
// �Ի�������
	enum { IDD = IDD_GxUSBSnapPnp_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listResult;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
