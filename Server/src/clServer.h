/*
 * clServer.h
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#ifndef CLSERVER_H_
#define CLSERVER_H_

#include <string>
#include <set>
#include <netinet/in.h>

class clServer
{
private:
	static std::string sBase_;
	static std::string sTmp_;
	static unsigned char PingBuf;

	int ListenerSock;
	sockaddr_in addr;

	std::set<int> Monitors;

public:
	clServer();
	virtual ~clServer();

	void Start();

private:
	void StartClientConnection(int ClientSock);

	void ParseAndDoCommand(std::string & sCommandLine, std::string & sResult);

	void OpenHostSocket();
	void CloseHostSocket();
	void CheckBaseFile();

	void Set(const std::string & tag, const std::string & val);
	void Get(const std::string & tag, std::string & val) const;
};

#endif /* CLSERVER_H_ */
