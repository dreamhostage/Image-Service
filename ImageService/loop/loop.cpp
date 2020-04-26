#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <gdiplus.h>
#include <string>
#include <ctime>
#include <sstream>
#include <winsock.h>
#include <sys/types.h>
#include <locale>
#include <shlobj_core.h>


#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

VOID SaveFile(const wchar_t* name, const wchar_t* sname)
{
	setlocale(LC_ALL, "Russian");
	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID   encoderClsid;
	Status  stat;
	EncoderParameters encoderParameters;
	ULONG    quality;
	Image* image = new Gdiplus::Image(name);

	// Get the CLSID of the PNG encoder.
	GetEncoderClsid(L"image/jpeg", &encoderClsid);

	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	quality = 20;
	encoderParameters.Parameter[0].Value = &quality;

	stat = image->Save(sname, &encoderClsid, &encoderParameters);

	delete image;
	GdiplusShutdown(gdiplusToken);
	return;
}

void TakeScreen(const wchar_t* name)
{
	setlocale(LC_ALL, "Russian");
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC scrdc, memdc;
	HBITMAP membit;
	// Получаем HDC рабочего стола
	// Параметр HWND для рабочего стола всегда равен нулю.
	scrdc = GetDC(0);
	// Определяем разрешение экрана
	int Height, Width;
	Height = 1080; //GetSystemMetrics(SM_CYSCREEN);
	Width = 1920;//GetSystemMetrics(SM_CXSCREEN);
	// Создаем новый DC, идентичный десктоповскому и битмап размером с экран.
	memdc = CreateCompatibleDC(scrdc);
	membit = CreateCompatibleBitmap(scrdc, Width, Height);
	SelectObject(memdc, membit);
	// Улыбаемся... Снято!

	BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap(hBitmap, NULL);

	bitmap.Save(name, &png);

	SetFileAttributes(name, FILE_ATTRIBUTE_HIDDEN);

	DeleteObject(hBitmap);
}

wchar_t* GetWC(char* c, size_t cSize)
{
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	std::wstring name;
	std::wstring sname;
	size_t ii = 0;
	while (argv[0][ii++]);
	wchar_t* n = GetWC(argv[0], ii);

	wchar_t wBuff[500];
	SHGetSpecialFolderPath(0, wBuff, CSIDL_PERSONAL, true);

	name = wBuff;
	name += L"\\";
	name += n;
	name += L".png";

	sname = wBuff;
	sname += L"\\";
	sname += n;
	sname += L".jpg";

	TakeScreen(&name[0]);
	SaveFile(&name[0], &sname[0]);

	HANDLE tFile = CreateFile(&name[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	CloseHandle(tFile);
	delete[] n;

	return 0;
}