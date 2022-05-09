// 1. 윈도우 클래스 등록
// 2. 윈도우 만들기 (1번의 윈도우 클래스)
// 3. 윈도우 메시지 루프 처리
// 4. 윈도우 프로시져(Procedure) 작성

#include <windows.h>
#include <sstream>

const wchar_t ClassName[] = L"Window Class";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd
)
{
	BOOL bRet;
	WNDCLASSEX wc{};

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = L"MyWindow";
	wc.lpszClassName = ClassName;

	RegisterClassEx(&wc);
	
	HWND hwnd = CreateWindowEx(
		0, 
		ClassName,
		L"Mywindow", 
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	// Window Message Loop
	MSG msg;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// Error
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
	
	//Ellipse(hdc, 0, 0, 100, 100);
	//LineTo(hdc, 50, 50);
	AngleArc(hdc, 100, 100, 70, 0, 70.5);

	DeleteObject(redPen);
	SelectObject(hdc, oldPen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	Rectangle(hdc, 100, 100, 0, 200);
	RoundRect(hdc, 150, 150, 200, 200, 10, 50);

	DeleteObject(brush);
	SelectObject(hdc, oldBrush);

	HFONT font = (HFONT)GetStockObject(ANSI_VAR_FONT);
	HFONT oldFont = (HFONT)SelectObject(hdc, font);
	
	TextOut(hdc, 200, 200, L"ANSI_VAR_FONT", 13);

	DeleteObject(font);

	TRIVERTEX vertex[2];
	vertex[0].x = 300;
	vertex[0].y = 100;
	vertex[0].Red = 0xd000;
	vertex[0].Green = 0x8000;
	vertex[0].Blue = 0x8000;
	vertex[0].Alpha = 0x0000;

	vertex[1].x = 600;
	vertex[1].y = 10;
	vertex[1].Red = 0x8000;
	vertex[1].Green = 0xd000;
	vertex[1].Blue = 0xd000;
	vertex[1].Alpha = 0x0000;

	GRADIENT_RECT gRect;
	gRect.UpperLeft = 0;
	gRect.LowerRight = 1;

	GdiGradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			std::wostringstream oss;
			
			oss << wParam << " " << lParam << std::endl;
			MessageBox(hwnd, oss.str().c_str(), L"Keyboard Input", MB_OK);

			if (wParam == VK_DOWN)
			{
				OutputDebugStringW(oss.str().c_str());
			}

			break;
		}
		
		case WM_LBUTTONDOWN:
		{
			POINT pt;

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			std::wostringstream oss;

			oss << "X : " << pt.x << ", Y : " << pt.y << std::endl;

			//MessageBox(hwnd, oss.str().c_str(), L"Coordinate", MB_OK);

			OutputDebugStringW(oss.str().c_str());

			break;
		}

		case WM_PAINT:
			OnPaint(hwnd);
			break;

		case WM_CLOSE:
			if (MessageBox(hwnd, L"Really quit?", L"My Winodw", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hwnd);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	
	return 0;
}
