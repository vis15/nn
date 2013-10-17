/*
 * File:   Log_t.h
 * Author: dis
 *
 * Created on August 31, 2012, 8:58 PM
 */

#ifndef LOG_T_H
#define	LOG_T_H

#include "main.h"

namespace nn
{
namespace Log
{
	int append(str text);
	int writeout(void);
	int check(void);
	void SetVars(constr vart, constr realnickt, uint lognum);
	void Begin(constr vart, constr realnickt, uint lognum);
	int Rotate(void);
	int Compress(void);
	int BeginOutput(void);
	//todo: implement reverse?
} //namespace Log
} //namespace nn

#endif	/* LOG_T_H */
