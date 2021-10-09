#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	LARGE_INTEGER sizeSetup = { 0 };
	LARGE_INTEGER sizebReader = { 0 };
	LARGE_INTEGER sizeImageService = { 0 };
	LARGE_INTEGER sizeInfo = { 0 };
	LARGE_INTEGER sizeloop = { 0 };
	DWORD check = 0;
	std::string info;
	std::string ip;
	std::string port;
	char isize[100];

	std::cout << "Enter IP: ";
	std::cin >> ip;
	std::cout << "Enter port: ";
	std::cin >> port;

	info += "port: ";
	info += port;
	info += '\n';
	info += "ip:   ";
	info += ip;

	if (!CreateDirectoryA("Installer", NULL))
	{
		std::cout << "Can't create Installer folder...\n";
		system("pause");
		return 0;
	}

	HANDLE hPacker = CreateFileA("Installer\\Setup.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hSetup = CreateFileA("Setup.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hbReader = CreateFileA("bReader.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hImageService = CreateFileA("ImageService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hInfo = CreateFileA("Installer\\Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hloop = CreateFileA("loop.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hPacker == INVALID_HANDLE_VALUE || hSetup == INVALID_HANDLE_VALUE || hbReader == INVALID_HANDLE_VALUE || hImageService == INVALID_HANDLE_VALUE || hInfo == INVALID_HANDLE_VALUE || hloop == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open or create files:\n";
		if (hPacker == INVALID_HANDLE_VALUE)
			std::cout << "Installer\\Setup.exe;\n";
		if (hSetup == INVALID_HANDLE_VALUE)
			std::cout << "Setup.exe;\n";
		if (hbReader == INVALID_HANDLE_VALUE)
			std::cout << "bReader.exe;\n";
		if (hImageService == INVALID_HANDLE_VALUE)
			std::cout << "ImageServicp.exe;\n";
		if (hInfo == INVALID_HANDLE_VALUE)
			std::cout << "Info.exe;\n";
		if (hloop == INVALID_HANDLE_VALUE)
			std::cout << "loop.exe;\n";
		system("pause");
		return false;
	}

	WriteFile(hInfo, &info[0], info.size(), &check, NULL);
	if (check != info.size())
	{
		std::cout << "Can't write info to Info.txt...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		system("pause");
		return 0;
	}

	CloseHandle(hInfo);
	hInfo = CreateFileA("Installer\\Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	if (hInfo == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open Info.exe\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		system("pause");
	}

	GetFileSizeEx(hSetup, &sizeSetup);
	GetFileSizeEx(hbReader, &sizebReader);
	GetFileSizeEx(hImageService, &sizeImageService);
	GetFileSizeEx(hInfo, &sizeInfo);
	GetFileSizeEx(hloop, &sizeloop);

	sprintf_s(isize, "%d", sizeInfo.QuadPart);

	char* bufSetup = new char[sizeSetup.QuadPart];
	char* bufbReader = new char[sizebReader.QuadPart];
	char* bufImageService = new char[sizeImageService.QuadPart];
	char* bufInfo = new char[sizeInfo.QuadPart];
	char* bufloop = new char[sizeloop.QuadPart];

	ReadFile(hSetup, bufSetup, sizeSetup.QuadPart, &check, NULL);
	if (check != sizeSetup.QuadPart)
	{
		std::cout << "Can't read Setup.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	ReadFile(hbReader, bufbReader, sizebReader.QuadPart, &check, NULL);
	if (check != sizebReader.QuadPart)
	{
		std::cout << "Can't read bReader.exe\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	ReadFile(hImageService, bufImageService, sizeImageService.QuadPart, &check, NULL);
	if (check != sizeImageService.QuadPart)
	{
		std::cout << "Can't read ImageService.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	ReadFile(hInfo, bufInfo, sizeInfo.QuadPart, &check, NULL);
	if (check != sizeInfo.QuadPart)
	{
		std::cout << "Can't read Info.txt...\n" << GetLastError() << std::endl;
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	ReadFile(hloop, bufloop, sizeloop.QuadPart, &check, NULL);
	if (check != sizeloop.QuadPart)
	{
		std::cout << "Can't read loop.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}

	SetFilePointer(hPacker, 0, 0, FILE_END);

	WriteFile(hPacker, bufSetup, sizeSetup.QuadPart, &check, NULL);
	if (check != sizeSetup.QuadPart)
	{
		std::cout << "Can't write from Setup.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	WriteFile(hPacker, bufbReader, sizebReader.QuadPart, &check, NULL);
	if (check != sizebReader.QuadPart)
	{
		std::cout << "Can't write from bReader.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	WriteFile(hPacker, bufImageService, sizeImageService.QuadPart, &check, NULL);
	if (check != sizeImageService.QuadPart)
	{
		std::cout << "Can't write from ImageService.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	WriteFile(hPacker, isize, 100, &check, NULL);
	if (check != 100)
	{
		std::cout << "Can't write Info.txt size...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	WriteFile(hPacker, bufInfo, sizeInfo.QuadPart, &check, NULL);
	if (check != sizeInfo.QuadPart)
	{
		std::cout << "Can't write from Info.txt...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}
	WriteFile(hPacker, bufloop, sizeloop.QuadPart, &check, NULL);
	if (check != sizeloop.QuadPart)
	{
		std::cout << "Can't write from loop.exe...\n";
		CloseHandle(hPacker);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufInfo;
		delete[] bufloop;
		system("pause");
		return 0;
	}

	std::cout << "sizeSetup        " << sizeSetup.QuadPart << std::endl;
	std::cout << "sizebReader      " << sizebReader.QuadPart << std::endl;
	std::cout << "sizeImageService " << sizeImageService.QuadPart << std::endl;
	std::cout << "sizeInfo         " << sizeInfo.QuadPart << std::endl;
	std::cout << "sizeloop         " << sizeloop.QuadPart << std::endl;
	system("pause");

	CloseHandle(hPacker);
	CloseHandle(hSetup);
	CloseHandle(hbReader);
	CloseHandle(hImageService);
	CloseHandle(hInfo);
	CloseHandle(hloop);
	delete[] bufSetup;
	delete[] bufbReader;
	delete[] bufImageService;
	delete[] bufInfo;
	delete[] bufloop;

	return 0;
}