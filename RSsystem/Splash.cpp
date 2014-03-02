// Splash.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Splash.h"


// CSplash

IMPLEMENT_DYNAMIC(CSplash, CWnd)

CSplash::CSplash()
{

}

CSplash::~CSplash()
{
}


BEGIN_MESSAGE_MAP(CSplash, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CSplash ��Ϣ�������
void CSplash::Create(UINT nBitmapID)  
{  
    m_bitmap.LoadBitmap(nBitmapID);  
    BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    //CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
    CreateEx(0,  
            AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  
            NULL, WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL , NULL);  
}  



void CSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	 BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    CDC dcComp;  
    dcComp.CreateCompatibleDC(&dc);  
    dcComp.SelectObject(&m_bitmap);  
    // draw bitmap  
    dc.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcComp,0,0,SRCCOPY);  
}


void CSplash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow(); 

	//CWnd::OnTimer(nIDEvent);
}
