#include <vector>
#include <windows.h>

using namespace std;

#define ID_TIMER    1
#define STRMAXLEN   24 //一个显示列的最大长度
#define STRMINLEN   8  //一个显示列的最小长度

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

typedef struct tagCharNode
{
	struct tagCharNode *prev;	//链表的前个元素
	TCHAR  ch;					//一个显示列中的一个字符
	struct tagCharNode *next;	//链表的后个元素
}CharNode, *pCharNode;

typedef struct tagCharChain
{
	CharNode* alloc_pointer;	//保存分配的堆内存地址，为了释放
	CharNode* head;				//可移动的头部指针
	int offset_y;		//字符链的头部在y轴上的偏移
	int strLen;			//字符链的长度
	int iStopTimes;		//下降速度控制参数-已经停滞的次数
	int iMustStopTimes;	//下降速度控制参数-必须停滞的次数，随机值
}CharChain, *pCharChain;

int WINAPI WinMain (HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
					PSTR szCmdLine, 
					int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("matrix") ;
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass ;

	//register window
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	//黑色背景
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szAppName ;
	if(!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("should run in win NT"), szAppName, MB_ICONERROR) ;
		return 0;
	}

	hwnd = CreateWindow(szAppName, NULL,
						WS_DLGFRAME | WS_THICKFRAME | WS_POPUP,
						0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
						NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWMAXIMIZED) ;	//最大化显示
	UpdateWindow(hwnd);

	ShowCursor(FALSE);						//隐藏鼠标光标
	srand((int)GetCurrentTime());			//初始化随机数发生器
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	ShowCursor(TRUE);					//显示鼠标光标
	return 0;
}

TCHAR randomChar()
{
	return (TCHAR)(rand()%(126-33)+33); //33到126之间,0~32为不可显示的字符,127为delete
}

void releaseCharChain(CharChain* cc)
{
	if(cc->alloc_pointer)
	{
		free(cc->alloc_pointer);
	}
	memset(cc, 0, sizeof(CharChain));
}

void resetCharChain(CharChain* cc)
{
	releaseCharChain(cc);

	cc->strLen = rand() % (STRMAXLEN - STRMINLEN) + STRMINLEN;
	cc->offset_y = 0;
	cc->iMustStopTimes = rand() % 6;
	cc->iStopTimes = 0;

	cc->alloc_pointer = cc->head = (pCharNode)calloc(cc->strLen, sizeof(CharNode));
	CharNode* temp_prev = cc->head + cc->strLen - 1;
	CharNode* temp_current = cc->head;
	for(int i = 0; i < cc->strLen - 1; i++)
	{
		temp_current->prev = temp_prev;
		temp_current->ch   = '\0';
		temp_current->next = temp_current + 1;
		temp_prev = temp_current;
		temp_current++;
	}

	//last node
	temp_current->prev = temp_prev;
	temp_current->ch   = '\0';
	temp_current->next = cc->head;

	//first node data
	cc->head->ch = randomChar();
}

HBITMAP hBitmapOld = NULL;
HFONT hFontOld = NULL;
HDC hdcMem = NULL;
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxScreen, cyScreen;					//屏幕的宽高.
	static int iFontWidth = 10,iFontHeight = 15;	//字体的宽高
	static vector<CharChain> ccChains;

	switch (message)
	{
	case WM_CREATE:
		{
			cxScreen = GetSystemMetrics(SM_CXSCREEN);	//屏幕宽度
			cyScreen = GetSystemMetrics(SM_CYSCREEN);	//屏幕长度
			SetTimer(hwnd, ID_TIMER, 10, NULL);

			//创建内存DC
			HDC hdc = GetDC(hwnd);
			hdcMem = CreateCompatibleDC(hdc);							//创建和窗口DC兼容的内存DC
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen);	//创建和窗口dc兼容的位图
			hBitmapOld = (HBITMAP)::SelectObject(hdcMem, hBitmap);	//位图选入内存DC
			ReleaseDC(hwnd, hdc);

			HFONT hFont = CreateFont(iFontHeight, iFontWidth - 5, 0, 0, FW_BOLD, 0, 0, 0,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DRAFT_QUALITY, FIXED_PITCH | FF_SWISS, TEXT("Fixedsys"));
			::SelectObject(hdcMem, hFont);
			::SetBkMode(hdcMem, TRANSPARENT);				//设置背景模式为 透明

			int iColumnCount = cxScreen / (iFontWidth * 3 / 2);	//屏幕所显示字母雨的列数
			ccChains.assign(iColumnCount, CharChain());			//每一列都是一个CharChain
			for(int i = 0; i < ccChains.size(); i++)
			{
				resetCharChain(&ccChains[i]);
			}
		}
		break;
	case WM_TIMER:
		{
			PatBlt (hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS);
			for(int i = 0; i < ccChains.size(); i++)
			{
				CharChain* cc = &ccChains[i];
				bool to_move = cc->iStopTimes > cc->iMustStopTimes;
				cc->iStopTimes++;

				CharNode* temp_current = cc->head;

				//画头部白色字符
				int display_x = (iFontWidth * 3 / 2) * i;
				SetTextColor(hdcMem, RGB(255, 255, 255));
				TextOut(hdcMem, display_x, cc->offset_y, &(temp_current->ch), 1); 
				temp_current = temp_current->next;

				//画剩余渐淡的字符
				for(int j = cc->offset_y, temp = 0 ; temp_current != cc->head && temp_current->ch;)
				{
					SetTextColor(hdcMem, RGB(0, 255 - (255 * (temp++) / cc->strLen), 0));
					TextOut(hdcMem, display_x, j -= iFontHeight, &(temp_current->ch), 1);
					temp_current = temp_current->next;
				}
				
				if(!to_move) continue;

				cc->iStopTimes = 0;				//recount the stop times
				cc->offset_y += iFontHeight;	//下次开始显示的y坐标 为当前的y坐标加上 一个字符的高度
				if(cc->offset_y - cc->strLen * iFontHeight > cyScreen) //如果头部字符的y坐标减去整个显示列的长度超过了屏幕的高度，则重设创建
				{
					resetCharChain(cc);
				}
				else
				{
					//链表的头为此链表的前个元素,因为下次开始显示的时候 就相当与在整个显示列的开头添加个元素,然后在开始往上显示
					cc->head = cc->head->prev;
					cc->head->ch = randomChar();
				}
			}
			HDC hdc = GetDC(hwnd);
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY); //内存DC上画的，直接copy到显示DC上
			ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			::KillTimer(hwnd, ID_TIMER);
		}
		break;
	case WM_RBUTTONUP:
		{
			::SetTimer(hwnd, ID_TIMER, 10, NULL);
		}
		break;
	case WM_KEYDOWN:
	case WM_DESTROY:
		{
			::KillTimer (hwnd, ID_TIMER);
			HBITMAP hBitmap = (HBITMAP)::SelectObject(hdcMem, hBitmapOld);
			HFONT hFont = (HFONT)::SelectObject(hdcMem, hFontOld);
			::DeleteObject(hBitmap);
			::DeleteObject(hFont);
			::DeleteDC(hdcMem);
			for(int i = 0; i<ccChains.size(); i++)
			{
				releaseCharChain(&ccChains[i]);
			}
			::PostQuitMessage(0);
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

