#pragma once
#include "afxwin.h"


// CSimpleUDPDlg dialog

class CSimpleUDPDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimpleUDPDlg)

public:
	CSimpleUDPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSimpleUDPDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimpleUDPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
};
