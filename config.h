/*
 * config.h
 *
 *  Created on: Oct 11, 2012 2:09:16 AM
 *  Author: dis
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "vars.h"

namespace nn
{

class config
{
	config& operator=(const config) = delete; //disables copying
	config(const config&) = delete; //disables default constructor

	static bool ranonce;

public:
	static str confile; //configure file
	static uint lognum;

	config();
	virtual ~config();

	int Check(void);
	int CreateDefault(void);
	int Parse(void);
	int Update(void);
};

namespace Config
{
constr LogNum = "LogNum";
const uint LogDValue = 0;
}

} /* namespace nn */
#endif /* CONFIG_H_ */
