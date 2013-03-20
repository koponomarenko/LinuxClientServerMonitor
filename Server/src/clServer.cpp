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
#include <sstream>
#include <errno.h>

#include <unistd.h> // for close/unlink function

using namespace std;

string clServer::sBase_ = "base.xml";
string clServer::sTmp_ = "~base.xml";



clServer::clServer(const std::string & sServSockName) : sServSockName_(sServSockName), Sock(0)
{
}

clServer::~clServer()
{
}


void clServer::Start()
{
	int nBytes;
	char buf[512] = {0};
	string sAcceptedLine;

	OpenSocket();

	while (1)
	{
		sAcceptedLine.clear();
		do
		{
			nBytes = recvfrom(Sock, buf, sizeof(buf) - 1,  0, 0, 0);
			if (nBytes < 0)
				throw string("recvfrom failed");

			buf[nBytes] = 0;
			sAcceptedLine += buf;
		} while (nBytes != 0);

		ParseAndDoCommand(sAcceptedLine);
	}

	CloseSocket();
}


void clServer::ParseAndDoCommand(string & sCommandLine)
{
	string sCommand;
	size_t pos = string::npos;


	/* preparations */
	pos = sCommandLine.find_first_of(' ');
	if (pos == string::npos)
		throw string("Incorrect call of command");
	sCommand = sCommandLine.substr(0, pos); // extract command
	sCommandLine.erase(0, pos + 1); // erase command only


	if (sCommand == "set")
	{
		pos = sCommandLine.find_first_of(' ');
		if (pos == string::npos)
			throw string("Incorrect call of command");

		string sTag = sCommandLine.substr(0, pos); // extract tag
		string sValue = sCommandLine.substr(pos + 1); // extract value

		Set(sTag, sValue);
	}
	else if (sCommand == "get")
	{
		pos = sCommandLine.find_first_of(' ');
		if (pos != string::npos)
			throw string("Incorrect call of command");

		string sTag = sCommandLine; // extract tag
		string sValue;

		Get(sTag, sValue);
		//cout << (sValue.size() ? sValue : "Tag not found") << endl;
	}
	else
		throw string("Incorrect call of command");
}


void clServer::Set(const std::string & tag, const std::string & val)
{
	fstream fin(sBase_.c_str(), ios_base::in); // open file for input
	if (!fin.is_open())
	{
		// try to create a file
		fin.close();
		fin.open(sBase_.c_str(), ios_base::out);
		if (!fin.is_open())
			throw string("Error while opening file \"base.xml\" for writing");
		fin << '<' << tag << '>' << val << "</" << tag << '>' << endl;
		return;
	}


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
	ifstream fin;

	fin.open(sBase_.c_str(), ios_base::in);
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
}


void clServer::OpenSocket()
{
	Sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (Sock < 0)
		throw string("socket failed");

	srvr_name.sa_family = AF_UNIX;
	strcpy(srvr_name.sa_data, sServSockName_.c_str()); // 13 symbols max?

	if (bind(Sock, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family)) < 0)
	{
		stringstream ss;
		ss << errno;
		throw string("bind failed. Err numb: ") + ss.str();
	}
}



void clServer::CloseSocket()
{
	close(Sock);
	unlink(sServSockName_.c_str());
}
