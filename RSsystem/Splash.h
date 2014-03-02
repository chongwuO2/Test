#pragma once


// CSplash

class CSplash : public CWnd
{
	DECLARE_DYNAMIC(CSplash)

public:
	CSplash();
	virtual ~CSplash();

protected:
	DECLARE_MESSAGE_MAP()
public:  
    CBitmap m_bitmap;   
    void Create(UINT nBitmapID);  
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


