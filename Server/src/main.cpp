#include "clServer.h"
#include <iostream>
#include <fstream>
#include <string>

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PID_FILE "/var/run/Server.pid"

void RunDemon();

int main()
{
//	while (1)
//	{
//		try
//		{
//			clServer Server;
//			Server.Start();
//			std::cerr << "Server stopped" << std::endl;
//			break;
//		}
//		catch (std::strinlg & ex)
//		{
//			std::cerr << "Error: " << ex << std::endl;
//		}
//		catch (...)
//		{
//			std::cerr << "Unknown exception!" << std::endl;
//		}
//		std::cerr << "Server restarted" << std::endl;
//	}
//	return 0;

	pid_t pid;

	pid = fork();
	switch(pid)
	{
	case 0: // in new thread
		RunDemon();
		break;

	case -1:
		// write to log "Fail: unable to fork"
		break;

	default: // in parent thread
		// write to log "Demon is created"
		break;
	}
}


void RunDemon()
{
	umask(0); /* Change the file mode mask */

	std::ofstream flog("/var/log/MyServer.log"); // open log file

	flog << "Server started" << std::endl;

	/* Create a new SID for the child process */
	pid_t sid = setsid();
	if (sid < 0)
	{
		flog << "ERROR: Create a new SID for the child process" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0)
	{
		flog << "ERROR: Change the current working directory" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	/* Since a daemon cannot use the terminal, these file descriptors are redundant and a potential security hazard. */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);


	/* Daemon-specific initialization goes here */

	/* Save pid to file for daemon script */
	std::ofstream fpid(PID_FILE);
	if (!fpid.is_open())
	{
		flog << "ERROR: Can't write pid to \"/var/run/Server.pid\"" << std::endl;
		exit(EXIT_FAILURE);
	}
	fpid << getpid();
	fpid.close();


	/* The Big Loop */
	while (1)
	{
		try
		{
			clServer Server;
			Server.Start();
			flog << "Server stopped" << std::endl;
			break;
		}
		catch (std::string & ex)
		{
			flog << "Error: " << ex << std::endl;
		}
		catch (...)
		{
			flog << "Unknown exception!" << std::endl;
		}
		flog << "Server restarted" << std::endl;
	}

	remove(PID_FILE);
}

