/*
 /*
 * File:   main.cpp
 * Author: Somebody
 *
 * Created on August 28, 2012, 1:46 AM
 */

#include "utils.h"
#include "mf.h"

int main(int argc, char** argv)
{
	if(argc < nn::DValue::MinArgs)
		nn::Util::Error(nn::ErrorNum::NoArgs);

	//setup vlevel and vars
	nn::Util::Begin(argc, argv);

	//setup main functions and begin program
	nn::mf::Begin();

	//Display end output and exit.
	nn::Util::Exit();

	return 0;
}
