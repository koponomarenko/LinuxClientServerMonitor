/*
 * clMonitor.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: konstantin
 */

#include "clMonitor.h"
#include <cstring>
#include <iostream>
#include <unistd.h> // for close/unlink function

using namespace std;


clMonitor::clMonitor() : Sock(0)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}


clMonitor::~clMonitor()
{
	CloseSocket();
}



void clMonitor::Start()
{
	unsigned char ConnectType = 1; // Monitor type (need for server)
	unsigned char PingBuf;
	int nBytes;

	OpenSocket();

	cout << "Monitor working." << endl;

	send(Sock, &ConnectType, sizeof(ConnectType), 0); // Introduce to server
	nBytes = recv(Sock, &PingBuf, sizeof(PingBuf), 0); // check, is server working ?..
	if(nBytes <= 0) // error/closed connection
		throw string("server does not respond");

	while (1)
	{
		nBytes = recv(Sock, &PingBuf, sizeof(PingBuf), 0);
		if(nBytes <= 0) // error/closed connection
			throw string("connect lost");

		cout << "Server has received something." << endl;
	}

	CloseSocket();
}



void clMonitor::OpenSocket()
{
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock < 0)
		throw string("socket failed");

	if(connect(Sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		throw string("connect failed");
}



void clMonitor::CloseSocket()
{
	close(Sock);
}

