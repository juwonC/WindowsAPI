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
	// 1. ���� ����
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	// 2. BITMAPFILEHEADER �б�
	BITMAPFILEHEADER bfh;
	file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));

	// 3. BITMAPINFOHEADER �б�
	BITMAPINFOHEADER bih;
	file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	if (bfh.bfType != 0x4D42) // ��Ʋ ������̶� �����´�
	{
		return E_FAIL;
	}
	if (bih.biBitCount != 32) // 4����Ʈ�� �ƴϸ� �о����� �ʴ´�
	{
		return E_FAIL;
	}

	// 4. �ȼ��� �ǳʶٱ�
	file.seekg(bfh.bfOffBits);

	// 5. ��Ʈ�� �б�(�ȼ� �迭)
	std::vector<char> pPixels(bih.biSizeImage);
	int pitch = bih.biWidth * (bih.biBitCount / 8);

	//file.read(&pPixels[0], bih.biSizeImage);

	// pitch ������ ����� �д´�
	//for (int y = bih.biHeight - 1; y >= 0; --y)
	//{
	//	file.read(&pPixels[y * pitch], pitch);
	//}

	// ��Ʈ�� �̹��� ��� ����
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

	// 6. 5�� �����ͷ� BITMAP �����
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
