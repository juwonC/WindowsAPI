#include <Windows.h>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	MessageBox(
		nullptr,
		L"Hello World!",
		L"SimpleWindwo",
		MB_ICONEXCLAMATION | MB_OKCANCEL
	);

	return 0;
}