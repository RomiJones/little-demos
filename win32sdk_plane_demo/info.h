#ifndef  _INFO_
#define  _INFO_

//敌人发出的是Cannon
//自己发出的是激光和导弹
class CStone//陨石
{
public:
	//static成员变量作为全部类对象的共有特征，
	//mfc运行时类型判断就是使用的是static成员变量
	static int ID;		//类型id
	static int level;		

	int id;				//物体id
	int x, y;			//??
	int x_offset;
	int y_offset;

	int height, width;		//方块的高度
	int x_speed, y_speed;

	int life;			//生命值
	int life_max;		//生命值上限
	int damage;			//对对手的伤害
	int score;			//被破坏了的得分
	int laser_ammo;		//对激光的防御
	int missile_ammo;	//对导弹的防御
	COLORREF color;		//颜色显示，也可以换成是图像

public:
	CStone();
	void Move();		//移动
	void ChangeColor();	//变色
	void SupplyAmmo(int&, int&);//???什么意思？？
};

class CEnemy//敌军
{
public:
	static int level;
	static int ID;

	int x, y;
	int life;
	static int life_max;
	int damage;
	int score;
	int laser_ammo;
	int missile_ammo;
	bool fire;
	COLORREF color;

	int id;
	int x_offset, y_offset;
	
	CEnemy();
	void Move();
	void ChangeColor();
	void Attack();				//多了一个攻击动作
	void SupplyAmmo(int&, int&);
	
protected:
	
	int height, width;	
	int x_speed, y_speed;
};

class CSpaceShip
{
public:
	int x, y;
	int level;
	int life;
	int life_max;
	int score;
	int wepon;
	int laser_ammo;
	int missile_ammo;
	bool fire;
	COLORREF color;
	int x_offset, y_offset;
	int next_score;
	
	CSpaceShip();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void Fire();
	void ChangeWepon();
	void LevelUp();

	void ChangeColor();

protected:
	
	int width, height;
	int x_speed, y_speed;
};

class CMissile
{
public:
	int x, y;
	int height, width;
	int x_offset, y_offset;
	int x_speed, y_speed;
	COLORREF color;
	static int damage;
	double TraceRate;
	int target_x;
	int target_y;
	int target_type;
	int target_ID;
	static int ID;
	int id;

public:
	CMissile(int, int);
	void ScanTarget();	//搜索目标
	void TraceTarget();	//追踪目标,也是移动
};

class CLaser
{
public:
	int x, y;
	int height, width;
	int y_speed;
	COLORREF color;
	static int damage;
	static int ID;
	int id;

	CLaser(int, int);
	void Move();
};

class CSupply
{
public:
	static int level;
	int x, y;
	int x_offset, y_offset;
	int x_speed, y_speed;
	COLORREF color;
	int life;
	int laser_ammo;
	int missile_ammo;
	static int ID;
	int id;

	CSupply();
	void Move();
	void GiveSupply(int, int&, int&, int&);//可以成为一个接口
};

class CCanon
{
public:
	int x, y;
	int height, width;
	int y_speed;
	COLORREF color;
	int damage;
	static int ID;
	int id;
	
	CCanon(int, int);
	void Move();
};

#endif