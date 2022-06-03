#include <fstream>
#include <vector>
#include "ImageExample.h"

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	LoadBMP(L"Data/32.bmp", mspBitmap.GetAddressOf());
	
	return S_OK;
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	mspRenderTarget->EndDraw();
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. 파일 열기
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	// 2. BITMAPFILEHEADER 읽기
	BITMAPFILEHEADER bfh;
	file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));

	// 3. BITMAPINFOHEADER 읽기
	BITMAPINFOHEADER bih;
	file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	if (bfh.bfType != 0x4D42) // 리틀 엔디안이라서 뒤집는다
	{
		return E_FAIL;
	}
	if (bih.biBitCount != 32) // 4바이트가 아니면 읽어지지 않는다
	{
		return E_FAIL;
	}

	// 4. 픽셀로 건너뛰기
	file.seekg(bfh.bfOffBits);

	// 5. 비트맵 읽기(픽셀 배열)
	std::vector<char> pPixels(bih.biSizeImage);
	int pitch = bih.biWidth * (bih.biBitCount / 8);

	//file.read(&pPixels[0], bih.biSizeImage);

	// pitch 단위로 뒤집어서 읽는다
	//for (int y = bih.biHeight - 1; y >= 0; --y)
	//{
	//	file.read(&pPixels[y * pitch], pitch);
	//}

	// 비트맵 이미지 배경 제거
	int index{};

	for (int y = bih.biHeight - 1; y >= 0; --y)
	{
		index = y * pitch;
		for (int x = 0; x < bih.biWidth; ++x)
		{
			char r{}, g{}, b{}, a{};

			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (r == 30 || g == 199 || r == 250)
			{
				r = g = b = a = 0;
			}

			pPixels[index++] = b;
			pPixels[index++] = g;
			pPixels[index++] = r;
			pPixels[index++] = a;
		}
	}

	file.close();

	// 6. 5번 데이터로 BITMAP 만들기
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(bih.biWidth, bih.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		),
		ppBitmap
	);

	ThrowIfFailed(hr);

	(*ppBitmap)->CopyFromMemory(nullptr, &pPixels[0], pitch);
	
	return S_OK;
}
