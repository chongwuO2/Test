// Splash.cpp : 实现文件
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



// CSplash 消息处理程序
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow(); 

	//CWnd::OnTimer(nIDEvent);
}
