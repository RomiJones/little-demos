#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "windows.h"

class Object
{
public:
	virtual bool Act() = 0;
	virtual bool Display(HDC hDC) = 0;
};

/*
class UnmovableObjct:public Object
{

};

class MovableObject:public Object
{

};
*/

#endif