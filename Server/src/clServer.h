/*
 * clServer.h
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#ifndef CLSERVER_H_
#define CLSERVER_H_

#include <string>
#include <sys/types.h> // for socket
#include <sys/socket.h> // for socket
#include <netinet/in.h>


class clServer
{
private:
	static std::string sBase_;
	static std::string sTmp_;

	int ListenerSock;
	sockaddr_in addr;

public:
	clServer();
	virtual ~clServer();

	void Start();

private:
	void StartNewConnection(int ClientSock);

	void ParseAndDoCommand(std::string & sCommandLine, std::string & sResult);

	void OpenHostSocket();
	void CloseHostSocket();
	void CheckBaseFile();

	void Set(const std::string & tag, const std::string & val);
	void Get(const std::string & tag, std::string & val) const;
};

#endif /* CLSERVER_H_ */
