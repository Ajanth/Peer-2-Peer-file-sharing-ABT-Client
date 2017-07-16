#pragma once


// CVCheck dialog

class CVCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CVCheck)

public:
	CVCheck(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVCheck();
	CFont Font1;
	CString path;
	CString cmd;
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	CString ExecCmd(LPCSTR x);
// Dialog Data
	enum { IDD = IDD_VCHECK };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
