
#include "stdafx.h"
#include "Ball.h"

Ball::Ball(Border* pBorder)
{
	m_pBorder = pBorder;
	m_xPos = BALL_START_POS_X;
	m_yPos = BALL_START_POS_Y;
	m_iRadius = BALL_RADIUS;
	m_xSpeed = BALL_INIT_SPEED_X;
	m_ySpeed = BALL_INIT_SPEED_Y;
}

Ball::~Ball()
{

}

bool Ball::Act()
{
	//碰撞检测
	if(m_xPos - m_iRadius <= m_pBorder->GetLeft())
	{
		//碰撞左边界的处理
		m_xSpeed = -m_xSpeed;
	}
	if(m_xPos + m_iRadius >= m_pBorder->GetRight())
	{
		//碰撞右边界的处理
		m_xSpeed = -m_xSpeed;
	}
	if(m_yPos - m_iRadius <= m_pBorder->GetTop())
	{
		//碰撞上边界的处理
		m_ySpeed = -m_ySpeed;
	}
	if(m_yPos + m_iRadius >= m_pBorder->GetBottom())
	{
		//碰撞下边界的处理
		m_ySpeed = -m_ySpeed;
	}
	m_xPos += m_xSpeed;
	m_yPos += m_ySpeed;
	return true;

}

bool Ball::Display(HDC hDC)
{
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,0));
	HGDIOBJ hOldbrush = ::SelectObject(hDC,hBrush);

	::Ellipse(hDC,m_xPos-m_iRadius,m_yPos-m_iRadius,m_xPos+m_iRadius,m_yPos+m_iRadius);
	::SelectObject(hDC,hOldbrush);
	::DeleteObject(hBrush);
	return true;
}
