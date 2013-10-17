/*
 * File:   vars.h
 * Author: dis
 *
 * Created on September 9, 2012, 4:53 PM
 *
 * Variables placed in functions because of unordered initialization.
 */

#include <sstream>
#include "vars.h"

namespace nn
{
//IRC Color Codes
namespace Color
{
	constr Purple(void) { return "06"; }
	constr Green(void) { return "03"; }
	constr Blue(void) { return "02"; }
	constr LBlue(void) { return "12"; }
	constr Red(void) { return "05"; }
	constr Default(void) { return Purple(); } //Default Color
}


//Program default directories and files ext
namespace DirExt //D=Directory, x=extensions
{
	constr UserNoteDir(void) { return "/UserNotes"; }
	constr DebugDir(void) { return "debug"; }
	constr DNotes(void) { return "Notes"; } //D means Dot because there use to be a period before Notes
	constr DPoints(void) { return "Points"; }
	constr DLog(void) { return "log"; }
	constr DNicks(void) { return "nicks"; }
	constr DConfig(void) { return "config"; }
}


//Delimiters
namespace Delim
{
	constr Main(void) { return ":"; }
	constr MainSp(void) { return ": "; }

	constr Sub(void) { return ","; }
	constr SubSp(void) { return ", "; }

	constr Points(void) { return "Points"; }
	constr PointStr(void) { return Points() + Main(); }
	constr PointStrSp(void) { return Points() + MainSp(); }
	constr Nicks(void) { return "Nicks"; }
	constr NickStr(void) { return Nicks() + Main(); }
	constr NickStrSp(void) { return Nicks() + MainSp(); }
	constr Friends(void) { return "Friends"; }
	constr FriendStr(void) { return Friends() + Main(); }
	constr FriendStrSp(void) { return Friends() + MainSp(); }
	constr ET(void) { return " "; } //Delim for End Text
}


//other
constr NL(void) { return "\n"; }
constr NLC(void) { std::stringstream nlc; nlc << char(10); return nlc.str(); } //newline char
constr OpenProg(void) { return "scite"; }

//values
namespace DValue //Default Values
{
	constr Points(void) { return "0"; }
}


//variables used in program
namespace Var
{
	constr DontLog(void) { return "L"; } //do not use Log file
	constr Notes(void) { return "n"; }
	constr Check(void) { return "c"; }
	constr SetMainNick(void) { return "sm"; }
	constr Time(void) { return "t"; }
	constr Points(void) { return "p"; }
	constr Debug(void) { return ":d"; }
	constr NoOutput(void) { return "k"; }
	constr Queit(void) { return "q"; }
	constr Verbose(void) { return "v"; }
	constr CheckExit(void) { return "cc"; }
	constr AddText(void) { return "j"; }
	constr PointsLog(void) { return "pl"; }
}

namespace Info
{
	constr Version(void) { return "4.5"; }
	constr Author(void) { return "Someone"; }
	constr Name(void) { return "nn"; }
	constr Description(void) { return "KVIrc User Notes Program"; }
}
} //namespace nn
