#include "stdafx.h"
#include "GameMgr.h"
#include "time.h"

GameMgr::GameMgr(HWND hWnd)
{
	m_hWnd = hWnd;
}

GameMgr::~GameMgr()
{

}

void GameMgr::Init()
{
	Border* pBorder = new Border(50,20);
	Ball*   pBall = new Ball(pBorder);
	AddObject(pBorder);
	AddObject(pBall);
}

void GameMgr::AddObject(Object* pNewObject)
{
	m_listObjects.push_back(pNewObject);
}

void GameMgr::AllAct()
{
	for(std::list<Object*>::iterator i = m_listObjects.begin();
		i != m_listObjects.end();
		i++)
	{
		(*i)->Act();//数据计算
	}
}

void GameMgr::AllDisplay(HDC hDC)
{
	RECT client_rect;
	::GetClientRect(m_hWnd, &client_rect);

	int client_width = client_rect.right - client_rect.left;
	int client_height = client_rect.bottom - client_rect.top;

	HDC mem_dc = ::CreateCompatibleDC(hDC);
	HBITMAP bitmap = ::CreateCompatibleBitmap(hDC, client_width, client_width);

	HBITMAP old_bitmap = (HBITMAP)::SelectObject(mem_dc, bitmap);

	::FillRect(mem_dc, &client_rect, (HBRUSH)::GetStockObject(WHITE_BRUSH));

	for(std::list<Object*>::iterator i = m_listObjects.begin();
		i != m_listObjects.end();
		i++)
	{
		(*i)->Display(mem_dc);//执行绘图
	}

	::BitBlt(hDC, 0, 0, client_width, client_height,mem_dc, 0, 0, SRCCOPY);

	::SelectObject(mem_dc, old_bitmap);
	::DeleteObject(bitmap);
	::DeleteDC(mem_dc);
}

void GameMgr::Execute()
{
	//while(1)
	{
		//GetTickCout返回毫秒数
		//time()返回从1970年1月1日开始到现在的秒数

		int starttime = ::GetTickCount();	//一帧即时开始
		this->AllAct();							//逻辑帧

		HDC hDC = ::GetDC(m_hWnd);
		this->AllDisplay(hDC);					//渲染帧，windows gdi是没有释放cpu的,所以每帧的画图中没有睡眠机制
		::ReleaseDC(m_hWnd,hDC);

		//控制一帧的时长
		int endtime = ::GetTickCount();
		while(endtime - starttime < 10)
		{
			::Sleep(1);
			endtime = ::GetTickCount();
		}

	}
}

