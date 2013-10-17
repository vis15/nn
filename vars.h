/*
 * File:   vars.h
 * Author: dis
 *
 * Created on September 9, 2012, 4:53 PM
 */

#ifndef VARS_H
#define	VARS_H

#include "main.h"

namespace nn
{
//IRC Color Codes
namespace Color
{
	constexpr int Color_v = 0x03;
	constexpr int Bold_v = 0x02;
	constexpr int Underline_v = 0x1f;
	constr Purple(void);
	constr Green(void);
	constr Blue(void);
	constr LBlue(void);
	constr Red(void);
	constr Default(void);
}


//Program default directories and files ext
namespace DirExt //Dir=Directory, Ext=extension
{
	constr UserNoteDir(void);
	constr DebugDir(void);
	constr DNotes(void); //D means dot
	constr DPoints(void);
	constr DLog(void);
	constr DNicks(void);
	constr DConfig(void);
}


//Delimiters
namespace Delim
{
	constr Main(void);
	constr MainSp(void);
	constexpr char MainC = ':';
	constr Sub(void);
	constr SubSp(void);
	constexpr char SubC = ',';
	constr Points(void);
	constr PointStr(void);
	constr PointStrSp(void);
	constr Nicks(void);
	constr NickStr(void);
	constr NickStrSp(void);
	constr Friends(void);
	constr FriendStr(void);
	constr FriendStrSp(void);
	constr ET(void);
}


//other
constr NL(void);
constr NLC(void);
//constr NL
constr OpenProg(void);

//values
namespace DValue //Default Values
{
	constexpr int MaxNicks = 1000;
	constexpr int ExitFail = -1;
	constexpr int MinArgs = 3;
	constr Points(void);
	constexpr int DiffTime = 24 * 60 * 60; //24hrs in seconds
	constexpr int MaxLogSize = 1024; //1MB
}


//variables used in program
namespace Var
{
	constr DontLog(void); //do not use Log file
	constr Notes(void);
	constr Check(void);
	constr SetMainNick(void);
	constr Time(void);
	constr Points(void);
	constr Debug(void);
	constr NoOutput(void);
	constr Queit(void);
	constr Verbose(void);
	constr CheckExit(void);
	constr AddText(void);
	constr PointsLog(void);
}

namespace Info
{
	constr Version(void);
	constr Author(void);
	constr Name(void);
	constr Description(void);
}
} //namespace nn

#endif	/* VARS_H */
