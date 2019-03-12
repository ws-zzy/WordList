#include "pch.h"
#include<cstdio>
#include<cstdlib>
#include "WordlistProject.h"
#include "Initialization.h"
#include "FindChain.h"
#include "Core.h"


void error(int e_code)
{

	switch (e_code)
	{
	case 0:
		printf("Incorrect command line parameters!");
		break;

	case 1:
		printf("Command line arguments include both -w and -c!");
		break;

	case 2:
		printf("Command line parameters do not contain - w or -c!");
		break;

	case 3:
		printf("The parameter should be a letter!");
		break;

	case 4:
		printf("There is a ring in the word list!");
		break;

	case 5:
		printf("Missing parameters!");
		break;

	default:

		break;
	}

	exit(1);
	return;
}



// ======================================= //


int main(int argc, char *argv[])
{
	
	Core core;
	char * result[105];
	int result_len = 0;

	Init init = core.init_word(argc,argv);
	core.build_map(init);
	if (init.w) result_len = core.gen_chain_word(init.words,init.n,result,init.h,init.t,init.r);
	else result_len = core.gen_chain_char(init.words, init.n, result, init.h, init.t, init.r);
	core.printf_chain(result, result_len);

	return 0;
}
