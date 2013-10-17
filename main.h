/*
 * File:   main.h
 * Author: Somebody
 *
 * Created on August 28, 2012, 1:47 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <string>
#include <vector>

namespace nn
{ //lack of indentation per as Google C++ style.
typedef const std::string constr; //it's con str
typedef std::string str;
typedef std::vector<std::string> vstr;
typedef const vstr vcstr;

enum class NP
{
	notes, points, other, norp, neither
};
//np = notes or points

enum class VLevel
{
	kNone, Quiet, xDefault, Verbose, Verbose2, Debug
};

enum class ErrorNum
{
	NoMakeDir, NoFileOpen, NoDirOpen, NoArgs, Unknown
};

enum class SayNum
{
	FileNotExist,
	FileExist,
	NotWritable,
	FileCreated,
	OpenedFile,
	FirstCreated,
	InsertDate,
	AddedText,
	Kate_opened,
	Kate_closed,
	Points,
	SecondNick
};

typedef SayNum saynum;
typedef VLevel vlevel;
typedef ErrorNum errornum;
typedef NP np;

struct GVar
{
	str svrch;
	str name;
	str var;
	str endtext;
	bool debug;
};

typedef GVar gvar;

} //namespace nn

#endif	/* MAIN_H */
