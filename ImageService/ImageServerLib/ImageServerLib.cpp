// ImageServerLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

bool LaunchProcess(const char* process_path, const char* arg)
{
	setlocale(LC_ALL, "Russian");

	//LaunchProcess("C:\\Users\\mi_ai\\source\\repos\\ServiceDebager\\x64\\Release\\ServiceDebager.exe", "ListenError");
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi = { };
	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
	if (CreateProcessA(process_path, (LPSTR)arg, NULL, NULL, FALSE, dwCreationFlags, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		return true;
	}
	else
		return false;
}

ImageServer::ImageServer(int port)
{
	sMain = new SOCKET;
	WsaData = new WSADATA;
	sin = new SOCKADDR_IN;
	message = new char[1024];
	connectionBase = new std::vector<connections>;

	WSAStartup(0x0101, WsaData);
	*sMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin->sin_family = AF_INET;
	sin->sin_port = htons(port);
	sin->sin_addr.s_addr = INADDR_ANY;

	bind(*sMain, (LPSOCKADDR)sin, sizeof(*sin));
	listen(*sMain, SOMAXCONN);

	CreateDirectoryA("imageLog", NULL);
	//LaunchProcess("C:\\Users\\mi_ai\\source\\repos\\ServiceDebager\\x64\\Release\\ServiceDebager.exe", "ListenError");
}
void ImageServer::clear()
{
	if (connectionBase)
	{
		for (int i = 0; i < connectionBase->size(); ++i)
			shutdown(connectionBase->data()[i].s, SD_BOTH);
		connectionBase->clear();
		delete connectionBase;
	}
	if (sMain)
	{
		shutdown(*sMain, SD_BOTH);
		delete sMain;
	}
	if (WsaData)
	{
		WSACleanup();
		delete WsaData;
	}
	if (sin)
		delete sin;
	if (message)
		delete[] message;
}
ImageServer::~ImageServer()
{
	clear();
}
bool ImageServer::recvLoop(int size, SOCKET& s)
{
	if (s == INVALID_SOCKET)
	{
		std::cout << "INVALID_SOCKET\n";
	}

	int check = 0;

	check = recv(s, message, size, MSG_WAITALL);

	return true;
}
SOCKET& ImageServer::GetSbyName(std::string& name)
{
	for (int i = 0; i < connectionBase->size();)
	{
		if ((*connectionBase)[i].name == name)
			return (*connectionBase)[i].s;
		++i;
	}
	SOCKET nu = 0;
	return nu;
}
void ImageServer::getImage(std::string& pc_name, bool show)
{
	//setlocale(LC_ALL, "Russian");

	SOCKET& s = GetSbyName(pc_name);

	if (!s)
		return;

	char cstart[100] = "start";

	send(s, cstart, 50, 0);

	if (!recvLoop(50, s))
		return;

	if (strcmp(cstart, message))
		return;

	if (!recvLoop(50, s))
		return;

	std::string name;
	std::string temp;
	int parts;
	int last = 0;
	DWORD size = 1024;

	int n = 0;
	while (message[n] != '\n')
		name += message[n++];
	++n;
	while (message[n] != '\n')
		temp += message[n++];
	parts = atoi(&temp[0]);
	temp.clear();
	while (message[n] != '\0')
		temp += message[n++];
	last = atoi(&temp[0]);

	////////////////////////////////
	char wBuff[500];
	SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);

	temp.clear();

	temp += wBuff;
	temp += "\\imageLog\\";
	CreateDirectoryA(&temp[0], NULL);
	temp += pc_name;
	CreateDirectoryA(&temp[0], NULL);
	temp += "\\" + name;
	std::cout << temp << std::endl;

	n = 0;

	HANDLE rFile = CreateFileA(&temp[0], GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	while (n < parts)
	{
		if (!recvLoop(size, s))
			return;
		WriteFile(rFile, message, size, NULL, NULL);
		++n;
	}

	if (!recvLoop(last, s))
		return;
	WriteFile(rFile, message, last, NULL, NULL);

	CloseHandle(rFile);
	/////////////////////////////////////////////////////
	if (show)
	{
		int ansver = (int)ShellExecuteA(NULL, "open", &temp[0], NULL, NULL, SW_SHOWNORMAL);

		//LaunchProcess("C:\\Users\\mi_ai\\source\\repos\\ServiceDebager\\x64\\Release\\ServiceDebager.exe", buf);
	}
}
bool ImageServer::getName()
{
	setlocale(LC_ALL, "Russian");

	int fromlen = sizeof(*sin);

	SOCKET s = accept(*sMain, (struct sockaddr*) sin, &fromlen);

	if (s == INVALID_SOCKET)
		return false;

	if (recv(s, message, 100, MSG_PEEK))
	{
		if (!strcmp(message, "##name##"))
		{
			recv(s, message, 100, 0);
			recv(s, message, 100, 0);
			connections newConnection(s, message);
			connectionBase->push_back(newConnection);
			message[0] = '\0';
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
void ImageServer::removeConnection(std::string& pc_name)
{
	setlocale(LC_ALL, "Russian");

	std::vector<connections>::iterator it = connectionBase->begin();
	for (int i = 0; i < connectionBase->size(); ++i)
	{
		if ((*it).name == pc_name)
		{
			closesocket((*it).s);
			connectionBase->erase(it);
		}
		++it;
	}
}
connections& ImageServer::GetLastCon()
{
	return (*connectionBase).back();
}
bool ImageServer::check(SOCKET& s)
{
	char buf[1];

	if (SOCKET_ERROR != send(s, buf, 0, 0))
		return true;
	else
		return false;
}
int ImageServer::delete_all_images(const std::string& refcstrRootDirectory, bool bDeleteSubdirectories)
{
	setlocale(LC_ALL, "Russian");

	bool            bSubdirectory = false;       // Flag, indicating whether
												 // subdirectories have been found
	HANDLE          hFile;                       // Handle to directory
	std::string     strFilePath;                 // Filepath
	std::string     strPattern;                  // Pattern
	WIN32_FIND_DATAA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = ::FindFirstFileA(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FileInformation.cFileName[0] != '.')
			{
				strFilePath.erase();
				strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (bDeleteSubdirectories)
					{
						// Delete subdirectory
						int iRC = delete_all_images(strFilePath, bDeleteSubdirectories);
						if (iRC)
							return iRC;
					}
					else
						bSubdirectory = true;
				}
				else
				{
					// Set file attributes
					if (::SetFileAttributesA(strFilePath.c_str(),
						FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();

					// Delete file
					if (::DeleteFileA(strFilePath.c_str()) == FALSE)
						return ::GetLastError();
				}
			}
		} while (::FindNextFileA(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

		DWORD dwError = ::GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else
		{
			if (!bSubdirectory)
			{
				// Set directory attributes
				if (::SetFileAttributesA(refcstrRootDirectory.c_str(),
					FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();

				// Delete directory
				if (::RemoveDirectoryA(refcstrRootDirectory.c_str()) == FALSE)
					return ::GetLastError();
			}
		}
	}

	return 0;
}

