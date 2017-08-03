#ifndef  _INFO_
#define  _INFO_

//���˷�������Cannon
//�Լ��������Ǽ���͵���
class CStone//��ʯ
{
public:
	//static��Ա������Ϊȫ�������Ĺ���������
	//mfc����ʱ�����жϾ���ʹ�õ���static��Ա����
	static int ID;		//����id
	static int level;		

	int id;				//����id
	int x, y;			//??
	int x_offset;
	int y_offset;

	int height, width;		//����ĸ߶�
	int x_speed, y_speed;

	int life;			//����ֵ
	int life_max;		//����ֵ����
	int damage;			//�Զ��ֵ��˺�
	int score;			//���ƻ��˵ĵ÷�
	int laser_ammo;		//�Լ���ķ���
	int missile_ammo;	//�Ե����ķ���
	COLORREF color;		//��ɫ��ʾ��Ҳ���Ի�����ͼ��

public:
	CStone();
	void Move();		//�ƶ�
	void ChangeColor();	//��ɫ
	void SupplyAmmo(int&, int&);//???ʲô��˼����
};

class CEnemy//�о�
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
	void Attack();				//����һ����������
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
	void ScanTarget();	//����Ŀ��
	void TraceTarget();	//׷��Ŀ��,Ҳ���ƶ�
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
	void GiveSupply(int, int&, int&, int&);//���Գ�Ϊһ���ӿ�
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