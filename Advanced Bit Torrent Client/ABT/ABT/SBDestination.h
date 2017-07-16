#ifndef __SBDESTINATION_H__
#define __SBDESTINATION_H__
#pragma once
#include "browseforfolder.h"
class CSBDestination :
	public CBrowseForFolder
{
public:
	CSBDestination(const HWND hParent = NULL, const int nTitleID = 0);
	~CSBDestination();

	void SetInitialSelection(const CString& strPath);

	void OnInit() const;
	void OnSelChanged(const LPITEMIDLIST pidl) const;

private:
	CString m_strInitialSelection;
};
#endif
