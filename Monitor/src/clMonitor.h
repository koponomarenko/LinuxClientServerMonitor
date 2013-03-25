/*
 * clMonitor.h
 *
 *  Created on: Mar 25, 2013
 *      Author: konstantin
 */

#ifndef CLMONITOR_H_
#define CLMONITOR_H_

#include <string>
#include <sys/types.h> // for socket
#include <sys/socket.h> // for socket
#include <netinet/in.h>

class clMonitor
{
private:
	int Sock;
	sockaddr_in addr;

public:
	clMonitor();
	virtual ~clMonitor();

	void Start();

private:
	void OpenSocket();
	void CloseSocket();
};

#endif /* CLMONITOR_H_ */
