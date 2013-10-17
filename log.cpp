/*
 * File:   Log_t.cpp
 * Author: dis
 *
 * Created on August 31, 2012, 8:58 PM
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <zlib.h>
#include <sstream>
#include <fstream>
#include "log.h"
#include "utils.h"
#include "config.h"

namespace nn
{
namespace Log
{
	namespace //private variables, only namespace Log has access
	{
		std::ostringstream Logstr;
		bool Enabled = true;
		str Var = "", Realnick = "";
		uint LogNum = 0;
		str Logfile = "";
	}

	void Begin(constr vart, constr realnickt, uint lognum)
	{
		SetVars(vart, realnickt, lognum);
		check();
	}

	void SetVars(constr vart, constr realnickt, uint lognum)
	{
		Var = vart;
		Realnick = realnickt;
		LogNum = lognum;
		Logfile = Util::GetFilePath(Info::Name(), DirExt::DLog());
	}

	int append(str text)
	{
		//if(var == "") return 1;

		if(Enabled == false) return 2;

		Logstr << text << NL(); //no need to use endl, no need to flush

		return 0;
	}

	int writeout(void)
	{
		if(Enabled == false) return 1;

		constr date = Util::GetDate();

		constr newstr = Logstr.str().erase(Logstr.str().size() - NL().size()); //remove extra newline

		Util::AppendText(Logfile, date + newstr);

		return 0;
	}

	int check(void)
	{
		if(Var.find_first_of(Var::DontLog()) != std::string::npos)
		{
			Enabled = false;
			return 1;
		}

		Enabled = true;

		const saynum check = Util::CheckFile(Logfile.c_str());

		if(check == SayNum::FileNotExist)
		{
			BeginOutput();
		}

		struct stat logsize;
		stat(Logfile.c_str(), &logsize);

//		if(logsize.st_size < DValue::MaxLogSize)
//		{
//			return 1;
//		}

		//rotate and compress
		Rotate();
		Compress();

		//update config file
		config cfg;
		cfg.lognum = cfg.lognum + 1;
		cfg.Update();

		BeginOutput();

		return 0;
	}

	int Rotate(void)
	{
		do
		{
			rename(str(Logfile + "." + Util::itoa(LogNum) + ".gz").c_str(), str(Logfile + "." + Util::itoa(LogNum+1) + ".gz").c_str());
		}
		while(LogNum-- > 1);

		return 0;
	}

	int Compress(void)
	{
			std::ifstream lfile(Logfile);

			if(! lfile.is_open()) Util::Error(ErrorNum::NoFileOpen, Logfile, __func__);

			lfile.seekg(0, std::ios::end);
			int length = lfile.tellg();
			lfile.seekg(0, std::ios::beg);

			char* buf = new char [length];
			lfile.read(buf, length);
			lfile.close();

			constr nlfile = Logfile + ".1.gz";

			gzFile file = gzopen(nlfile.c_str(), "wb");

			if(file == NULL)
			{
				Util::Say("Error: file null", Color::Red(), true);
				return -1;
			}

			if(gzputs(file, buf) != length)
			{
				Util::Say("Error: gzputs failed", Color::Red(), true);
				return -2;
			}

			gzclose(file);

			delete[] buf;

		return 0;
	}

	int BeginOutput(void)
	{
		std::ofstream file;

		file.open(Logfile.c_str());

		if(! file.is_open()) return Util::Error(ErrorNum::NoFileOpen, Logfile, __func__);

		file << Util::GetDate() << NL();
		file << "Log File First Created" << NL();
		file.close();

		return 0;
	}

	} //namespace Log
} //namespace nn
