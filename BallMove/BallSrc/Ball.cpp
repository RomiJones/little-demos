
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
	//��ײ���
	if(m_xPos - m_iRadius <= m_pBorder->GetLeft())
	{
		//��ײ��߽�Ĵ���
		m_xSpeed = -m_xSpeed;
	}
	if(m_xPos + m_iRadius >= m_pBorder->GetRight())
	{
		//��ײ�ұ߽�Ĵ���
		m_xSpeed = -m_xSpeed;
	}
	if(m_yPos - m_iRadius <= m_pBorder->GetTop())
	{
		//��ײ�ϱ߽�Ĵ���
		m_ySpeed = -m_ySpeed;
	}
	if(m_yPos + m_iRadius >= m_pBorder->GetBottom())
	{
		//��ײ�±߽�Ĵ���
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
