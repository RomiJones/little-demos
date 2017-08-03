#include "stdafx.h"
#include "Border.h"
Border::Border(int left, int top, int width, int height)
{
	border_rect.left = left;
	border_rect.top = top;
	border_rect.right = left + width;
	border_rect.bottom = top + height;
}

Border:: ~Border()
{

}

bool Border::Act()
{
	//边界是不用动的
	return true;
}

bool Border::Display(HDC hDC)
{
	::Rectangle(hDC, border_rect.left, border_rect.top, border_rect.right, border_rect.bottom);
	return true;
}

int Border::GetWidth()
{
	return border_rect.right - border_rect.left;
}

int Border::GetHeight()
{
	return border_rect.bottom - border_rect.top;
}

int Border::GetLeft()
{
	return border_rect.left;
}

int Border::GetRight()
{
	return border_rect.right;
}

int Border::GetTop()
{
	return border_rect.top;
}

int Border::GetBottom()
{
	return border_rect.bottom;
}
