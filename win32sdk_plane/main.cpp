#define  WIN32_LEAN_AND_MEAN

#define  INITGUID

#include <vector>
#include <cstdio>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <cstdlib>
#include "info.h"

using namespace std;

#define  WINDOW_CLASS_NAME  "Fly_Game"

#define  WINDOW_WIDTH       800
#define  WINDOW_HEIGHT		600

#define  ERASE              1
#define  NO_ERASE           0

//ID���
#define  IS_CANON           0
#define  IS_LASER           1<<1
#define  IS_MISSILE         1<<2
#define  IS_STONE           1<<3
#define  IS_ENEMY           1<<4
#define  IS_SPACESHIP       1<<5
#define  IS_SUPPLY          1<<6

#define  LASER				1
#define  MISSILE			2

//����߿�
#define  BATTLE_OFFSET      200
#define  BATTLE_WIDTH       400
#define  BATTLE_HEIGHT      600
#define  BATTLE_LEFT		BATTLE_OFFSET
#define  BATTLE_RIGHT		BATTLE_LEFT + BATTLE_WIDTH

#define  KEYDOWN(vk_code)  (GetAsyncKeyState(vk_code) & 0x8000 ? 1:0)
#define  KEYUP(vk_code)    (GetAsyncKeyState(vk_code) & 0x8000 ? 0:1)

#pragma comment(lib,"winmm.lib")

//���������洦ʹ�õ�ȫ�ֱ���
HWND        main_window_handle = NULL;		//�����ھ��
HINSTANCE   main_window_hinstance = NULL;	//Ӧ�ó���ʵ�����
HDC         global_hdc;						//ȫ�ֵĻ�ͼdc
//int         window_closed = 0;

//���ֶ���������
CSpaceShip			ship;		//���Ƿɴ�
vector<CEnemy>		EnemyList;	//ȫ���ĵо�
vector<CStone>		StoneList;	//ȫ������ʯ
vector<CSupply>		SupplyList;	//ȫ���Ĺ���
vector<CLaser>		LaserList;	//ȫ���ļ����ڵ�
vector<CMissile>	MissileList;//ȫ���ĵ���
vector<CCanon>		CanonList;	//ȫ���ļ�ũ���ڵ�

int Enemy_Kill = 0;	//������

//�����ַ���ʾ
char   buffer1[]  = " 'A'�������ƶ� ";
char   buffer2[]  = " 'S'�������ƶ� ";
char   buffer3[]  = " 'D'�������ƶ� ";
char   buffer10[] = " 'W'�������ƶ� ";
char   buffer4[]  = " 'J'������";
char   buffer5[]  = " 'K'���������� ";
char   buffer6[80];
char   buffer7[80];
char   buffer8[80];
char   buffer9[80];
char   buffer11[80];
char   buffer12[80];
char   buffer13[80];
char   buffer14[80];
char   buffer15[80];
char   buffer16[80];
char   buffer17[10];

int Game_Init();
int Game_Main();
int Game_Shutdown();
int Draw_Sprite(int flag);
int Check_Hit(int flag);	//��ײ���
void Game_Over();

LRESULT CALLBACK WindowProc(HWND hwnd, 
							UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	
	PAINTSTRUCT		ps;		
	HDC				win_hdc;	       
	
	switch(msg)
	{	
	case WM_CREATE: 
        {
			return(0);
		} break;
		
	case WM_PAINT: 
		{	
			win_hdc = BeginPaint(hwnd,&ps);
			EndPaint(hwnd,&ps);
			return(0);
		} break;
		
	case WM_CLOSE:
		{
			if(MessageBox(main_window_handle,
							"Are you sure you want to quit?",
							"Warning",
							MB_YESNO)==IDYES)
				return (DefWindowProc(hwnd, msg, wparam, lparam));
			else
				return(0); 
        }
		break;
	case WM_DESTROY: 
		{	
			PostQuitMessage(0);	
			return(0);
		} break;
	default:break;
    } 
	return (DefWindowProc(hwnd, msg, wparam, lparam));
} 

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
                    PSTR szCmdLine,
					int iCmdShow)
{
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WindowProc;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = WINDOW_CLASS_NAME ;
	
	main_window_hinstance = hInstance;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, 
					TEXT("This program requires Windows NT!"), 
					WINDOW_CLASS_NAME,
					MB_ICONERROR) ;
		return 0 ;
	}
	
	//������һ����һ���ڵ�Ӧ�ó���
	if(!(hwnd = CreateWindow (WINDOW_CLASS_NAME,
								"FlyGame_DEMO",
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								WINDOW_WIDTH,
								WINDOW_HEIGHT,
								NULL, NULL, hInstance, NULL)))
		return 0;
	
	main_window_handle = hwnd;//ȫ�ֱ������������ھ��

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd) ;
	
	Game_Init();//��Ϸ��ʼ��

	while(TRUE)
	{
		//�ȴ���app����Ϣ��Ӧ
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
        Game_Main();	
	}

	Game_Shutdown();//app�˳���Ϣѭ�����������,������Դ���ջ�...

	return msg.wParam ;
}

int Game_Init()
{
	srand(time(NULL));//�����������
	global_hdc = GetDC(main_window_handle);	//��ȡ������dc�ľ��
	SetTextColor(global_hdc,RGB(0,255,0));	//������ʾ���ı�������ɫ��
	SetBkMode(global_hdc,TRANSPARENT);		//͸��
	return 0;
}

int Game_Main()
{
	SetBkMode(global_hdc, OPAQUE);//��͸��
	strcpy(buffer13,"��ǰ��������:");

	//����ֻ��������������ǹ���ߵ���
	if (ship.wepon == LASER)//��ǹ
	{
		strcpy(buffer17," ���� ");
	}
	else
	{
		strcpy(buffer17," ���� ");
	}

	strcat(buffer13,buffer17);
	sprintf(buffer12,"�ɴ��ȼ�: %d ",ship.level);
	sprintf(buffer6,"�÷�: %d  ", ship.score);
	sprintf(buffer7,"����ֵ: %d  ", ship.life);
	sprintf(buffer8,"���ⵯҩ��: %d  ", ship.laser_ammo);
	sprintf(buffer9,"��������: %d  ", ship.missile_ammo);
	sprintf(buffer11,"�ݻٵл�����: %d ",Enemy_Kill);
	sprintf(buffer14,"�����˺�ֵ: %d ", CLaser::damage);
	sprintf(buffer15,"�����˺�ֵ: %d ", CMissile::damage);
	sprintf(buffer16,"�л�����ֵ: %d ", CEnemy::life_max);
	
	//ʹ��ȫ��dc�����ʾ�ַ���Ϣ
	TextOut(global_hdc,2,55,	buffer1,strlen(buffer1));
	TextOut(global_hdc,2,80,	buffer2,strlen(buffer2));
	TextOut(global_hdc,2,105,	buffer3,strlen(buffer3));
	TextOut(global_hdc,2,130,	buffer10,strlen(buffer10));
	TextOut(global_hdc,2,155,	buffer4,strlen(buffer4));
	TextOut(global_hdc,2,180,	buffer5,strlen(buffer5));

	TextOut(global_hdc,640,55, buffer12,strlen(buffer12));
	TextOut(global_hdc,640,80, buffer6,strlen(buffer6));
	TextOut(global_hdc,640,105,buffer7,strlen(buffer7));
	TextOut(global_hdc,640,130,buffer8,strlen(buffer8));
	TextOut(global_hdc,640,155,buffer9,strlen(buffer9));
	TextOut(global_hdc,640,180,buffer11,strlen(buffer11));
	TextOut(global_hdc,640,205,buffer13,strlen(buffer13));
	TextOut(global_hdc,640,230,buffer14,strlen(buffer14));
	TextOut(global_hdc,640,255,buffer15,strlen(buffer15));
	TextOut(global_hdc,640,280,buffer16,strlen(buffer16));

	//��ͼ
	Draw_Sprite(NO_ERASE);//�������
	Sleep(40);
	SetBkMode(global_hdc, TRANSPARENT);
	SetBkColor(global_hdc, RGB(0,0,0));
	Draw_Sprite(ERASE);
//	Sleep(15);


	//��ȡ����
	if(KEYDOWN('A'))
		ship.MoveLeft();
	if(KEYDOWN('S'))
		ship.MoveDown();
	if(KEYDOWN('D'))
		ship.MoveRight();
	if(KEYDOWN('W'))
		ship.MoveUp();
	if(KEYDOWN('J'))
	{
		ship.Fire();
		if(ship.wepon == LASER)
			PlaySound("Fire.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
		else
			PlaySound("rocket.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
	}
	if(KEYDOWN('K'))
		ship.ChangeWepon();

	//vector���Ƕ�����
	//һ��������������
	if(rand()%1000 < 30)
	{
		EnemyList.push_back(CEnemy());
	}
	//һ��������������
	if(rand()%2000 < 5)
	{
		SupplyList.push_back(CSupply());
	}

	///////////Supply ////////////////////////////////////////
	//ÿ��������Ҫ�ƶ�
	for(int i = 0; i < SupplyList.size(); i++)
	{
		SupplyList[i].Move();
	}
	///////////Stone///////////////////////////////////////////
	//ÿ����ʯ��Ҫ�ƶ�
	for(int i = 0; i < StoneList.size(); i++)
	{
		StoneList[i].Move();
	}
	
	/////////////Enemy/////////////////////////////////////////
	//ÿ�����˶�Ҫ�ƶ�
	for(int i = 0; i < EnemyList.size(); i++)
	{
		EnemyList[i].Move();
		if(rand()%100 < 5)//ÿ������һ�����ʵĹ���
			EnemyList[i].Attack();
	}
	
	////////////Canon ///////////////////////////////////////
	//��ũ�ڵ��ƶ�
	for(int i = 0; i < CanonList.size(); i++)
	{
		CanonList[i].Move();		
	}
	
	//////////////////Laser //////////////////////////////////
	//�����ƶ�
	for(int i = 0; i < LaserList.size(); i++)
	{
		LaserList[i].Move();		
	}
	///////////////////Missile ///////////////////////////
	//�����ƶ�
	for(int i = 0; i < MissileList.size(); i++)
	{
		MissileList[i].ScanTarget();
		MissileList[i].TraceTarget();		
	}

	//��ײ���
	Check_Hit(IS_CANON);	//����ũ�ڵ���ײ
	Check_Hit(IS_LASER);	//��鼤���ӵ���ײ
	Check_Hit(IS_MISSILE);	//��鵼����ײ
	Check_Hit(IS_SUPPLY);	//��鹩����ײ

	ship.LevelUp();			//�ж�����
	
	return 0;
}

int Game_Shutdown()
{
	ReleaseDC(main_window_handle, global_hdc);//�ͷ�������dc�ľ��
	return 0;
}

int Draw_Sprite(int flag)
{
	HDC hdc;
	hdc = GetDC(main_window_handle);//�������λ�ȡͬһ�����ڵ�dc��?

	HBRUSH brush;
	RECT rect;
	if (flag == ERASE)
	{	
		brush = CreateSolidBrush(RGB(0,0,0));
		SetRect(&rect, 
				BATTLE_LEFT - 35, 
				0, 
				BATTLE_RIGHT + 35, 
				WINDOW_HEIGHT - 1);
		FillRect(hdc, &rect, brush);	//��Ļˢ��
		DeleteObject(brush);			//

		//Ϊʲô��Ҫ�ѵ�����ˢ�ڵ����ѵ���Ļˢ�ڵ�ʱ��û�ж�ˢ�ڵ���
		for(int i=0; i<MissileList.size();i++)
		{
			SetRect(&rect,
					MissileList[i].x, 
					MissileList[i].y,
					MissileList[i].x+MissileList[i].width,
					MissileList[i].y+MissileList[i].height);	
			brush = CreateSolidBrush(RGB(0,0,0));
			
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
	}
	else
	{
		//����ɫ�����ұ߽�
		brush = CreateSolidBrush(RGB(0,128,192));//����GDI����
		SetRect(&rect, 
				BATTLE_LEFT - 22, 
				1, 
				BATTLE_LEFT - 20, 
				BATTLE_HEIGHT - 1);
		FillRect(hdc, &rect, brush);
		SetRect(&rect, 
				BATTLE_RIGHT + 20, 
				1, 
				BATTLE_RIGHT + 22, 
				BATTLE_HEIGHT - 1);
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);//ɾ��GDI����
		
		////////////// SpaceShip//////////////////////////////////
		SetRect(&rect,
				ship.x-ship.x_offset,
				ship.y-ship.y_offset,
				ship.x+ship.x_offset,
				ship.y+ship.y_offset);
		brush = CreateSolidBrush(ship.color);//���ɴ�
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		
		/////////////Stone////////////////////////////////////////
		for(int i = 0; i < StoneList.size(); i++)
		{
			SetRect(&rect,
					StoneList[i].x-StoneList[i].x_offset,
					StoneList[i].y-StoneList[i].y_offset,
					StoneList[i].x+StoneList[i].x_offset,
					StoneList[i].y+StoneList[i].y_offset);
			
			brush = CreateSolidBrush(StoneList[i].color);
			
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
		
		/////////////Enemy/////////////////////////////////////////
		for(int i = 0; i < EnemyList.size(); i++)
		{
			SetRect(&rect,
				EnemyList[i].x-EnemyList[i].x_offset,
				EnemyList[i].y-EnemyList[i].y_offset,
				EnemyList[i].x+EnemyList[i].x_offset,
				EnemyList[i].y+EnemyList[i].y_offset);
			brush = CreateSolidBrush(EnemyList[i].color);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
		
		///////////Supply ////////////////////////////////////////
		for(int i = 0; i < SupplyList.size(); i++)
		{
			SetRect(&rect,
				SupplyList[i].x-SupplyList[i].x_offset,
				SupplyList[i].y-SupplyList[i].y_offset,
				SupplyList[i].x+SupplyList[i].x_offset,
				SupplyList[i].y+SupplyList[i].y_offset);
			brush = CreateSolidBrush(SupplyList[i].color);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
		
		////////////Canon ///////////////////////////////////////
		for(int i = 0; i < CanonList.size(); i++)
		{
			SetRect(&rect,
				CanonList[i].x, 
				CanonList[i].y,
				CanonList[i].x+CanonList[i].width,
				CanonList[i].y+CanonList[i].height);
			brush = CreateSolidBrush(CanonList[i].color);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
		
		//////////////////Laser //////////////////////////////////
		for(int i = 0; i < LaserList.size(); i++)
		{
			SetRect(&rect,
				LaserList[i].x, 
				LaserList[i].y,
				LaserList[i].x+LaserList[i].width,
				LaserList[i].y+LaserList[i].height);
			brush = CreateSolidBrush(LaserList[i].color);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
		
		///////////////////Missile ///////////////////////////
		for(int i = 0; i < MissileList.size(); i++)
		{
			SetRect(&rect,
				MissileList[i].x, 
				MissileList[i].y,
				MissileList[i].x+MissileList[i].width,
				MissileList[i].y+MissileList[i].height);

			brush = CreateSolidBrush(MissileList[i].color);
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
	}

//	DeleteObject(brush);
	ReleaseDC(main_window_handle,hdc);//�ͷ�������dc
	return 0;
}

int Check_Hit(int flag)
{
	int diff;	//�����������ĵ�֮��ľ���
	int hit;	//��������֮����ײ�ľ��룬diff���ڴ�ֵ����ײ��diffС�ڴ�ֵ�ͷ�����ײ

	if(flag == IS_MISSILE)//�жϵ�������˭
	{
		for(vector<CMissile>::iterator Missile_iter = MissileList.begin();
			Missile_iter != MissileList.end();)
			{
				for(vector<CEnemy>::iterator Enemy_iter = EnemyList.begin();
					Enemy_iter != EnemyList.end();)
					{
						diff = (int)sqrt((double)((Missile_iter->x-Enemy_iter->x)*(Missile_iter->x-Enemy_iter->x)+
												  (Missile_iter->y-Enemy_iter->y)*(Missile_iter->y-Enemy_iter->y)));
						hit = Missile_iter->x_offset + Enemy_iter->x_offset;
						if(diff<hit)//������
						{
							Enemy_iter->life-= Missile_iter->damage;//��Ѫ
							if(Enemy_iter->life>0)	//������û��
								Enemy_iter->ChangeColor();
							else					//�����˶�������
							{
								ship.score+=Enemy_iter->score;				//��ȡ�÷�
								ship.laser_ammo+=Enemy_iter->laser_ammo;		//��ȡ���ⵯҩ
								ship.missile_ammo+=Enemy_iter->missile_ammo;	//��ȡ������ҩ
								Enemy_iter = EnemyList.erase(Enemy_iter);		//�����������
								PlaySound("EXP.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
								Enemy_Kill++;
							}
							Missile_iter=MissileList.erase(Missile_iter);	//�����Լ�Ҳ������,������һ������
							break;
						}
						else//û����
						{
							Enemy_iter++;
							if(Enemy_iter == EnemyList.end())
							{
								Missile_iter++;
								break;
							}
						}
					}
				if(EnemyList.size() == 0 && 
				   Missile_iter != MissileList.end())
				{
					Missile_iter++;
				}
		}
		
		for(vector<CMissile>::iterator Missile_iter = MissileList.begin();
			Missile_iter!=MissileList.end(); 
			Missile_iter++)
		{
			for(vector<CStone>::iterator Stone_iter = StoneList.begin();
				Stone_iter!=StoneList.end();
				Stone_iter++)
			{
				diff = (int)sqrt((double)((Missile_iter->x-Stone_iter->x)*(Missile_iter->x-Stone_iter->x)+
										  (Missile_iter->y-Stone_iter->y)*(Missile_iter->y-Stone_iter->y)));
				hit = Missile_iter->x_offset + Stone_iter->x_offset;
				if(diff < hit)
				{
					Stone_iter->life -= Missile_iter->damage;
					if(Stone_iter->life > 0)//�����л�û��
						Stone_iter->ChangeColor();
					else					//�����ж��ҹҵ���
					{
						ship.score+=Stone_iter->score;
						ship.laser_ammo+=Stone_iter->laser_ammo;
						ship.missile_ammo+=Stone_iter->missile_ammo;

						//erase���ر�ɾ����Ԫ��(����һ�鱻ɾ��Ԫ�ص����һ��Ԫ��)����һ��Ԫ�صĵ�����
						Stone_iter=StoneList.erase(Stone_iter);//���˾����ٵ�
						PlaySound("EXP.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
					}
					Missile_iter = MissileList.erase(Missile_iter);
					if (MissileList.empty())
					{
						return 1;
					}
					break;
				}				
			}
		}
	}
	/*
	if (flag == IS_LASER)
	{
		for(vector<CLaser>::iterator Laser_iter = LaserList.begin(); Laser_iter!=LaserList.end(); ++Laser_iter)
		{
			for(vector<CEnemy>::iterator Enemy_iter = EnemyList.begin(); Enemy_iter!=EnemyList.end(); ++Enemy_iter)
			{
				diff = (int)sqrt((double)((Laser_iter->x - Enemy_iter->x) * (Laser_iter->x - Enemy_iter->x) +
					(Laser_iter->y - Enemy_iter->y) * (Laser_iter->y - Enemy_iter->y)));
				hit = Laser_iter->width + Enemy_iter->x_offset;

				if(diff < hit)
				{
					Enemy_iter->life -= Laser_iter->damage;
					if(Enemy_iter->life > 0)
						Enemy_iter->ChangeColor();
					else
					{
						ship.score += Enemy_iter->score;
						ship.laser_ammo += Enemy_iter->laser_ammo;
						ship.missile_ammo += Enemy_iter->missile_ammo;

						EnemyList.erase(Enemy_iter);
						PlaySound("EXP.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);

						Enemy_iter--;
						Enemy_Kill ++;
					}
					LaserList.erase(Laser_iter);
					if (LaserList.empty())
					{
						return 1;
					}
					Laser_iter --;
					break;
				}
				
			}
		}

		for(vector<CLaser>::iterator Laser_iter = LaserList.begin(); Laser_iter!=LaserList.end(); ++Laser_iter)
		{
			for(vector<CStone>::iterator Stone_iter = StoneList.begin(); Stone_iter!=StoneList.end(); ++Stone_iter)
			{
				diff = (int)sqrt((double)((Laser_iter->x - Stone_iter->x) * (Laser_iter->x - Stone_iter->x) +
					(Laser_iter->y - Stone_iter->y) * (Laser_iter->y - Stone_iter->y)));
				hit = Laser_iter->width + Stone_iter->x_offset;

				if(diff < hit)
				{
					Stone_iter->life -= Laser_iter->damage;
					if(Stone_iter->life > 0)
						Stone_iter->ChangeColor();
					else
					{
						ship.score += Stone_iter->score;
						ship.laser_ammo += Stone_iter->laser_ammo;
						ship.missile_ammo += Stone_iter->missile_ammo;

						StoneList.erase(Stone_iter);

						PlaySound("EXP.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
						Stone_iter--;
					}
					LaserList.erase(Laser_iter);
					if (LaserList.empty())
					{
						return 1;
					}
					Laser_iter--;
					break;
				}
				
			}
		}
	}
	if (flag == IS_CANON)
	{
		for(vector<CCanon>::iterator Canon_iter = CanonList.begin(); Canon_iter!=CanonList.end(); ++Canon_iter)
		{
			
			diff = (int)sqrt((double)((Canon_iter->x - ship.x) * (Canon_iter->x - ship.x) +
				(Canon_iter->y - ship.y) * (Canon_iter->y - ship.y)));
			hit = Canon_iter->width + ship.x_offset;

			if(diff < hit)
			{
				ship.life -= Canon_iter->damage;
				if(ship.life > 0)
					ship.ChangeColor();
				else if(ship.life <= 0)
				{
					PlaySound("EXP.wav", main_window_hinstance, SND_ASYNC | SND_FILENAME);
					Game_Over();
					return 1;
				}

				CanonList.erase(Canon_iter);

				if (CanonList.empty())
				{
					return 1;
				}
				Canon_iter--;
				break;
			}
				
		}
		
	}
	if (flag == IS_SUPPLY)
	{
		for(vector<CSupply>::iterator Supply_iter = SupplyList.begin(); Supply_iter!=SupplyList.end(); ++Supply_iter)
		{
			
			diff = (int)sqrt((double)((Supply_iter->x - ship.x) * (Supply_iter->x - ship.x) +
				(Supply_iter->y - ship.y) * (Supply_iter->y - ship.y)));
			hit = Supply_iter->x_offset + ship.x_offset;
			
			if(diff <= hit)
			{
				ship.life += Supply_iter->life;
				ship.laser_ammo += Supply_iter->laser_ammo;
				ship.missile_ammo += Supply_iter->missile_ammo;

				if(ship.life > ship.life_max)
					ship.life = ship.life_max;

				if(ship.life < ship.life_max)
					ship.ChangeColor();

				PlaySound("supply.wav",main_window_hinstance,SND_ASYNC | SND_FILENAME);

				SupplyList.erase(Supply_iter);
				if (SupplyList.empty())
				{
					return 1;
				}
				Supply_iter--;
				break;
			}		
		}		
	}
	*/
	return 0;
}

void Game_Over()
{
	for(int i = 0; i < 60; i++)
	{
		for(int j = 0; j <= 10; j++)
		{
			MoveToEx(global_hdc, 1, i*10+j, NULL);
			LineTo(global_hdc,WINDOW_WIDTH-1,i*10+j);
		}
		Sleep(25);
	}

	if (MessageBox(main_window_handle,"��Ϸ������ȷ���˳���","����",MB_YESNO) == IDYES)
	{
		PostMessage(main_window_handle,WM_CLOSE,0,0);
		//window_closed = 1;
	} 
	Game_Shutdown();
}