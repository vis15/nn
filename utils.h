/*
 * File:   utils.h
 * Author: dis
 *
 * Created on September 5, 2012, 4:35 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include "main.h"
#include "vars.h"

namespace nn
{
namespace Util
{
	constr Pwd(void);
	constr itoa(int num);
	constr Color(constr color, bool bold = false, constr text = "");
	constr Bold(constr text);
	constr GetDir(void);
	constr GetFilePath(constr name = "", constr ext = "");
	constr GetFilePathOut(constr fname, constr ext);
	constr GetDate(void);
	int Say(constr text, constr color = Color::Default(), bool bold = false, vlevel vlevelt = VLevel::xDefault);
	void SayE(saynum num, constr text = "", constr color = Color::Default(), bool bold = false, vlevel vlevelt = VLevel::xDefault);
	void SayE(saynum num, vcstr text, constr color = Color::Default(), bool bold = false, vlevel vlevelt = VLevel::xDefault);
	constr GetVerOutLvl(void);
	saynum CheckFile(const char* file);
	int Error(errornum err_num, constr text = "", constr fn = "", bool exit_ = true);
	void Exit(int status = 0);
	constr SetArgs(int argc, char* argv[]);
	void SetProgVlevel(void); //also sets debug
	void Begin(int argc, char** argv);
	int AppendText(constr file, constr text);
	int WriteFile(constr file, constr text);
	void GetVars(gvar &vars);
	void Help(void);
	constr Underline(constr text);
} //namespace Util
} //namespace nn

#endif	/* UTILS_H */
