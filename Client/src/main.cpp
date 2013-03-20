#include "clClient.h"
#include <iostream>
#include <string>

int main()
{
	try
	{
		clClient Client("socket.soc");
		Client.Start();
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
