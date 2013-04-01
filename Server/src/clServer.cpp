/*
 * clServer.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#include "clServer.h"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <unistd.h> // for close/unlink function
#include <algorithm>

#include <sys/types.h> // for socket
#include <sys/socket.h> // for socket

using namespace std;

string clServer::sBase_ = "base.xml";
string clServer::sTmp_ = "~base.xml";
unsigned char clServer::PingBuf = 1;



clServer::clServer() : ListenerSock(0)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

clServer::~clServer()
{
	CloseHostSocket();
}


void clServer::Start()
{
	CheckBaseFile();
	OpenHostSocket();

	int ClientSock;
	int nBytes;
	unsigned char ConnectType;
	while (1)
	{
		ClientSock = accept(ListenerSock, NULL, NULL);
		if (ClientSock < 0)
			continue;

		nBytes = recv(ClientSock, &ConnectType, sizeof(ConnectType), 0);
		if(nBytes <= 0) // error/closed connection
		{
			close(ClientSock);
			continue;
		}

		switch (ConnectType)
		{
		case 0: // Client
			StartClientConnection(ClientSock);
			break;

		case 1: // Monitor
			send(ClientSock, &PingBuf, sizeof(PingBuf), 0); // send response to monitor
			Monitors.insert(ClientSock);
			break;
		}
	}

	CloseHostSocket();
}


void clServer::StartClientConnection(int ClientSock) // this part of code can be new thread
{
	int nBytes;
	const short kBufSize = 512;
	char buf[kBufSize];
	string sAcceptedLine;
	string sResult;
	bool bIsResult;
	bool bExit = false;


	while (!bExit)
	{
		sAcceptedLine.clear();
		sResult.clear();

		do
		{
			nBytes = recv(ClientSock, buf, kBufSize, 0);
			if(nBytes <= 0) // error/closed connection
			{
				bExit = true;
				break;
			}
			sAcceptedLine.append(buf, nBytes);
		} while (nBytes == kBufSize);

		if (bExit) break;

		try
		{
			ParseAndDoCommand(sAcceptedLine, sResult);
		}
		catch (string & ex) // TODO Close All sockets and restart server
		{
			sResult = ex;
			bExit = true;
		}

		if (sResult.size())
		{
			bIsResult = true;
			send(ClientSock, &bIsResult, 1, 0);
			send(ClientSock, sResult.data(), sResult.size() , 0);
		}
		else
		{
			bIsResult = false;
			send(ClientSock, &bIsResult, 1, 0);
		}

		// TODO this two string only for temporary work with one client!
			close(ClientSock);
			return;
	}

	close(ClientSock);
}


void clServer::ParseAndDoCommand(string & sCommandLine, string & sResult)
{
	string sCommand;
	size_t pos = string::npos;

	// preparations
	pos = sCommandLine.find_first_of(' ');
	if (pos != string::npos)
	{
		sCommand = sCommandLine.substr(0, pos); // extract command
		sCommandLine.erase(0, pos + 1); // erase command only
	}
	else
		sCommand = sCommandLine;


	if (sCommand == "set")
	{
		pos = sCommandLine.find_first_of(' ');
		if (pos == string::npos)
		{
			sResult = "Incorrect call of command";
			return;
		}

		string sTag = sCommandLine.substr(0, pos); // extract tag
		string sValue = sCommandLine.substr(pos + 1); // extract value

		Set(sTag, sValue);

		string sToMonitors ("The Server has set tag: " + sTag + ", with value: " + sValue);
		for (set<int>::iterator it = Monitors.begin(); it != Monitors.end(); ++it)
			send(*it, sToMonitors.data(), sToMonitors.size(), 0); // TODO modification: need to accept a respond from Monitor
	}
	else if (sCommand == "get")
	{
		if (sCommandLine.find_first_of(' ') != string::npos)
		{
			sResult = "Incorrect call of command";
			return;
		}

		string sTag = sCommandLine; // extract tag
		string sValue;

		Get(sTag, sValue);
		sResult = sValue.size() ? sValue : "Tag not found";
	}
	else
		sResult = "Incorrect call of command";
}



void clServer::OpenHostSocket()
{
	ListenerSock = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenerSock < 0)
		throw string("socket failed");

	if (bind(ListenerSock, (sockaddr*) &addr, sizeof(addr)) < 0)
		throw string("bind failed");

	listen(ListenerSock, 1);
}



void clServer::CloseHostSocket()
{
	close(ListenerSock);
}



void clServer::CheckBaseFile()
{
	fstream f(sBase_.c_str(), ios_base::in); // try to open file for input
	if (!f.is_open())
	{
		f.close();
		f.open(sBase_.c_str(), ios_base::out); // try to create a file
		if (!f.is_open())
			throw string("Error while creating file \"base.xml\"");
	}

	f.close();
}



void clServer::Set(const std::string & tag, const std::string & val)
{
	ifstream fin(sBase_.c_str(), ios_base::in);
	if (!fin.is_open())
		throw string("Error while opening file \"base.xml\" for reading");

	ofstream fout(sTmp_.c_str(), ios_base::out | ios_base::trunc);

	string buf;
	string buf_val;
	bool bReplaced = false;

	while (fin && fin.peek() != EOF)
	{
		if (fin.peek() == '\n')
		{
			fin.get();
			continue;
		}

		getline(fin, buf, '<');
		fout << buf << '<';

		getline(fin, buf, '>');
		fout << buf << '>';

		getline(fin, buf_val, '<');
		// ----- temporary decision -------
		if (fin.get() != '/')
		{
			fout.close();
			remove(sTmp_.c_str());
			throw string("Invalid format");
		}
		// ----- ------------------ -------
		if (buf == tag)
		{
			fout << val << "</";
			bReplaced = true;
		}
		else
			fout << buf_val << "</";

		getline(fin, buf, '>');
		fout << buf << '>';

		fout << endl;
	}

	if (!bReplaced)
		fout << '<' << tag << '>' << val << "</" << tag << '>' << endl;

	fin.close();
	fout.close();
	remove(sBase_.c_str());
	rename(sTmp_.c_str(), sBase_.c_str());
}



void clServer::Get(const std::string & tag, std::string & val) const
{
	ifstream fin(sBase_.c_str(), ios_base::in);
	if (!fin.is_open())
		throw string("Error while opening file \"base.xml\" for reading");


	string buf;
	string buf_val;
	while (fin && fin.peek() != EOF)
	{
		if (fin.peek() == '\n')
		{
			fin.get();
			continue;
		}

		getline(fin, buf, '<');

		getline(fin, buf, '>');

		getline(fin, buf_val, '<');
		// ----- temporary decision -------
		if (fin.get() != '/')
			throw string("Invalid format");
		// ----- ------------------ -------
		if (buf == tag)
		{
			val = buf_val;
			break;
		}

		getline(fin, buf, '>');
	}

	fin.close();
}
