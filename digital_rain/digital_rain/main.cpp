#include <vector>
#include <windows.h>

using namespace std;

#define ID_TIMER    1
#define STRMAXLEN   24 //һ����ʾ�е���󳤶�
#define STRMINLEN   8  //һ����ʾ�е���С����

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

typedef struct tagCharNode
{
	struct tagCharNode *prev;	//�����ǰ��Ԫ��
	TCHAR  ch;					//һ����ʾ���е�һ���ַ�
	struct tagCharNode *next;	//����ĺ��Ԫ��
}CharNode, *pCharNode;

typedef struct tagCharChain
{
	CharNode* alloc_pointer;	//�������Ķ��ڴ��ַ��Ϊ���ͷ�
	CharNode* head;				//���ƶ���ͷ��ָ��
	int offset_y;		//�ַ�����ͷ����y���ϵ�ƫ��
	int strLen;			//�ַ����ĳ���
	int iStopTimes;		//�½��ٶȿ��Ʋ���-�Ѿ�ͣ�͵Ĵ���
	int iMustStopTimes;	//�½��ٶȿ��Ʋ���-����ͣ�͵Ĵ��������ֵ
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
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	//��ɫ����
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

	ShowWindow(hwnd, SW_SHOWMAXIMIZED) ;	//�����ʾ
	UpdateWindow(hwnd);

	ShowCursor(FALSE);						//���������
	srand((int)GetCurrentTime());			//��ʼ�������������
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	ShowCursor(TRUE);					//��ʾ�����
	return 0;
}

TCHAR randomChar()
{
	return (TCHAR)(rand()%(126-33)+33); //33��126֮��,0~32Ϊ������ʾ���ַ�,127Ϊdelete
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
	static int cxScreen, cyScreen;					//��Ļ�Ŀ��.
	static int iFontWidth = 10,iFontHeight = 15;	//����Ŀ��
	static vector<CharChain> ccChains;

	switch (message)
	{
	case WM_CREATE:
		{
			cxScreen = GetSystemMetrics(SM_CXSCREEN);	//��Ļ���
			cyScreen = GetSystemMetrics(SM_CYSCREEN);	//��Ļ����
			SetTimer(hwnd, ID_TIMER, 10, NULL);

			//�����ڴ�DC
			HDC hdc = GetDC(hwnd);
			hdcMem = CreateCompatibleDC(hdc);							//�����ʹ���DC���ݵ��ڴ�DC
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen);	//�����ʹ���dc���ݵ�λͼ
			hBitmapOld = (HBITMAP)::SelectObject(hdcMem, hBitmap);	//λͼѡ���ڴ�DC
			ReleaseDC(hwnd, hdc);

			HFONT hFont = CreateFont(iFontHeight, iFontWidth - 5, 0, 0, FW_BOLD, 0, 0, 0,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DRAFT_QUALITY, FIXED_PITCH | FF_SWISS, TEXT("Fixedsys"));
			::SelectObject(hdcMem, hFont);
			::SetBkMode(hdcMem, TRANSPARENT);				//���ñ���ģʽΪ ͸��

			int iColumnCount = cxScreen / (iFontWidth * 3 / 2);	//��Ļ����ʾ��ĸ�������
			ccChains.assign(iColumnCount, CharChain());			//ÿһ�ж���һ��CharChain
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

				//��ͷ����ɫ�ַ�
				int display_x = (iFontWidth * 3 / 2) * i;
				SetTextColor(hdcMem, RGB(255, 255, 255));
				TextOut(hdcMem, display_x, cc->offset_y, &(temp_current->ch), 1); 
				temp_current = temp_current->next;

				//��ʣ�ཥ�����ַ�
				for(int j = cc->offset_y, temp = 0 ; temp_current != cc->head && temp_current->ch;)
				{
					SetTextColor(hdcMem, RGB(0, 255 - (255 * (temp++) / cc->strLen), 0));
					TextOut(hdcMem, display_x, j -= iFontHeight, &(temp_current->ch), 1);
					temp_current = temp_current->next;
				}
				
				if(!to_move) continue;

				cc->iStopTimes = 0;				//recount the stop times
				cc->offset_y += iFontHeight;	//�´ο�ʼ��ʾ��y���� Ϊ��ǰ��y������� һ���ַ��ĸ߶�
				if(cc->offset_y - cc->strLen * iFontHeight > cyScreen) //���ͷ���ַ���y�����ȥ������ʾ�еĳ��ȳ�������Ļ�ĸ߶ȣ������贴��
				{
					resetCharChain(cc);
				}
				else
				{
					//�����ͷΪ�������ǰ��Ԫ��,��Ϊ�´ο�ʼ��ʾ��ʱ�� ���൱����������ʾ�еĿ�ͷ��Ӹ�Ԫ��,Ȼ���ڿ�ʼ������ʾ
					cc->head = cc->head->prev;
					cc->head->ch = randomChar();
				}
			}
			HDC hdc = GetDC(hwnd);
			BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY); //�ڴ�DC�ϻ��ģ�ֱ��copy����ʾDC��
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

