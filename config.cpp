/*
 * config.cpp
 *
 *  Created on: Oct 11, 2012 2:09:16 AM
 *  Author: dis
 */

#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include "config.h"
#include "utils.h"

namespace nn
{

bool config::ranonce = false;
str config::confile = "";
uint config::lognum = Config::LogDValue;

config::config()
{
	if(ranonce)
		return;

	//confile cannot be static const b/c debug is not checked yet
	confile = Util::GetFilePath(Info::Name(), DirExt::DConfig()); // /userdir/nn.config

	//Default Values
	//lognum = Config::LogDValue;

	config::Check();
	config::Parse();

	ranonce = true;
}

int config::Check(void)
{
	saynum rtn = Util::CheckFile(confile.c_str());

	if(rtn == SayNum::FileExist)
		return 1; //return if config file exist

	//else create config file
	config::CreateDefault();

	return 0;
}

int config::CreateDefault(void)
{
	std::ofstream cfile(confile);

	if(! cfile.is_open()) Util::Error(ErrorNum::NoFileOpen, confile, __func__);

	cfile << Config::LogNum << Delim::MainSp() << Config::LogDValue;

	cfile.close();

	return 0;
}

int config::Parse(void)
{
	std::ifstream cfile(confile.c_str());

	if(! cfile.is_open()) Util::Error(ErrorNum::NoFileOpen, confile, __func__);

	str line = "";
	std::regex rxlognum("(" + Config::LogNum + Delim::MainSp() + ")(.*)");
	std::smatch rxm;

	while(getline(cfile, line))
	{
		if(std::regex_match(line, rxm, rxlognum))
		{
			lognum = atoi(str(rxm[2]).c_str());
			continue;
		}
	}

	cfile.close();

	return 0;
}

int config::Update(void)
{
	std::ofstream cfile(confile);

	if(! cfile.is_open()) Util::Error(ErrorNum::NoFileOpen, confile, __func__);

	cfile << Config::LogNum << Delim::MainSp() << lognum;

	cfile.close();

	return 0;
}

config::~config()
{
	// TODO Auto-generated destructor stub
}

} /* namespace nn */
