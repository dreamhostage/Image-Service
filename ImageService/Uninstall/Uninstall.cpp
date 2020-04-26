#include <iostream>
#include <Windows.h>

int main()
{
	// Stop process:

	setlocale(LC_ALL, "Russian");

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	std::string com = "C:\\Windows\\System32\\ImageService.exe stop";
	if (CreateProcessA(nullptr, &com[0], nullptr, nullptr, FALSE, NULL, nullptr, nullptr, &si, &pi))
		WaitForSingleObject(pi.hProcess, INFINITE);
	else
	{
		std::cout << "Cant launch ImageService.exe to stop...\n";
		system("pause");
		return 0;
	}

	// Delete files:

	STARTUPINFOA si2 = { sizeof(si) };
	PROCESS_INFORMATION pi2;
	com = "C:\\Windows\\System32\\ImageService.exe remove";
	if (CreateProcessA(nullptr, &com[0], nullptr, nullptr, FALSE, NULL, nullptr, nullptr, &si2, &pi2))
		WaitForSingleObject(pi2.hProcess, INFINITE);
	else
	{
		std::cout << "Cant launch ImageService.exe to remove...\n";
		system("pause");
		return 0;
	}

	HANDLE wsFile = CreateFileA("C:\\Windows\\System32\\loop.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE wiFile = CreateFileA("C:\\Windows\\System32\\ImageService.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE winfFile = CreateFileA("C:\\Windows\\System32\\imageConf.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
	HANDLE winfFile3 = CreateFileA("C:\\Windows\\System32\\bReader.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);

	if (wsFile == INVALID_HANDLE_VALUE || wiFile == INVALID_HANDLE_VALUE || winfFile == INVALID_HANDLE_VALUE || winfFile3 == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open files:\n";

		if (wsFile == INVALID_HANDLE_VALUE)
			std::cout << "loop.exe;\n";
		if (wiFile == INVALID_HANDLE_VALUE)
			std::cout << "ImageService.exe;\n";
		if (winfFile == INVALID_HANDLE_VALUE)
			std::cout << "imageConf.exe;\n";
		if (winfFile3 == INVALID_HANDLE_VALUE)
			std::cout << "bReader.exe;\n";

		CloseHandle(wsFile);
		CloseHandle(wiFile);
		CloseHandle(winfFile);
		CloseHandle(winfFile3);
		system("pause");
		return 0;
	}

	CloseHandle(wsFile);
	CloseHandle(wiFile);
	CloseHandle(winfFile);
	CloseHandle(winfFile3);
}