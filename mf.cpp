/*
 * File:   mf.cpp
 * Author: dis
 *
 * Created on August 31, 2012, 10:16 PM
 */

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <regex>
#include "log.h"
#include "utils.h"
#include "mf.h"
#include "config.h"

namespace nn
{
namespace mf
{
	namespace
	{
		bool debug = false;
		str var = "", name = "", endtext = "", realnick = "", realonick = "",
				svrch = "", rnotefile = "", rnotefileout = "", rpointfile = "", rpointfileout = "";
		const vlevel prog_vlevel = VLevel::xDefault;
		constr pwd = Util::GetDir();
		constr nickfile = Util::GetFilePath(Info::Name(), DirExt::DNicks()); //nn.nicks
		constr notefile = Util::GetFilePath(name, DirExt::DNotes());
		constr pointfile = Util::GetFilePath(name, DirExt::DPoints());
		constr notefileout = Util::GetFilePathOut(name, DirExt::DNotes());
		constr pointfileout = Util::GetFilePathOut(name, DirExt::DPoints());
	}

	void Begin(void)
	{
		SetVars();
		SetRVars();
		config cvars;
		Log::Begin(var, realnick, cvars.lognum);
		BeginOutput();
		MainCheck();
		FirstCheck(rnotefile, NP::notes);
		//start main program
		ParseOpts();
	}

	void SetVars(void)
	{
		gvar vars;
		Util::GetVars(vars);

		svrch = vars.svrch;
		name = vars.name;
		var = vars.var;
		endtext = vars.endtext;
		debug = vars.debug;
	}

	void SetRVars(void)
	{
		realnick = name; //nicks.getrealnick

		rnotefile = Util::GetFilePath(realnick, DirExt::DNotes());
		rpointfile = Util::GetFilePath(realnick, DirExt::DPoints());
		rnotefileout = Util::GetFilePathOut(realnick, DirExt::DNotes());
		rpointfileout = Util::GetFilePathOut(realnick, DirExt::DPoints());
	}

	void BeginOutput(void)
	{
		str say = "Begin Output: ";
		Util::Say(say, Color::Blue());
		Log::append(say);

		say = "Server and Channel: " + svrch;
		Util::Say(say);
		Log::append(say);

		say = "Command: " + Util::Underline(name);
		if(var != "")
			say += Util::Underline(" " + var);
		if(endtext != "")
			say += Util::Underline(" " + endtext);
		Util::Say(say, Color::Green());
		Log::append(say);

		say = "Real Nick: " + Util::Bold(realnick);
		Util::Say(say, Color::LBlue());
		Log::append(say);
	}

	constr OutFileTxt(constr file)
	{
		if(file == rnotefile)
			return rnotefileout;
		else if(file == rpointfile)
			return rpointfileout;
		else if(file == notefile)
			return notefileout;
		else if(file == pointfile)
			return pointfileout;
		else
			return "0";

	}

	int CheckLastDate(constr file)
	{
		std::ifstream infile;
		infile.open(file.c_str());

		if(! infile.is_open()) Util::Error(ErrorNum::NoFileOpen, file, __func__);

		std::string line = "";
		int i = 0;
		std::regex rxdate("[0-9].:[0-9].:[0-9].");
		vstr dline;

		while (getline(infile, line))
		{
			if(regex_search(line, rxdate))
			{
				dline.push_back(line);
				i++;
			}
		}

		infile.close();

		i--; //needed b/c dline starts at 0;

		std::string cmonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

		constr month = dline[i].substr(5,3);

		int imonth = 0;

		//convert month to number
		for(int j = 0; j<12; j++)
		{
			if(month == cmonth[j])
			{
				imonth=j+1;
				break;
			}
		}

		const uint day = atoi(dline[i].substr(9, 2).c_str());
		uint hour = atoi(dline[i].substr(12, 2).c_str());
		const uint min = atoi(dline[i].substr(15, 2).c_str());
		const uint sec = atoi(dline[i].substr(18, 2).c_str());
		constr ampm = dline[i].substr(21, 2);
		const uint year = atoi(dline[i].substr(24, 4).c_str());

		if(ampm == "PM" && hour != 12) { hour += 12; } //turn into 24 hours
		else if(ampm == "AM" && hour == 12) { hour = 0; }

		time_t now = 0, dif = 0; //dif = date in file
		double diff = 0;
		time(&now);
		struct tm * timeinfo;
		timeinfo = localtime(&now);
		timeinfo->tm_mon = imonth - 1;
		timeinfo->tm_mday = day;
		timeinfo->tm_hour = hour;
		timeinfo->tm_min = min;
		timeinfo->tm_sec = sec;
		timeinfo->tm_year = year - 1900;
		timeinfo->tm_isdst = -1; //-1 = no info
		dif = mktime(timeinfo);
		diff = difftime(now, dif);

		if(diff >= DValue::DiffTime) return 1; //more then 24 hours
		else return 0;
	}

	int InsertDate(constr file, np n_p)
	{
		const int cdate = CheckLastDate(file); //1 if last date is more then 24hrs

		std::string::size_type pos = var.find_first_of(Var::Time());
		if(pos == std::string::npos && cdate < 1) return 1;

		if(n_p == NP::notes) InsertDateNotes(file);
		else if(n_p == NP::points) AppendTextL(file, NL() + Util::GetDate());
		else if(n_p == NP::neither) AppendTextL(file, NL() + Util::GetDate());
		// TODO : add else if norp and other

		if(n_p != NP::neither) //don't log if for Log
		{
			constr file_out = OutFileTxt(file);
			Util::SayE(SayNum::InsertDate, file_out, Color::Default(), false, VLevel::Verbose);
			Log::append("Inserted Date to file: " + file);
		}

		return 0;
	}

	void InsertDateNotes(constr file)
	{
		str date = Util::GetDate();
		date = NL() + date + NL();
		InsertTextLNL(file, date);
	}

	int InsertTextLNL(constr file, constr text) //Insert Text Last New Line
	{
		std::string line = "", tmp = "";
		std::fstream infile;
		infile.open(file.c_str());

		if(! infile.is_open()) return Util::Error(ErrorNum::NoFileOpen, file, __func__);

		int i = 0;

		//count blank lines.
		while (getline(infile, line))
		{
			if(line == "") i++;
		}

		infile.clear(); //clear eof state
		infile.seekg(0, std::ios::beg); //go back to the beginning of the file.

		//insert test into last blank line.
		bool done = false;
		int j = 0;

		while (getline(infile, line))
		{
			tmp += line;
			if(line == "") j++;
			if(j == i && done == false)
			{
				tmp += text;
				done = true;
			}
			tmp += NL();
		}

		tmp.erase(tmp.size() - NL().size()); //remove extra newline

		infile.close(); //cannot read and write at the same time.
		infile.open(file.c_str());

		if(! infile.is_open()) return Util::Error(ErrorNum::NoFileOpen, file, __func__);

		infile << tmp;

		infile.close();

		Log::append(str(__func__) + ": " + text);

		return 0;
	}

	int FirstCheck(constr ufile, np npt)
	{
		std::string::size_type pos = var.find_first_of(Var::Check());
		std::string::size_type pos2 = var.find(Var::SetMainNick()); //don't create newfile b/c we are going to rename old file.
		if(pos != std::string::npos || pos2 != std::string::npos) return 1;

		const char* ufile_c = ufile.c_str();

		int fd = open(ufile_c, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
		if(fd < 0)
		{
			close(fd);
			return 2;
		} //exit if file exist.
		close(fd);

		std::ofstream file;

		file.open(ufile_c);
		file << Util::GetDate() << NL();

		if(npt == NP::notes)
		{
			file << NL() << NL() << Delim::NickStr() << NL();
			file << Delim::FriendStr() << NL();
			file << Delim::PointStr() << Delim::ET() << DValue::Points(); //0 default amount of points;
		}

		file.close();

		constr ufile_o = OutFileTxt(ufile);

		Util::SayE(SayNum::FirstCreated, ufile_o);

		Log::append("First Created: " + ufile_o);

		return fd;
	}

	void ParseOpts(void)
	{
		std::string::size_type pos;

		pos = var.find(Var::CheckExit());
		if(pos != std::string::npos)
		{
			CheckC(true);
		}
		else if(var.find_first_of(Var::Check()) != std::string::npos)
		{
			CheckC(false);
		}
		if(var.find_first_of(Var::AddText()) != std::string::npos)
		{
			//FunctionJ();
		}
		if(var.find(Var::PointsLog()) != std::string::npos)
		{
			FunctionPL();
			Util::Exit();
		}
		if(var.find_first_of(Var::Points()) != std::string::npos)
		{
			FunctionP();
		}
	//	if(var.find("ns") != std::string::npos)
	//	{
	//		Nicks.swapnicks();
	//	}
	//	else if(var.find("sm") != std::string::npos)
	//	{
	//		Nicks.setmainnick();
	//	}
	//	else if(var.find("nsa") != std::string::npos)
	//	{
	//		Nicks.searchadd("");
	//	}
	//	else if(var.find("nm") != std::string::npos)
	//	{
	//		Nicks.newmainnick();
	//	}
	//	else if(var.find("Dn") != std::string::npos)
	//	{
	//		Nicks.deletenick();
	//	}
	//	else if(var.find("DMn") != std::string::npos)
	//	{
	//		Nicks.deletenick(true);
	//	}
	//    else if(var.find_first_of("n") != std::string::npos)
	//    {
	//        Nicks.function_n();
	//    }
	}

	void CheckC(bool cc)
	{
		str fileout = "";
		saynum exist = SayNum::FileNotExist;

		if(var.find_first_of(Var::Points()) != std::string::npos)
		{
			exist = Util::CheckFile(rpointfile.c_str());
			fileout = rpointfileout;
		}
		else if(var.find_first_of(Var::Notes()) != std::string::npos)
		{
			exist = Util::CheckFile(rnotefile.c_str());
			fileout = rnotefileout;
		}

		std::regex rxnp("[" + Var::Notes() + Var::Points() + "]");
		if(! std::regex_search(var, rxnp))
		{
			exist = Util::CheckFile(rnotefile.c_str());
			Util::SayE(exist, rnotefileout, Color::Default(), false, VLevel::Quiet);
			exist = Util::CheckFile(rpointfile.c_str());
			fileout = rpointfileout;
		}

		Util::SayE(exist, fileout, Color::Default(), false, VLevel::Quiet);

		if(cc == true) Util::Exit();
	}

	int FunctionP(void)
	{
		DisplayPoints();

		CheckExit("tpjcqvk");

		FirstCheck(pointfile, NP::points);

		std::string::size_type jpos = var.find_first_of(Var::AddText());
		if(jpos == std::string::npos)
		{
			InsertDate(pointfile, NP::points);
		}

		if(endtext != "" && jpos == std::string::npos)
		{
			AppendTextL(pointfile, endtext);
		}

		std::regex pregex("[^*/+=-]*([*/+=-]*)([0-9]+).*");
		std::smatch match;
		std::regex_match(var, match, pregex);

		constr oper = match[1];
		constr snum = match[2];

		const int num = atoi(snum.c_str());

		if(oper == "") return 1;

		const int points = GetPoints();
		int tpoints = 0;

		if(oper == "+") tpoints = points + num;
		else if(oper == "-") tpoints = points - num;
		else if(oper == "*") tpoints = points * num;
		else if(oper == "/") tpoints = points / num;
		else if(oper == "=") tpoints = num;

		constr stpoints = Util::itoa(tpoints);
		UpdatePoints(notefile, stpoints);

		Util::Say(Util::Bold("Total") + " " + Delim::PointStr() + " " + Util::itoa(points) + " " + oper + " " +
			snum + " = " + Util::Color(Color::Green(), false, stpoints), Color::Default(), false, VLevel::Quiet);

		AppendTextL(pointfile, oper + snum + " " + Delim::Points() + "." + " Total " + Delim::PointStrSp() + stpoints);

		return 0;
	}

	int FunctionPL(void)
	{
		DisplayPoints();

		std::string ekate = OpenProg() + " " + rpointfile;

		Util::SayE(SayNum::Kate_opened, rpointfileout);

		std::string::size_type pos = var.find_first_of(Var::Notes());
		if(pos != std::string::npos)
		{
			Util::SayE(SayNum::Kate_opened, rnotefileout);
			ekate += (" " + rnotefile);
		}

		//only works when debugging, with Kate, works just fine with gedit. no idea why.
	//    pid_t gpid = fork();
	//    if(gpid == 0) //only execute in the new process
	//    {
	//        int eret = execlp(ekate.c_str(), ekate.c_str(), pfile.c_str(), NULL);
	//        if(eret < 0) cout << "execpl error" << endl;
	//        //exit(0);
	//    }

		system(ekate.c_str());

		Util::SayE(SayNum::Kate_closed, rpointfileout);

		if(pos != std::string::npos)
		{
			Util::SayE(SayNum::Kate_closed, rnotefileout);
			Log::append("Opened: " + rnotefile + " in Kate");
		}

		Log::append("Opened: " + rpointfile + " in Kate");

		return 0;
	}

	int GetPoints(void)
	{
		std::ifstream infile;
		infile.open(rnotefile.c_str());

		if(! infile.is_open()) Util::Error(ErrorNum::NoFileOpen, rnotefile, __func__);

		std::string line = "";

		 while (getline(infile, line))
		{
			if(line.find(Delim::PointStr()) != std::string::npos)
			{
				break;
			}
		}

		infile.close();

		int points = atoi(line.substr(Delim::PointStrSp().length()).c_str());

		return points;
	}

	void DisplayPoints(void)
	{
		constr points = Util::itoa(GetPoints());

		vstr output;
		output.push_back(points);
		output.push_back(realnick);

		Util::SayE(SayNum::Points, output);
	}

	int UpdatePoints(constr file, constr points)
	{
		std::string line = "";
		std::fstream infile;
		infile.open(file.c_str());

		if(! infile.is_open()) Util::Error(ErrorNum::NoFileOpen, file, __func__);

		std::string tmp = "";

		 while (getline(infile, line))
		{
			if(line.find(Delim::PointStr()) != std::string::npos)
			{
				tmp += (Delim::PointStr() + " " + points);
			}
			else
			{
				tmp += line;
			}
			tmp += NL();
		}

		infile.close();
		infile.open(file.c_str());
		if(! infile.is_open()) Util::Error(ErrorNum::NoFileOpen, file, __func__);

		infile << tmp;
		infile.close();

		Log::append("Updated Points File");

		return 0;
	}

	int AppendTextL(constr file, constr text)
	{
		Util::AppendText(file, text);

		constr fileout = OutFileTxt(file);
		vstr alltext;
		alltext.push_back(fileout);
		alltext.push_back(text);
		Util::SayE(SayNum::AddedText, alltext);

		Log::append("Added Text: " + text + "to file: " + file);

		return 0;
	}

	int WriteFileL(constr file, constr text)
	{
		Util::WriteFile(file, text);

		constr say = "Wrote: " + text + " to file: " + file;
		Util::Say(say);
		Log::append(say);

		return 0;
	}

	int MainCheck(void)
	{
		struct stat sb;

		if (! (stat(pwd.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)))
		{
			int mkd = mkdir(pwd.c_str(), 0744);
			if( mkd != 0)
				Util::Error(ErrorNum::NoMakeDir, pwd, __func__);

			constr say = "Created User Notes Directory: " + pwd;
			Util::Say(say);
			Log::append(say);
		}

		saynum check = Util::CheckFile(nickfile.c_str());
		if(check != SayNum::FileExist)
		{
			//Nicks.searchadd(nickfile);
		}

		return 0;
	}

	void CheckExit(constr regx)
	{
		std::regex strex( "[" + regx + "]");
		std::string replace = "";

		constr match = std::regex_replace(var, strex, replace);

		if(match == "")
		{
			Util::Exit();
		}
	}
} //namespace mf
} //namespace nn
