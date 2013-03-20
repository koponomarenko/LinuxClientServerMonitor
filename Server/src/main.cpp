#include "clServer.h"
#include <iostream>
#include <string>

int main()
{
	try
	{
		clServer Server("socket.soc");
		Server.Start();
	}
	catch (std::string & ex)
	{
		std::cerr << "Error: " << ex << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception!" << std::endl;
	}
}
