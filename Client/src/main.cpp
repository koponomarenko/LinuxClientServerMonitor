#include "clClient.h"
#include <iostream>
#include <string>

int main()
{
	try
	{
		clClient Client;
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
