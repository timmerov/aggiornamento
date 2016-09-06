/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
hello world example.
*/

#include <amlib/amlib.h>

#include <iostream>

int main(
	int argc, char *argv[]
) throw() {
	(void) argc;
	(void) argv;

	std::cout<<"Hello, World!"<<std::endl;
	std::cout<<"Goodbye, World!"<<std::endl;

	return 0;
}
