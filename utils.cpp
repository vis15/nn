/*
 * File:   utils.cpp
 * Author: dis
 *
 * Created on September 5, 2012, 4:35 PM
 */

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "utils.h"

namespace nn
{
namespace Util
{
	namespace
	{
		bool Debug = false;
		vlevel Prog_vlevel = VLevel::xDefault;
		str Var = "", Name = "", Endtext = "", Realnick = "", Realonick = "", Svrch = "";
	}

	void Begin(int argc, char** argv)
	{
		SetArgs(argc, argv);
		SetProgVlevel(); //sets Debug mode
		//todo: check var for errors? Maybe not for speed
	}

	void GetVars(gvar &vars)
	{
		vars.svrch = Svrch;
		vars.name = Name;
		vars.var = Var;
		vars.endtext = Endtext;
		vars.debug = Debug;
	}

	constr Pwd(void)
	{
		struct passwd *pw = getpwuid(getuid());
		constr pwd = pw->pw_dir;
		return pwd;
	}

	constr itoa(int num)
	{
		std::stringstream s;
		s << num;
		return s.str();
	}

	constr Color(constr color, bool bold, constr text)
	{
		const char color_c = nn::Color::Color_v; //converts Color to a char for output.
		const char bold_v = nn::Color::Bold_v;
		std::stringstream p;

		if(bold == true)
		{
			if(color == "" && text == "") { p << bold_v; }
			else if(color == "" && text != "") { p << bold_v << text << bold_v; }
			else { p << color_c << color << bold_v; }
		}
		else
		{
			if(text == "") p << color_c << color;
			else p << color_c << color << text << color_c << nn::Color::Default();
		}

		return p.str();
	}

	constr Underline(constr text)
	{
		const char uchar = Color::Underline_v;
		std::stringstream ss;

		ss << uchar << text << uchar;

		return ss.str();
	}

	constr Bold(constr text)
	{
		return Util::Color("", true, text);
	}

	constr GetDir(void)
	{
		std::string pwd = Pwd() + DirExt::UserNoteDir();
		constr dot = ".";

		if(Debug == true)
			pwd += dot + nn::DirExt::DebugDir();

		return pwd;
	}

	constr GetFilePath(constr name, constr ext)
	{
		str pwd = Pwd();
		str ddir = "";
		str dot = ".";

		if(Debug == true)
		{
			ddir = dot;
			ddir += DirExt::DebugDir();
		}

		if(name == "" && ext == "")
			dot = "";

		pwd += DirExt::UserNoteDir();
		pwd += ddir;
		pwd += "/";
		pwd += name;
		pwd += dot;
		pwd += ext;

		return pwd;
	}

	constr GetFilePathOut(constr fname, constr ext)
	{
		std::string pwd = "";

		if(Prog_vlevel > nn::VLevel::xDefault) //verbose or higher
		{
			pwd += GetFilePath("", ""); //GetFilePath returns with '/' at the end. GetDir does not.
		}

		constr bold = Color("", true);
		pwd += bold + fname + bold + Color(nn::Color::Blue()) + "." + ext + Color(nn::Color::Default());

		return pwd;
	}

	constr GetDate(void)
	{
		time_t cdate;
		struct tm* timeinfo;
		char ctime[80];

		time(&cdate);
		timeinfo = localtime(&cdate);

		strftime(ctime, 80, "%a, %b %d %I:%M:%S %p %Y %Z", timeinfo);
		constr otime(ctime);

		return otime;
	}

	int Say(constr text, constr color, bool bold, vlevel vlevelt)
	{
		if(vlevelt > Prog_vlevel) return 1;

		constr p = Color(color, bold);
		std::cout << p << text << std::endl;

		return 0;
	}

	void SayE(saynum num, vcstr text, constr color, bool bold, vlevel vlevelt)
	{
		//constr err = GetVerOutLvl();

		switch(num)
		{
			case nn::SayNum::AddedText:
				Say(text.at(0) + " " + Bold("Added: ") + text.at(1), color, bold, vlevelt);
				break;
			case nn::SayNum::Points:
				Say(text.at(1) + " " + Bold(nn::Delim::PointStr()) + " " + Color(Color::Green(), false, text.at(0)), color, bold, vlevelt);
				break;
			default:
				Error(nn::ErrorNum::Unknown, "", __func__, false);
				break;
		}
	}

	void SayE(saynum num, constr text, constr color, bool bold, vlevel vlevelt)
	{
		constr err = GetVerOutLvl();

		switch(num)
		{
			case nn::SayNum::FileNotExist:
				Say(err + text + Bold(" not exist") + ".", color, bold, vlevelt);
				break;
			case nn::SayNum::FileExist:
				Say(err + text + Bold(" exist") + ".", color, bold, vlevelt);
				break;
			case nn::SayNum::NotWritable:
				Say(err + text + " is not writable.", color, bold, vlevelt);
				break;
			case nn::SayNum::FileCreated:
				Say(err + text + " was created.", color, bold, vlevelt);
				break;
			case nn::SayNum::OpenedFile:
				Say(Bold("Opened") + " file: " + text, color, bold, vlevelt);
				break;
			case nn::SayNum::FirstCreated:
				Say(err + text + " was first created", color, bold, vlevelt);
				break;
			case nn::SayNum::InsertDate:
				Say(Bold(err) + ": Inserted Date into: " + text, color, bold, vlevelt);
				break;
			case nn::SayNum::Kate_opened:
				Say(Bold("Opened") + " in Kate: " + text, color, bold, vlevelt);
				break;
			case nn::SayNum::Kate_closed:
				Say(Bold("Closed") + " in Kate: " + text, color, bold, vlevelt);
				break;
			case nn::SayNum::SecondNick:
				Say("You need to enter the second nick", color, bold, vlevelt);
				break;
			default:
				Error(nn::ErrorNum::Unknown, "", __func__, false);
				break;
		}
	}

	constr GetVerOutLvl(void)
	{
		if(Prog_vlevel == nn::VLevel::Debug) return "Debug: ";
		else if(Prog_vlevel == nn::VLevel::Verbose) return "Verbose: ";
		else if(Prog_vlevel == nn::VLevel::xDefault || Prog_vlevel == VLevel::Quiet || Prog_vlevel == VLevel::kNone) return "Info: ";
		else return "Error: ";
	}


	saynum CheckFile(const char* file)
	{
		std::ifstream cfile(file);
		saynum output = nn::SayNum::FileNotExist;

		if(cfile) { output = nn::SayNum::FileExist; }
		else { return nn::SayNum::FileNotExist; }

		if (! cfile.good()) output = nn::SayNum::NotWritable;

		return output;
	}

	int Error(errornum err_num, constr text, constr fn, bool exit_)
	{
		std::string stretxt = "";

		switch (err_num)
		{
			case nn::ErrorNum::NoMakeDir:
				stretxt = "directory " + text + " could not be created";
				break;
			case nn::ErrorNum::NoFileOpen:
				stretxt = "file " + text + " could not be opened";
				break;
			case nn::ErrorNum::NoDirOpen:
				stretxt = "directory " + text + " could not be opened";
				break;
			case nn::ErrorNum::NoArgs:
				stretxt = "To few Arguments";
				break;
			default:
			case nn::ErrorNum::Unknown:
				stretxt = "An Unknown Error has occurred.";
				break;
		}

		str fns = "";

		if(fn != "")
			fns = fn + ": ";

		constr stre = "Error: ";
		stretxt = fns + stre + stretxt;
		Say(stretxt, nn::Color::Red(), true, nn::VLevel::kNone); //should always output

		if(exit_ == true)
			exit(nn::DValue::ExitFail);

		return 0;
	}

	void Exit(int status)
	{
		Say("End Output.", nn::Color::Blue());

		exit(status);
	}

	constr SetArgs(int argc, char* argv[])
	{
		std::string output = "";
		int pos = 0; //position

		Svrch = argv[++pos];
		output += Svrch + nn::Delim::ET();

		Name = argv[++pos];
		output += Name + nn::Delim::ET();
		Var = "";

		if(argv[++pos] != 0)
			Var = argv[pos];

		SetProgVlevel();
		output += Var + nn::Delim::ET();
		Endtext = "";

		for(int i = (nn::DValue::MinArgs + 1); i<argc; i++)
		{
			Endtext += argv[i] + nn::Delim::ET();
		}

		//remove extra Delim::ET
		if(Endtext != "")
			Endtext = Endtext.erase(Endtext.size() - Delim::ET().size());

		output += Endtext;

		return output;
	}

	void SetProgVlevel(void)
	{
		std::string::size_type pos;

		pos = Var.find(nn::Var::Debug());
		if(pos != std::string::npos)
		{
			Prog_vlevel = nn::VLevel::Debug;
			Debug = true;
			return;
		}

		pos = Var.find_first_of(nn::Var::NoOutput());
		if(pos != std::string::npos)
		{
			Prog_vlevel = nn::VLevel::kNone;
			return;
		}

		pos = Var.find_first_of(nn::Var::Queit());
		if(pos != std::string::npos)
		{
			Prog_vlevel = nn::VLevel::Quiet;
			return;
		}

		pos = Var.find_first_of(nn::Var::Verbose());
		if(pos != std::string::npos)
		{
			Prog_vlevel = nn::VLevel::Verbose;
			return;
		}
	}

	int AppendText(constr file, constr text)
	{
		if(CheckFile(file.c_str()) != nn::SayNum::FileExist)
		{
			WriteFile(file, ""); //creates  a blank file
		}

		std::string line, tmp;
		std::fstream infile;
		infile.open(file.c_str());

		if(! infile.is_open()) Error(nn::ErrorNum::NoFileOpen, file, __func__);

		//ios:app works intermediately, as if there is hidden newline
		while (getline(infile, line))
		{
			tmp += ( line + nn::NL() );
		}

		infile.close();
		tmp += text;

		infile.open(file.c_str());

		if(! infile.is_open()) Error(nn::ErrorNum::NoFileOpen, file, __func__);

		infile << tmp;
		infile.close();

		return 0;
	}

	int WriteFile(constr file, constr text)
	{
		std::ofstream nfile(file.c_str());

		if(! nfile.is_open()) Error(nn::ErrorNum::NoFileOpen, file, __func__);

		nfile << text;

		nfile.close();

		return 0;
	}

	void Help(void)
	{
		Say("Usage: " + Info::Name() + " ServerChannel Nick [vars] [End Text]\n");
		//todo: Display Help.txt
	}

} //namespace Util
} //namespace nn
