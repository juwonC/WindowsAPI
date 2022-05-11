#include <windows.h>
#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain
(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupinput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupinput, NULL);

	HWND hwnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed To Register Window", L"Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"MyWindow",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hwnd)
	{
		MessageBox(NULL, L"Failed To Create Window", L"Error", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 255));
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 100, 0));
	Gdiplus::FontFamily fontFamily(L"Times New Roman");
	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF point(0.0f, 100.0f);
	Gdiplus::RectF rect(100.0f, 0.0f, 50.0f, 50.0f);

	graphics.DrawLine(&pen, 0, 0, 50, 50);
	graphics.DrawString(L"Hello World!", -1, &font, point, &brush);
	graphics.DrawRectangle(&pen, rect);
	graphics.FillRectangle(&brush, rect);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			OnPaint(hwnd);
			break;
		
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}