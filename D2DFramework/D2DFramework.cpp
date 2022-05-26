#include <sstream>
#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::Init(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		mspD2DFactory.GetAddressOf()
	);
	if (FAILED(hr))
	{
		ShowErrorMsg(L"Failed to create the D2D Factory");
		return hr;
	}

	RECT rc;
	GetClientRect(hwnd, &rc);
	hr = mspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
		),
		mspRenderTarget.GetAddressOf()
	);
	if (FAILED(hr))
	{
		ShowErrorMsg(L"Failed to create the D2D Render Target");
		return hr;
	}

	return S_OK;
}

void D2DFramework::Release()
{
	mspRenderTarget.Reset();
	mspD2DFactory.Reset();
}

void D2DFramework::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->EndDraw();
}

void D2DFramework::ShowErrorMsg(LPCWSTR msg, HRESULT error, LPCWSTR title)
{
	std::wostringstream oss;

	oss << L"ERROR : 0x" << std::hex << error << std::endl;
	oss << msg;
	OutputDebugString(oss.str().c_str());

	MessageBox(nullptr, oss.str().c_str(), title, MB_ICONEXCLAMATION | MB_OK);
}
