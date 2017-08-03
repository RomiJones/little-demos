#ifndef __BORDER_H__
#define __BORDER_H__

#include "Object.h"
#include "windows.h"


#define DEF_BORDER_WIDHT	800
#define DEF_BORDER_HEIGTH	500

class Border:public Object
{
protected:
	RECT border_rect;

public:
	Border(int left = 0, int top = 0, int width = DEF_BORDER_WIDHT, int height = DEF_BORDER_HEIGTH);
	virtual ~Border();
	virtual bool Act();
	virtual bool Display(HDC hDC);
	int GetWidth();
	int GetHeight();
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();
};

#endif
