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
#include <netinet/in.h>

class clClient
{
private:
	int Sock;
	sockaddr_in addr;

public:
	clClient();
	virtual ~clClient();

	void Start();

private:
	void OpenSocket();
	void CloseSocket();
};

#endif /* CLCLIENT_H_ */
