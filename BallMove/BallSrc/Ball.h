#ifndef __BALL_H__
#define __BALL_H__

#include "Object.h"
#include "Border.h"


#define BALL_START_POS_X	100
#define BALL_START_POS_Y	100
#define BALL_RADIUS			10
#define BALL_INIT_SPEED_X	1
#define BALL_INIT_SPEED_Y	1

class Ball : public Object
{
protected:
	int		m_xPos;			//球的位置x
	int		m_yPos;			//球的位置y
	int		m_iRadius;		//球的半径
	int		m_xSpeed;		//x轴上的速度
	int		m_ySpeed;		//y轴上的速度
	Border* m_pBorder;		//所在的边界

public:
	Ball(Border* pBorder);
	virtual ~Ball();
	virtual bool Act();
	bool Display(HDC hDC);
};

#endif