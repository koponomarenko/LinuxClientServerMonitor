/*
 * clClient.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#include "clClient.h"
#include <cstring>
#include <iostream>

using namespace std;


clClient::clClient(const std::string & sServSockName) : sServSockName_(sServSockName), Sock(0)
{
}

clClient::~clClient()
{
}

void clClient::Start()
{
	char buf[BUF_SIZE];

	cout << "Client application" << endl;
	cout << "use --help for list command" << endl;

	OpenSocket();

	string sCommandLine;

	while (true)
	{
		cout << ">";
		getline(cin, sCommandLine);
		if (sCommandLine == "exit")
			break;

		sendto(Sock, sCommandLine.c_str(), sCommandLine.size(), 0, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family));

		// Receive from
	}

	CloseSocket();
}


void clClient::OpenSocket()
{
	Sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (Sock < 0)
		throw string("socket failed");

	srvr_name.sa_family = AF_UNIX;
	strcpy(srvr_name.sa_data, sServSockName_.c_str()); // 13 symbols max?
}

void clClient::CloseSocket()
{
	// TODO: Do I need unlink socket?
}
