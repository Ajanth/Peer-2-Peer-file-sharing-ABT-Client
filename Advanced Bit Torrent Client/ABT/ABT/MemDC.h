#if !defined(AFX_MEMDC_H__19EE512C_AC28_4C2C_B716_C3ADA625E781__INCLUDED_)
#define AFX_MEMDC_H__19EE512C_AC28_4C2C_B716_C3ADA625E781__INCLUDED_

#pragma once

namespace foo {
class CMemDCM :
	public CDC
{
	public:
    DECLARE_DYNAMIC( CMemDCM );

    // constructor sets up the memory DC
    CMemDCM( CDC* pDC = NULL );

    void AttachDC( CDC *pDC, CRect *pRect = NULL );

    void DettachDC();

    // Destructor copies the contents of the mem DC to the original DC
    virtual ~CMemDCM();

    // Allow usage as a pointer
    CMemDCM* operator->()
    {
        return this;
    }

    // Allow usage as a pointer
    operator CMemDCM*()
    {
        return this;
    }

private:
    CBitmap m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
    CDC* m_pDC;         // Saves CDC passed in constructor
    CRect m_rect;        // Rectangle of drawing area.
    BOOL m_bMemDC;      // TRUE if CDC really is a Memory DC.
    BOOL m_bValid;

};
}
#endif 