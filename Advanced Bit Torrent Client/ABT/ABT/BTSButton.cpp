// BTSButton.cpp : implementation file
//

#include "stdafx.h"
#include "ABT.h"
#include "BTSButton.h"
#include "mmsystem.h"
#pragma comment(lib,"Winmm")

// CBTSButton

IMPLEMENT_DYNAMIC(CBTSButton,CButton)

CBTSButton::CBTSButton()
{
m_nTimerID=0;
m_bOverControl=false; 
init=false;

}
void CBTSButton::Id_Assigner(int nid)
{
niD=nid;
}
CBTSButton::~CBTSButton()
{
}


BEGIN_MESSAGE_MAP(CBTSButton, CButton)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CBTSButton message handlers



void CBTSButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
    CPoint p(GetMessagePos());
    ScreenToClient(&p);

    // Get the bounds of the control (just the client area)
    CRect rect;
    GetClientRect(rect);

    // Check if the mouse is inside the control
    if (!rect.PtInRect(p))
    {
        m_bOverControl = FALSE;
        KillTimer(m_nTimerID);
        Invalidate();
    }
	CButton::OnTimer(nIDEvent);
}

void CBTSButton::OnMouseMove(UINT nFlags, CPoint point)
{
	
if (!m_bOverControl)                  
    {
        m_bOverControl = TRUE;             
        Invalidate();                      
        SetTimer(m_nTimerID,100,NULL);    
    }
	CButton::OnMouseMove(nFlags, point);
}

void CBTSButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
if(!init)
{
	if(niD==IDC_DOWNLOAD)
{
norm.LoadBitmap(IDB_BITMAP2);
push.LoadBitmap(IDB_BITMAP2);
hover.LoadBitmap(IDB_BITMAP1);
}
	else if(niD==IDC_BUTTON1)
	{
	norm.LoadBitmap(IDB_BITMAP9);
    push.LoadBitmap(IDB_BITMAP9);
    hover.LoadBitmap(IDB_BITMAP8);
	}
	else if(niD==IDC_PAUSE)
	{
	norm.LoadBitmap(IDB_BITMAP5);
    push.LoadBitmap(IDB_BITMAP5);
    hover.LoadBitmap(IDB_BITMAP4);
	}
	else if(niD==IDC_RESUME)
	{
	norm.LoadBitmap(IDB_BITMAP7);
    push.LoadBitmap(IDB_BITMAP7);
    hover.LoadBitmap(IDB_BITMAP6);
	}
	else if(niD==IDC_CLOSE)
	{
	norm.LoadBitmap(IDB_BITMAP13);
    push.LoadBitmap(IDB_BITMAP13);
    hover.LoadBitmap(IDB_BITMAP12);
	}
	else if(niD==IDC_MIN)
	{
	norm.LoadBitmap(IDB_BITMAP10);
    push.LoadBitmap(IDB_BITMAP10);
    hover.LoadBitmap(IDB_BITMAP11);
	}
	init=true;
}
	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect =lpDrawItemStruct->rcItem;
    UINT state =lpDrawItemStruct->itemState;
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CPoint size(rect.Width(),rect.Height());
	CPoint org(0,0);
	if(state & ODS_SELECTED)
	{
		
		memDC.SelectObject(push);
        pDC->DPtoLP(&org);
	    pDC->BitBlt(0,0,size.x, size.y,&memDC, org.x, org.y,SRCCOPY);
	}
    else 
	{  	memDC.SelectObject(norm);
		pDC->DPtoLP(&org);
	    //pDC->BitBlt(-2,-2,size.x+2,size.y+4,&memDC, org.x, org.y,SRCAND|SRCPAINT);
		 pDC->BitBlt(0,0,size.x,size.y,&memDC, org.x, org.y,SRCCOPY);
		
	}
	if(m_bOverControl&&!(state&ODS_SELECTED))
	 {
	 
	   memDC.SelectObject(hover);
       pDC->DPtoLP(&org);
	   pDC->BitBlt(-2,-2,size.x+2, size.y+4,&memDC, org.x, org.y,SRCCOPY);
	}
   }

void CBTSButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CButton::PreSubclassWindow();
	ModifyStyle(0,BS_OWNERDRAW);
	
}
