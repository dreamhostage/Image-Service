#pragma once
#include "pch.h"

struct connections
{
	SOCKET s;
	std::string name;
	connections(SOCKET& s, char* name)
	{
		this->s = s;
		this->name = name;
	}
};

class ImageServer
{
	SOCKET* sMain;
	WSADATA* WsaData;
	SOCKADDR_IN* sin;
	char* message;

public:
	std::vector<connections>* connectionBase;
	ImageServer(int port);
	void clear();
	~ImageServer();
	bool recvLoop(int size, SOCKET& s);
	SOCKET& GetSbyName(std::string& name);
	void getImage(std::string& pc_name, bool show);
	bool getName();
	void removeConnection(std::string& pc_name);
	connections& GetLastCon();
	bool check(SOCKET& s);
	int delete_all_images(const std::string& refcstrRootDirectory, bool bDeleteSubdirectories = true);
};