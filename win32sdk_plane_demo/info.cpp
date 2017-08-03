#include <vector>
#include <cstdio>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <cstdlib>
#include "info.h"
#include "math.h"

#define  BATTLE_OFFSET      200				//??
#define  BATTLE_WIDTH       400				//显示宽度
#define  BATTLE_HEIGHT      600				//显示高度
#define  BATTLE_LEFT		BATTLE_OFFSET				//显示左边界
#define  BATTLE_RIGHT		BATTLE_LEFT + BATTLE_WIDTH	//显示右边界

#define  LASER				1
#define  MISSILE			2
#define  TARGET_STONE       3
#define  TARGET_ENEMY       4
#define  NO_TARGET			5


using namespace std;

//各个陨石，敌军，供给类的level初始化
int CStone::level = 1;
int CEnemy::level = 1;
int CSupply::level = 1;

int CStone::ID			= 1;
int CSupply::ID			= 1;
int CEnemy::ID			= 1;
int CLaser::ID			= 1;
int CMissile::ID		= 1;
int CCanon::ID			= 1;
int CLaser::damage		= 50;
int CMissile::damage	= 200;
int CEnemy::life_max	= (int)200*(1+0.25*CEnemy::level);

//不在本模块中，不在本cpp文件中
extern	vector<CLaser>		LaserList;
extern	vector<CMissile>	MissileList;
extern	vector<CCanon>		CanonList;
extern	vector<CStone>		StoneList;
extern	vector<CEnemy>		EnemyList;
extern	vector<CSupply>		SupplyList;

CStone::CStone()
{
	id = CStone::ID;
	x = BATTLE_OFFSET + 100 + rand()%200;
	y = 0;
	color = RGB(64,128,128);
	x_speed = -(5 + CStone::level) + rand()%(10 + 2*CStone::level);
	y_speed = -rand()%(4 + 2*CStone::level);
	height = 40;
	width = 40;
	x_offset = 20;
	y_offset = 20;
	life_max = (int)200*(1+CStone::level*0.3);
	life = life_max;
	damage = 30;
	score = (int)20 * (1 + CStone::level*0.25);
	laser_ammo = 20;
	missile_ammo = 5;
	CStone::ID++;
}

void CStone::Move()
{

	if(y_speed<2)
		y_speed = 2;
	if(y > BATTLE_HEIGHT)
	{
		//如果出了底界，那么就要销毁自己
		for(vector<CStone>::iterator stone_itor = StoneList.begin();
			stone_itor!=StoneList.end(); 
			stone_itor++)
				if(stone_itor->id == this->id)
				{
					StoneList.erase(stone_itor);
					break;
				}
	}
	else
	{
		if(x > BATTLE_RIGHT || x < BATTLE_LEFT)
			x_speed = -x_speed;//碰见左右边界就要反弹
		x += x_speed;
		y += y_speed;
	}
}

void CStone::ChangeColor()
{
	int damage_rate = 10 - 10*life/life_max;
	if(damage_rate < 10)
	{
		color = RGB(64+19*damage_rate, 128 - 12*damage_rate, 128 - 12*damage_rate);
	}	
}

void CStone::SupplyAmmo(int& a, int& b)
{
	a += this->laser_ammo;
	b += this->missile_ammo;
}

CEnemy::CEnemy()
{
	
	life = CEnemy::life_max;
	damage = 15;
	score = (int)5*(1+0.2*CEnemy::level);
	laser_ammo = 5;
	missile_ammo = 0;
	fire = false;
	color = RGB(0,255,0);
	id = CEnemy::ID;
	
	x = BATTLE_OFFSET + rand()%BATTLE_WIDTH;
	y = 0;
	x_offset = 25;
	y_offset = 20;
	height = 2*x_offset;
	width = 2*y_offset;
	x_speed = -(8 + CEnemy::level) + rand()%(16 + 2*CEnemy::level);
	y_speed = rand()%(10 + 3*CEnemy::level);
	CEnemy::ID++;
}

void CEnemy::Move()
{
	if(y_speed<2)
		y_speed = 2;
	if(y > BATTLE_HEIGHT)
	{
		vector<CEnemy>::iterator enemy_itor = EnemyList.begin();
		for(;enemy_itor!=EnemyList.end(); enemy_itor++)
			if(enemy_itor->id == this->id)
				break;
			EnemyList.erase(enemy_itor);
	}
	else
	{
		if(x > BATTLE_RIGHT || x < BATTLE_LEFT)
			x_speed = -x_speed;

		x += x_speed;
		y += y_speed;
		
	}
}

void CEnemy::ChangeColor()
{
	int damage_rate = 10 - 10*life/life_max;
	if(damage_rate < 10)
	{
		color = RGB(25*damage_rate, 255-25*damage_rate, 0);
	}	
}

void CEnemy::SupplyAmmo(int& a, int& b)
{
	a += this->laser_ammo;
	b += this->missile_ammo;
}

void CEnemy::Attack()
{
	if(rand()%10000 > 200 + CEnemy::level*100)
		CanonList.push_back(CCanon(x,y));
}


CSpaceShip::CSpaceShip()
{
	level = 1;
	life_max = 200 + 50*level;
	life = life_max;
	score = 0;
	wepon = LASER;
	laser_ammo = 1000;
	missile_ammo = 500;
	fire = false;
	color = RGB(255,255,0);
	next_score = 200;

	x = BATTLE_LEFT + BATTLE_WIDTH/2;
	y = BATTLE_HEIGHT - 100;	
	x_offset = 30;
	y_offset = 20;
	width = 2*x_offset;
	height = 2*y_offset;
	x_speed = 20;
	y_speed = 20;
}

void CSpaceShip::MoveLeft()
{
	x -= x_speed;
	if(x < BATTLE_LEFT)
	{
		x = BATTLE_LEFT;
	}
}

void CSpaceShip::MoveRight()
{
	x += x_speed;
	if(x > BATTLE_RIGHT)
	{
		x = BATTLE_RIGHT;
	}
}

void CSpaceShip::MoveUp()
{
	y -= y_speed;
	if(y < 0)
	{
		y = 0;
	}
}

void CSpaceShip::MoveDown()
{
	y += y_speed;
	if(y > BATTLE_HEIGHT)
	{
		y = BATTLE_HEIGHT;
	}
}

void CSpaceShip::ChangeColor()
{	
	int damage_rate = 10 - 10*life/life_max;
	if(damage_rate < 10)
	{
		color = RGB(255, 255-25*damage_rate, 0);
	}			
	if(damage_rate == 10)
	{
		color = RGB(255,255,0);
	}
}

void CSpaceShip::ChangeWepon()
{
	wepon = (wepon == LASER) ? MISSILE:LASER;
}

void CSpaceShip::Fire()
{
	if(wepon == LASER)
	{
		if(laser_ammo > 0)
		{
			laser_ammo --;
			LaserList.push_back(CLaser(x,y));
		}

	}
	else if(wepon == MISSILE)
	{
		if(missile_ammo > 0)
		{
			missile_ammo --;
			MissileList.push_back(CMissile(x,y));
		}
		
	}
}


void CSpaceShip::LevelUp()
{
	if (score > next_score)
	{
		level++;
		next_score += (int)next_score * 1.5;
		life_max = life_max + 40;
		life = life_max;
		
		CStone::level++;
		CEnemy::level++;
		CSupply::level++;
	}
}

CLaser::CLaser(int ox, int oy)
{
	id = CLaser::ID;
	x = ox;
	y = oy;
	height = 30;
	width = 3;
	y_speed= -50;
	color = RGB(255,255,255);
//	damage = 50;
	
	CLaser::ID++;
}

void CLaser::Move()
{
	y+=y_speed;
	if(y < 0)
	{
		vector<CLaser>::iterator laser_itor = LaserList.begin();
		for(;laser_itor!=LaserList.end(); laser_itor++)
			if(laser_itor->id == this->id)
				break;
			LaserList.erase(laser_itor);
	}
}

CCanon::CCanon(int ox, int oy)
{
	id = CCanon::ID;
	x = ox;
	y = oy;
	height = 10, width = 4;
	y_speed = 10;
	color = RGB(255,255,0);
	damage = 45;

	CCanon::ID++;
}


void CCanon::Move()
{
	y+=y_speed;
	if(y < 0)
	{
		vector<CCanon>::iterator canon_itor = CanonList.begin();
		for(;canon_itor!=CanonList.end(); canon_itor++)
			if(canon_itor->id == this->id)
				break;
			CanonList.erase(canon_itor);
	}
}

CSupply::CSupply()
{
	id = CSupply::ID;
	x = BATTLE_OFFSET + 100 + rand()%200;
	y = 0;
	x_offset = 20;
	y_offset = 20;
	x_speed = -12 + rand()%24;
	y_speed = -rand()%12;
	color = RGB(255,255,255);
	life = 80;
	laser_ammo = 100;
	missile_ammo = 50;

	CSupply::ID++;
}

void CSupply::Move()
{
	if(y_speed<2)
		y_speed = 2;

	x += x_speed;
	y += y_speed;

	if(x < BATTLE_LEFT)
	{
		x = BATTLE_LEFT;
		x_speed = -x_speed;
	}
	if(x > BATTLE_RIGHT)
	{
		x = BATTLE_RIGHT;
		x_speed = -x_speed;
	}
	if(y > BATTLE_HEIGHT)
	{
		vector<CSupply>::iterator supply_itor = SupplyList.begin();
		for(;supply_itor!=SupplyList.end(); supply_itor++)
			if(supply_itor->id == this->id)
				break;
			SupplyList.erase(supply_itor);
	}
}

void CSupply::GiveSupply(int life_max, int& s_life, int& s_laser, int& s_missile)
{
	s_life += life;
	if(s_life > life_max)
		s_life = life_max;
	s_laser += laser_ammo;
	s_missile += missile_ammo;
}


CMissile::CMissile(int ox, int oy)
{
	id = CMissile::ID;
	x = ox;
	y = oy;
	x_offset = 10;
	y_offset = 10;
	height = 2 * y_offset;
	width =  2 * x_offset;
	x_speed = 0;
	y_speed = 8;
	color = RGB(0,0,255);
//	damage = 100;
	TraceRate = 0.5;
	target_x = ox-1;
	target_y = 0;
	target_type = NO_TARGET;
	target_ID = -1;
	
	CMissile::ID++;
}

void CMissile::ScanTarget()
{
	int dist = 0xffffff;
	int temp;

	for(int i = 0; i < StoneList.size(); i++)
	{
		temp = sqrt((double)((StoneList[i].x - x)*(StoneList[i].x - x) + (StoneList[i].y - y)*(StoneList[i].y - y)));
		if(temp < dist)
		{
			dist = temp;
			target_type = TARGET_STONE;
			target_ID = StoneList[i].id;
			target_x = StoneList[i].x;
			target_y = StoneList[i].y;
		}
	}
	for(int i = 0; i < EnemyList.size(); i++)
	{
		temp = sqrt((double)((EnemyList[i].x - x)*(EnemyList[i].x - x) + (EnemyList[i].y - y)*(EnemyList[i].y - y)));
		if(temp < dist)
		{
			dist = temp;
			target_type = TARGET_ENEMY;
			target_ID = EnemyList[i].id;
			target_x = EnemyList[i].x;
			target_y = EnemyList[i].y;
		}
	}
	if(target_ID == -1)
	{
		target_x = x;
		target_y = 0;
	}
	if(target_type != NO_TARGET && dist == 0xffffff)
	{
		target_x = x;
		target_y = y;
	}

}


void CMissile::TraceTarget()
{
	int x_signal = 0;
	int y_signal = 0;

	int speed_max;
	int x_dist = (int)(x-target_x)*TraceRate;
	int y_dist = (int)(y-target_y)*TraceRate;

	while(x_dist > 3 || y_dist > 3)
	{
		x_dist = x_dist * TraceRate;
		y_dist = y_dist * TraceRate;
	}

	if(x_speed > 0)
		x_signal = 1;
	else
		x_signal = -1;
	
	if(y_speed > 0)
		y_signal = 1;
	else
		y_signal = -1;

	x_speed -= x_dist;
	y_speed += y_dist;

	speed_max = (int)sqrt((double)(x_speed*x_speed + y_speed*y_speed));
	while(speed_max > 35)
	{
		x_speed = x_speed * 0.9;
		y_speed = y_speed * 0.9;
		if(x_speed == 0)
		{
			x_speed = x_signal;
		}
		if(y_speed == 0)
		{
			y_speed = y_signal;
		}
		speed_max = (int)sqrt((double)(x_speed*x_speed + y_speed*y_speed));
	}
	x += x_speed;
	y -= y_speed;


	if(x < BATTLE_LEFT - 50  || x > BATTLE_RIGHT + 50 || y < 0)
	{
		vector<CMissile>::iterator missile_itor = MissileList.begin();
		for(;missile_itor!=MissileList.end(); missile_itor++)
			if(missile_itor->id == this->id)
				break;
			MissileList.erase(missile_itor);
	}
}