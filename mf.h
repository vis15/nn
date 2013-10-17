/*
 * File:   mf_t.h
 * Author: dis
 *
 * Created on August 31, 2012, 10:16 PM
 */

#ifndef MF_T_H
#define	MF_T_H

#include "main.h"

namespace nn
{
namespace mf //mf = main functions
{
	//void ExitL(int status = 0);
	void CheckExit(constr regx);
	int MainCheck(void);
	int AppendTextL(constr file, constr text);
	int UpdatePoints(constr file, constr points);
	int FunctionPL(void);
	int WriteFileL(constr file, constr text = "");
	//int ErrorL(errornum err_num, constr text = "", constr fn = "", bool exit_ = true);
	void Begin(void);
	void ParseOpts(void);
	int FunctionP(void);
	int FirstCheck(constr ufile, np npt);
	int InsertDate(constr file, np n_p);
	void InsertDateNotes(constr file);
	int InsertTextLNL(constr file, constr text);
	void DisplayPoints(void);
	void CheckC(bool cc);
	int CheckLastDate(constr file);
	int GetPoints(void);
	constr OutFileTxt(constr file);
	void SetVars(void);
	void SetRVars(void);
	void BeginOutput(void);
} //namespace mf
} //namespace nn

#endif	/* MF_T_H */
