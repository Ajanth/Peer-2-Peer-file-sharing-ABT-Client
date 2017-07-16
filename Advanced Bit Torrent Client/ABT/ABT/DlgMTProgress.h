#pragma once


// CDlgMTProgress dialog
#include "DlgMakeTorrent.h"
#include "TextProgressCtrl.h"

class CStrArray
{
public:
	CStrArray( CStringArray &strArray )
	{
		m_pStrArray = NULL;
		if ( 0 == strArray.GetSize() )
		{
			return;
		}

		m_nCount = strArray.GetSize();
		m_pStrArray = new LPTSTR[ m_nCount ];
		ASSERT( NULL != m_pStrArray );
		for ( register int i = 0; i < m_nCount; i++ )
		{
			m_pStrArray[ i ] = new TCHAR[ strArray[ i ].GetLength() + 1 ];
			strcpy_s( m_pStrArray[ i ],1, (LPCTSTR)strArray[ i ]);
		}
	};

	~CStrArray()
	{
		for ( register int i = 0; i < m_nCount; i++ )
		{
			delete [] m_pStrArray[ i ];
			m_pStrArray[ i ] = NULL;
		}

		delete [] m_pStrArray;
		m_pStrArray = NULL;
	};

	LPTSTR *GetArrayPtr()
	{
		return m_pStrArray;
	};

	int GetArraySize()
	{
		return m_nCount;
	};
	
protected:

	LPTSTR *m_pStrArray;
	int m_nCount;
};
class CDlgMTProgress : public CDialog
{
	public:
    CDlgMTProgress( CWnd* pParent = NULL );   // standard constructor
	CString server_msg;
	CString client_IP;
    // Dialog Data
    //{{AFX_DATA(CDlgMTProgress)
    enum { IDD = IDD_DLG_MTPOGRESS };
    CTextProgressCtrl	m_wndProgress;
    //}}AFX_DATA

	static HANDLE m_hThread;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMTProgress)
protected:
    virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMTProgress)
    virtual BOOL OnInitDialog();
    afx_msg void OnOk();
    afx_msg void OnCancel();
    afx_msg void OnTimer( UINT nIDEvent );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:

    static UINT WorkThreadProc( LPVOID lpParam );
    UINT WorkThreadProc();

    DWORD GetPieceSize( LPCTSTR lpszFilePath, int nSelIndex );

protected:

	int m_nProgress;
public:
	afx_msg void OnBnClickedOk();
	bool duplicate,copyright,upload_ok,success;
	void send_server(CString cs);
	DWORD x;
	int analyse_cpy();
	void send_myself(CString cx);
	CBitmap Bacground;//for holding the background image
	BITMAP bm;//for holding the background image
	CSize bitmapSize;//for holding the background image
	CString server_IP;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
UINT Receive_Client(LPVOID x);