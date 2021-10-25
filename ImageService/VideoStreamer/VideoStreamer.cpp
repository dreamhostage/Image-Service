#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Winsock2.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <sys/types.h>
#include <Winsock2.h>
#include <windows.h>
#include <sstream>
#include <shellapi.h>
#include <filesystem>
#include <conio.h>
#include <string>
#include <locale>
#include <shlobj_core.h>

#pragma warning( disable : 4996)
#pragma comment(lib, "Ws2_32.lib")

using namespace sf;

int main(int argc, char* argv[])
{
    int port = 0;
    std::string sPort;
    std::string username;
    std::string command = argv[0];
    int i = 0;
    while (command[i] != '\n')
        sPort += argv[0][i++];
    ++i;
    while (i < command.size())
        username += command[i++];
    sscanf_s(sPort.c_str(), "%d", &port);
    port -= 2;

    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    int bufSize = 0;
    int check = 0;
    std::string size;
    char buffer[10];
    char start[10] = "start";
    char info[50];

    WSADATA WsaData;
    SOCKADDR_IN sin;

    WSAStartup(0x0101, &WsaData);
    SOCKET sMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    bind(sMain, (LPSOCKADDR)&sin, sizeof(sin));
    listen(sMain, SOMAXCONN);
    int fromlen = sizeof(sin);
    SOCKET s = accept(sMain, (struct sockaddr*)&sin, &fromlen);

    RenderWindow window(VideoMode(800, 600), username);

    Texture texture;

    Sprite sprite;

    int n = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        recv(s, buffer, 10, MSG_WAITALL);
        if (!strcmp(start, buffer))
        {
            recv(s, info, 50, MSG_WAITALL);
            size = info;
            bufSize = std::stoi(size);
            std::vector<char> imageBuf;
            imageBuf.reserve(bufSize);
            check = recv(s, imageBuf.data(), bufSize, MSG_WAITALL);
            MemoryInputStream stream;
            stream.open(imageBuf.data(), bufSize);
            texture.loadFromStream(stream);
            sprite.setTexture(texture);
            imageBuf.clear();
        }
        if (check == bufSize)
        {
            float wx = window.getSize().x;
            float wy = window.getSize().y;
            float sx = texture.getSize().x;
            float sy = texture.getSize().y;
            sprite.setScale(wx / sx, wy / sy);
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
        else
        {
            Sleep(100);
        }
    }
}
