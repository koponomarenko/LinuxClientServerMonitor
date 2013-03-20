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


class clServer
{
private:
	static std::string sBase_;
	static std::string sTmp_;

	std::string sServSockName_;
	int Sock;
	sockaddr srvr_name;

public:
	clServer(const std::string & sServSockName);
	virtual ~clServer();

	void Start();

private:
	void ParseAndDoCommand(std::string & sCommandLine);

	void Set(const std::string & tag, const std::string & val);
	void Get(const std::string & tag, std::string & val) const;

	void OpenSocket();
	void CloseSocket();
};

#endif /* CLSERVER_H_ */
