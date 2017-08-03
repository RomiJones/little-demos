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
	int		m_xPos;			//���λ��x
	int		m_yPos;			//���λ��y
	int		m_iRadius;		//��İ뾶
	int		m_xSpeed;		//x���ϵ��ٶ�
	int		m_ySpeed;		//y���ϵ��ٶ�
	Border* m_pBorder;		//���ڵı߽�

public:
	Ball(Border* pBorder);
	virtual ~Ball();
	virtual bool Act();
	bool Display(HDC hDC);
};

#endif