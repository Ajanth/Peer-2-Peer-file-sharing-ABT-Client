#include "stdafx.h"
#include "SBDestination.h"


CSBDestination::CSBDestination(const HWND hParent, const int nTitleID)
	: CBrowseForFolder(hParent, NULL, nTitleID)
{

}

CSBDestination::~CSBDestination()
{

}

void CSBDestination::SetInitialSelection(const CString & strPath)
{
	m_strInitialSelection = strPath;
}

void CSBDestination::OnInit() const
{
	SetSelection(m_strInitialSelection);
	SetStatusText(m_strInitialSelection);
}

void CSBDestination::OnSelChanged(const LPITEMIDLIST pidl) const
{
	CString strBuffer;
	if (SHGetPathFromIDList(pidl, strBuffer.GetBuffer(MAX_PATH)))
		strBuffer.ReleaseBuffer();
	else
		strBuffer.ReleaseBuffer(0);
	SetStatusText(strBuffer);
}
