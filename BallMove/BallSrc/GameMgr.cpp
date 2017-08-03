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
		(*i)->Act();//���ݼ���
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
		(*i)->Display(mem_dc);//ִ�л�ͼ
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
		//GetTickCout���غ�����
		//time()���ش�1970��1��1�տ�ʼ�����ڵ�����

		int starttime = ::GetTickCount();	//һ֡��ʱ��ʼ
		this->AllAct();							//�߼�֡

		HDC hDC = ::GetDC(m_hWnd);
		this->AllDisplay(hDC);					//��Ⱦ֡��windows gdi��û���ͷ�cpu��,����ÿ֡�Ļ�ͼ��û��˯�߻���
		::ReleaseDC(m_hWnd,hDC);

		//����һ֡��ʱ��
		int endtime = ::GetTickCount();
		while(endtime - starttime < 10)
		{
			::Sleep(1);
			endtime = ::GetTickCount();
		}

	}
}

