/*
 * clServer.h
 *
 *  Created on: Mar 18, 2013
 *      Author: konstantin
 */

#ifndef CLSERVER_H_
#define CLSERVER_H_

#include <string>

class clServer {
private:
	static std::string sBase_;
	static std::string sTmp_;

public:

	void Set(const std::string & tag, const std::string & val);
	void Get(const std::string & tag, std::string & val) const;
};

#endif /* CLSERVER_H_ */
