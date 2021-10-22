#define NOMINMAX

#include <algorithm>
using namespace std;
#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include <winsock.h>
#include <fstream>
#include <string>

#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

HBITMAP getImage()
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    HDC scrdc, memdc;
    HBITMAP membit;
    scrdc = GetDC(0);
    int Height, Width;
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    Height = GetSystemMetrics(SM_CYSCREEN);
    Width = GetSystemMetrics(SM_CXSCREEN);
    memdc = CreateCompatibleDC(scrdc);
    membit = CreateCompatibleBitmap(scrdc, Width, Height);
    SelectObject(memdc, membit);

    BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
    HBITMAP hBitmap;
    hBitmap = (HBITMAP)SelectObject(memdc, membit);
    return hBitmap;
}

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

char* convertImage(Gdiplus::Bitmap& image, int& buferSize)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID   encoderClsid;
    Status  stat;
    EncoderParameters encoderParameters;
    ULONG    quality;

    GetEncoderClsid(L"image/jpeg", &encoderClsid);

    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;

    quality = 15;
    encoderParameters.Parameter[0].Value = &quality;

    IStream* istream = nullptr;
    HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &istream);
    CLSID clsid_png;
    CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid_png);
    image.Save(istream, &encoderClsid, &encoderParameters);

    HGLOBAL hg = NULL;
    GetHGlobalFromStream(istream, &hg);
    int bufsize = GlobalSize(hg);
    char* buffer = new char[bufsize];

    LPVOID ptr = GlobalLock(hg);
    memcpy(buffer, ptr, bufsize);
    GlobalUnlock(hg);

    istream->Release();

    GdiplusShutdown(gdiplusToken);
    buferSize = bufsize;
    return buffer;
}

void readIp(char* argv[], char *ip, int& port)
{
    char chport[5];
    int i = 0;

    for (; i < 4; ++i)
        chport[i] = argv[0][i];
    sscanf_s(chport, "%d", &port);
    int k = 0;
    while (argv[0][i])
    {
        ip[k] = argv[0][i];
        ++k; ++i;
    }
    ip[k] = '\0';
}

int main(int argc, char* argv[])
{
    int port = 0;
    char ip[20];

    readIp(argv, ip, port);

    WSADATA WsaData;
    char sendingImageInfo[50];
    int check = 0;
    const int imagePartSize = 1024;
    if (WSAStartup(0x0101, &WsaData))
        return false;
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
        return FALSE;
    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = inet_addr(ip);

    fd_set fd;
    timeval tv;
    FD_ZERO(&fd);
    FD_SET(s, &fd);
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    if (connect(s, (struct sockaddr*)&sin, sizeof(struct sockaddr)))
        return FALSE;
    char start[10] = "start";
    char startBuffer[10];

    while (true)
    {
        // Get image:
        HBITMAP hBitmap = getImage();
        Gdiplus::Bitmap bitmap(hBitmap, NULL);

        int bufSize = 0;
        char* buffer = convertImage(bitmap, bufSize);
        DeleteObject(hBitmap);

        char imagePartBuffer[imagePartSize];

        std::string imageInfo;
        imageInfo = std::to_string(bufSize);
        for (int i = 0; i < imageInfo.size(); ++i)
        {
            sendingImageInfo[i] = imageInfo[i];
        }
        sendingImageInfo[imageInfo.size()] = '\0';

        int ImagePartCounter = 0;

        check = send(s, start, 10, 0);

        if (check != 10)
            exit(0);

        check = send(s, sendingImageInfo, 50, 0);

        if (check != 50)
            exit(0);

        check = send(s, buffer, bufSize, 0);

        if (check != bufSize)
            exit(0);

        delete[] buffer;
        Sleep(40);
    }
}
