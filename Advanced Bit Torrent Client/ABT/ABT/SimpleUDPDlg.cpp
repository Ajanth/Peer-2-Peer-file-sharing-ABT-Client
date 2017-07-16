// SimpleUDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "SimpleUDPDlg.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CSimpleUDPDlg, CDialog)

CSimpleUDPDlg::CSimpleUDPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleUDPDlg::IDD, pParent)
{

}

CSimpleUDPDlg::~CSimpleUDPDlg()
{
}

void CSimpleUDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}
BOOL CSimpleUDPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return 0;
}


BEGIN_MESSAGE_MAP(CSimpleUDPDlg, CDialog)
END_MESSAGE_MAP()


// CSimpleUDPDlg message handlers
