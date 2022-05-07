// 1. 윈도우 클래스 등록
// 2. 윈도우 만들기 (1번의 윈도우 클래스)
// 3. 윈도우 메시지 루프 처리
// 4. 윈도우 프로시져(Procedure) 작성

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASSA wc;

	wc.style			= CS_DBLCLKS | CS_PARENTDC;
	wc.lpfnWndProc		= (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, (LPTSTR)IDC_IBEAM);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= "MyWindow";
	wc.lpszClassName	= "MyWindow";

	RegisterClassA(&wc);
	

	// Window Message Loop
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// Error
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CHAR:
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	
	return 0;
}
