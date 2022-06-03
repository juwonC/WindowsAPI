#pragma once

#include <wincodec.h>
#include "D2DFramework.h"

class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

public:
	virtual HRESULT Initialize(
		HINSTANCE hInstance, 
		LPCWSTR title = L"Direct2D Exmaple", 
		UINT width = 1024, UINT height = 768) override;

	void Render() override;
	void Release() override;

public:
	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
	HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};

