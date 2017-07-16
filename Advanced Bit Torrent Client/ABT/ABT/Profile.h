#pragma once
//#include "ABTUI.h"
//#include "ABTUI.h"
// CProfile dialog

class CProfile : public CDialogEx
{
	DECLARE_DYNAMIC(CProfile)

public:
	CProfile(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProfile();
	CString usr_name;//parameters to display in profile view
	CString rl_name;
	CString e_id;
	CString login_time;
	CString loc;
	CString intrests;
	CString tot_upload;
	CString tot_download; 
// Dialog Data
	enum { IDD = IDD_PROFILE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	
public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
