/*
 * clClient.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#include "clClient.h"
#include <cstring>
#include <iostream>
#include <unistd.h> // for close/unlink function

using namespace std;


clClient::clClient() : Sock(0)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

clClient::~clClient()
{
	CloseSocket();
}


static int sendall(int s, const char * buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);
}


void clClient::Start()
{
	unsigned char ConnectType = 0; // Client type (need for server)
	const short kBufSize = 512;

	bool bIsResult;
	bool bExit = false;
	char buf[kBufSize];
	int nBytes;
	string sCommandLine;
	string sResult;


//	OpenSocket();

	cout << "Client working." << endl;
	cout << "use \"exit\" to exit" << endl;

//	send(Sock, &ConnectType, sizeof(ConnectType), 0); // Introduce to server

	while (!bExit)
	{
		sCommandLine.clear();
		sResult.clear();

		cout << ">";
		getline(cin, sCommandLine);
		if (sCommandLine == "exit")
			break;

		OpenSocket();
		send(Sock, &ConnectType, sizeof(ConnectType), 0); // Introduce to server
		sendall(Sock, sCommandLine.data(), sCommandLine.size(), 0); // my helpful own function

		recv(Sock, &bIsResult, 1, 0);
		if (bIsResult)
		{
			do
			{
				nBytes = recv(Sock, buf, kBufSize, 0);
				if(nBytes <= 0) // error/closed connection
				{
					cout << "connect lost" << endl;
					bExit = true;
					break;
				}
				sResult.append(buf, nBytes);
			} while (nBytes == kBufSize);

			if (bExit) break;

			cout << sResult << endl;
		}
		CloseSocket(); // tmp decision
	}

	CloseSocket();
}


void clClient::OpenSocket()
{
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock < 0)
		throw string("socket failed");

	if(connect(Sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		throw string("connect failed");
}



void clClient::CloseSocket()
{
	close(Sock);
}
