#pragma once
#include "afxwin.h"


// ProxyDummy dialog

class ProxyDummy : public CDialogEx
{
	DECLARE_DYNAMIC(ProxyDummy)

public:
	ProxyDummy(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProxyDummy();

// Dialog Data
	enum { IDD = IDD_PROXY_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void OnProxySettings();
	int	m_nProxyType;
    BOOL	m_bAuthRequired;
    CString	m_strProxyServer;
    UINT	m_nProxyPort;
    BOOL	m_bUseOnlyHTTP;
    CString	m_strUserName;
    CString	m_strPassword;
	CDialog proxy;
	afx_msg void OnBnClickedOk6();
};
