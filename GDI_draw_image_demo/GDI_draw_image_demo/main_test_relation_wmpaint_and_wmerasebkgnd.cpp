#include "windows.h"

HINSTANCE	hInst = NULL;
HBITMAP		hbmp = NULL;
HBITMAP		old_hbmp = NULL;
HDC			memdc = NULL;

ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MyPaint(HDC hdc);
void initMemDC(HWND hwnd);
void uninitMemDC();
void displayStrWithCnt(wchar_t* str);

const wchar_t wnd_class_name[] = L"canvas";
const wchar_t wnd_title[] = L"绘图demo";
const wchar_t bmp_file_name[] = L"bg.bmp";

const int image_width = 510;
const int image_height = 352;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);

	hInst = hInstance;

	HWND hWnd = CreateWindow(wnd_class_name, wnd_title, WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL,
							hInstance, NULL);
	if (!hWnd) return 1;

	::MoveWindow(hWnd, 10, 10, 800, 600, true);
	::ShowWindow(hWnd, nCmdShow);

	initMemDC(hWnd);

	::UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	uninitMemDC();
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);//(HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= wnd_class_name;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

void initMemDC(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	memdc = CreateCompatibleDC(hdc);	//memdc兼容GetDC出来的dc，应该能在同一个窗口上的BeginPaint出来的dc上bitblt贴图
	hbmp = (HBITMAP)LoadImage(NULL, bmp_file_name, IMAGE_BITMAP, image_width, image_height, LR_LOADFROMFILE); 
	old_hbmp = (HBITMAP)::SelectObject(memdc, hbmp);
	ReleaseDC(hwnd,	hdc);
}

void uninitMemDC()
{
	::SelectObject(memdc, old_hbmp);
	::DeleteObject(hbmp);
	::DeleteDC(memdc);
}

void MyPaint(HDC hdc)
{
	if (memdc)
		BitBlt(hdc, 0, 0, image_width,image_height, memdc, 0, 0, SRCCOPY);
}

void displayStrWithCnt(wchar_t* str)
{
	static int cnt = 0;
	wchar_t str_tmp[128] = {0};
	wsprintf(str_tmp, L"%s:%d\n",str,cnt);
	OutputDebugString(str_tmp);
	cnt++;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cnt = 0;
	switch (message)
	{
		case WM_ERASEBKGND:
			{
				displayStrWithCnt(L"WM_ERASEBKGND handler");
				return TRUE;
			}
			break;
		case WM_PAINT:
			{
				displayStrWithCnt(L"WM_PAINT handler begin");
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				displayStrWithCnt(ps.fErase?L"PAINTSTRUCT.fErase:true":L"PAINTSTRUCT.fErase:false");
				MyPaint(hdc);
				EndPaint(hWnd, &ps);
				displayStrWithCnt(L"WM_PAINT handler end");
			}
			break;
		case WM_DESTROY:
			{
				DeleteDC(memdc);
				DeleteObject(hbmp);
				PostQuitMessage(0);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				::InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				::InvalidateRect(hWnd,NULL,TRUE);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
