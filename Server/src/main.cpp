#include <iostream>
#include <string>
#include "clServer.h"
using namespace std;


/*
 * сервер принимает строку с командой которая к нему пришла
 * парсит ее, и дает ответ
 */


int main()
{
	clServer Server;

	string sCommandLine;
	string sCommand;
	size_t pos = string::npos;

	cout << "Client application" << endl;
	while (sCommandLine != "exit")
	{
		cout << ">";
		getline(cin, sCommandLine);
		if (sCommandLine == "exit")
			break;

		try
		{
			/* preparations */
			pos = sCommandLine.find_first_of(' ');
			if (pos == string::npos)
				throw "Incorrect call of command";
			sCommand = sCommandLine.substr(0, pos); // extract command
			sCommandLine.erase(0, pos + 1); // erase command only


			if (sCommand == "set")
			{
				pos = sCommandLine.find_first_of(' ');
				if (pos == string::npos)
					throw "Incorrect call of command";

				string sTag = sCommandLine.substr(0, pos); // extract tag
				string sValue = sCommandLine.substr(pos + 1); // extract value

				Server.Set(sTag, sValue);
			}
			else if (sCommand == "get")
			{
				pos = sCommandLine.find_first_of(' ');
				if (pos != string::npos)
					throw "Incorrect call of command";

				string sTag = sCommandLine; // extract tag
				string sValue;

				Server.Get(sTag, sValue);
				cout << (sValue.size() ? sValue : "Tag not found") << endl;
			}
			else
				throw "Incorrect call of command";
		}
		catch (string & ex)
		{
			cerr << ex << endl;
		}
		catch (...)
		{
			cerr << "Unknown exception!" << endl;
			break;
		}
	} // while
}
