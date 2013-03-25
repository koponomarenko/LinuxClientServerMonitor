#include "clMonitor.h"
#include <iostream>
#include <string>

int main()
{
	try
	{
		clMonitor Monitor;
		Monitor.Start();
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
