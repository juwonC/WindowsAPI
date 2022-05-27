#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <sstream>

class com_exception : public std::exception
{
private:
	HRESULT mResult;

public:
	com_exception(HRESULT hr) : mResult(hr) {}

	virtual const char* what() const override
	{
		std::stringstream ss;
		ss << "Failure with Code : "
			<< mResult << std::endl;

		return ss.str().c_str();
	}

};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error = 0, LPCWSTR title = L"Error");
};

