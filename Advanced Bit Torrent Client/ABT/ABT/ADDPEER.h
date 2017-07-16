#pragma once


// CADDPEER dialog

class CADDPEER : public CDialogEx
{
	DECLARE_DYNAMIC(CADDPEER)

public:
	CADDPEER(CWnd* pParent = NULL);   // standard constructor
	virtual ~CADDPEER();

// Dialog Data
	enum { IDD = IDD_ADD_PEER };
	UINT	m_nPort;
	//}}AFX_DATA

    char m_szIPAddress[ 16 ];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedCancel3();
};
