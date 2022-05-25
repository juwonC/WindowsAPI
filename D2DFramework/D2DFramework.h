#pragma once

#include <d2d1.h>
#include <wrl/client.h>

class D2DFramework
{
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

public:
	virtual HRESULT Inint(HWND hwnd);
	virtual void Release();
	virtual void Render();


};

