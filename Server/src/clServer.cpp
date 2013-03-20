/*
 * clServer.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#include "clServer.h"
#include <fstream>
#include <cstdio>

using namespace std;

string clServer::sBase_ = "base.xml";
string clServer::sTmp_ = "~base.xml";



void clServer::Set(const std::string & tag, const std::string & val)
{
	fstream fin(sBase_.c_str(), ios_base::in); // open file for input
	if (!fin.is_open())
	{
		// try to create a file
		fin.close();
		fin.open(sBase_.c_str(), ios_base::out);
		if (!fin.is_open())
			throw "Error while opening file \"base.xml\" for writing";
		fin << '<' << tag << '>' << val << "</" << tag << '>' << endl;
		return;
	}


	ofstream fout(sTmp_.c_str(), ios_base::out | ios_base::trunc);

	string buf;
	string buf_val;
	bool bReplaced = false;
	while (fin && fin.peek() != EOF)
	{
		if (fin.peek() == '\n')
		{
			fin.get();
			continue;
		}

		getline(fin, buf, '<');
		fout << buf << '<';

		getline(fin, buf, '>');
		fout << buf << '>';

		getline(fin, buf_val, '<');
		// ----- temporary decision -------
		if (fin.get() != '/')
		{
			fout.close();
			remove(sTmp_.c_str());
			throw "Invalid format";
		}
		// ----- ------------------ -------
		if (buf == tag)
		{
			fout << val << "</";
			bReplaced = true;
		}
		else
			fout << buf_val << "</";

		getline(fin, buf, '>');
		fout << buf << '>';

		fout << endl;
	}

	if (!bReplaced)
		fout << '<' << tag << '>' << val << "</" << tag << '>' << endl;

	fin.close();
	fout.close();
	remove(sBase_.c_str());
	rename(sTmp_.c_str(), sBase_.c_str());
}



void clServer::Get(const std::string & tag, std::string & val) const
{
	ifstream fin;

	fin.open(sBase_.c_str(), ios_base::in);
	if (!fin.is_open())
		throw "Error while opening file \"base.xml\" for reading";


	string buf;
	string buf_val;
	while (fin && fin.peek() != EOF)
	{
		if (fin.peek() == '\n')
		{
			fin.get();
			continue;
		}

		getline(fin, buf, '<');

		getline(fin, buf, '>');

		getline(fin, buf_val, '<');
		// ----- temporary decision -------
		if (fin.get() != '/')
			throw "Invalid format";
		// ----- ------------------ -------
		if (buf == tag)
		{
			val = buf_val;
			break;
		}

		getline(fin, buf, '>');
	}
}
