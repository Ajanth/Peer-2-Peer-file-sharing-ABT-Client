// Profile.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "Profile.h"
#include "afxdialogex.h"
#include "ABTUI.h"

CString ABTUI::fr_end;
// CProfile dialog

IMPLEMENT_DYNAMIC(CProfile, CDialogEx)

CProfile::CProfile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProfile::IDD, pParent)
{

}

CProfile::~CProfile()
{
}

void CProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}
BOOL CProfile::OnInitDialog()
{
	AfxMessageBox(usr_name);
	
this->SetDlgItemTextA(IDC_STATIC1,usr_name);
this->SetDlgItemTextA(IDC_STATIC2,rl_name);
this->SetDlgItemTextA(IDC_STATIC3,e_id);
this->SetDlgItemTextA(IDC_STATIC4,login_time);
this->SetDlgItemTextA(IDC_STATIC5,loc);
this->SetDlgItemTextA(IDC_STATIC6,tot_download);
return true;
}

BEGIN_MESSAGE_MAP(CProfile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProfile::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProfile::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProfile message handlers


void CProfile::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ABTUI::fr_end=usr_name;
	//if(parent== NULL)
		//AfxMessageBox("null");
	//parent->fr_final=usr_name;
	//parent->fri_ajanth.InsertItem(0,usr_name);
	//AfxMessageBox(parent->fr_final);
	//parent->friends_list.AddString(usr_name);
	CDialogEx::OnOK();
}


void CProfile::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
