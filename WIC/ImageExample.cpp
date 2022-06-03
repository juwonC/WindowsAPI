#include <fstream>
#include <vector>
#include "ImageExample.h"

#pragma comment (lib, "WindowsCodecs.lib")

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = CoInitialize(nullptr);
	ThrowIfFailed(hr);

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);
	
	D2DFramework::Initialize(hInstance, title, width, height);

	//LoadBMP(L"Data/32.bmp", mspBitmap.GetAddressOf());
	//LoadBMP(L"Data/32.bmp", mspBitmap.GetAddressOf());
	//LoadWICImage(L"Data/32.bmp", mspBitmap.GetAddressOf());
	LoadWICImage(L"Data/1.png", mspBitmap.GetAddressOf());

	return S_OK;
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	mspRenderTarget->EndDraw();
}

void ImageExample::Release()
{
	D2DFramework::Release();

	mspBitmap.Reset();
	mspWICFactory.Reset();

	CoUninitialize();
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. ���� ����
	std::ifstream file;
	file.open(filename, std::ios::binary);

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

HRESULT ImageExample::LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. Decoder ����
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;
	hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. Decoder���� Frame ȹ��
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. Converter�� ����ؼ� ���ϴ� �������� ��ȯ
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);
	
	// 4. ��ȯ�� �����Ϳ��� ��Ʈ�� ����
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}
