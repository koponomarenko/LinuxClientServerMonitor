/*
 * clClient.h
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#ifndef CLCLIENT_H_
#define CLCLIENT_H_

#include <string>
#include <sys/types.h> // for socket
#include <sys/socket.h> // for socket

class clClient
{
public:
	enum { BUF_SIZE  = 512 };

private:
	std::string sServSockName_;
	int Sock;
	sockaddr srvr_name;

public:
	clClient(const std::string & sServSockName);
	virtual ~clClient();

	void Start();

private:
	void OpenSocket();
	void CloseSocket();
};

#endif /* CLCLIENT_H_ */
