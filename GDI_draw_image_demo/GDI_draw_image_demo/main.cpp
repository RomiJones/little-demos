#include "windows.h"

HINSTANCE	hInst = NULL;

HBITMAP		hbmp_memdc = NULL;
HBITMAP		old_hbmp_memdc = NULL;
HDC			memdc = NULL;

HBITMAP		hbmp_imgdc = NULL;
HBITMAP		old_hbmp_imgdc = NULL;
HDC			imgdc = NULL;

ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MyPaint(HWND hwnd, HDC hdc);
void initMemDC(HWND hwnd);
void uninitMemDC();
void flushDC(HDC hdc, int the_width, int the_height);

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
	RECT client_rect = {0};
	::GetClientRect(hwnd, &client_rect);

	HDC hdc = GetDC(hwnd);
	memdc = CreateCompatibleDC(hdc);	//memdc兼容GetDC出来的dc，应该能在同一个窗口上的BeginPaint出来的dc上bitblt贴图
	hbmp_memdc = ::CreateCompatibleBitmap(hdc, client_rect.right, client_rect.bottom);
	old_hbmp_memdc = (HBITMAP)::SelectObject(memdc, hbmp_memdc);

	imgdc = CreateCompatibleDC(NULL);	//memdc兼容GetDC出来的dc，应该能在同一个窗口上的BeginPaint出来的dc上bitblt贴图
	hbmp_imgdc = (HBITMAP)LoadImage(NULL, bmp_file_name, IMAGE_BITMAP, image_width, image_height, LR_LOADFROMFILE); 
	old_hbmp_imgdc = (HBITMAP)::SelectObject(imgdc, hbmp_imgdc);

	ReleaseDC(hwnd,	hdc);
}

void uninitMemDC()
{
	::SelectObject(imgdc, old_hbmp_imgdc);
	::DeleteObject(hbmp_imgdc);
	::DeleteDC(imgdc);

	::SelectObject(memdc, old_hbmp_memdc);
	::DeleteObject(hbmp_memdc);
	::DeleteDC(memdc);
}

void flushDC(HDC hdc, int the_width, int the_height)
{
	RECT rect = {0};
	rect.right = the_width;
	rect.bottom = the_height;
	int cnt = 1;
	while (cnt > 0)
	{
		::FillRect(hdc, &rect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
		cnt--;
	}
}

void MyPaint(HWND hwnd, HDC hdc)
{
	if (memdc)
	{
		flushDC(memdc, image_width, image_height);
		BitBlt(memdc, 0, 0, image_width, image_height, imgdc, 0, 0, SRCCOPY);

		RECT client_rect = {0};
		::GetClientRect(hwnd, &client_rect);

		BitBlt(hdc, 0, 0, client_rect.right, client_rect.bottom, memdc, 0, 0, SRCCOPY);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cnt = 0;
	switch (message)
	{
		case WM_ERASEBKGND:
			{
				return TRUE;
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				//flushDC(hdc, image_width, image_height);
				MyPaint(hWnd, hdc);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			{
				DeleteDC(memdc);
				DeleteObject(hbmp_memdc);
				PostQuitMessage(0);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				::InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				::InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
