#ifndef __GAMEMGR_H__
#define __GAMEMGR_H__

#include "windows.h"
#include "Object.h"
#include "Border.h"
#include "Ball.h"
#include <list>

class GameMgr
{
protected:
	std::list<Object*> m_listObjects;
	HWND m_hWnd;

public:
	GameMgr(HWND hWnd);
	virtual ~GameMgr();
	void Init();
	void AddObject(Object* pNewObject);
	void AllDisplay(HDC hDC);
	void AllAct();

	void Execute();

};

#endif