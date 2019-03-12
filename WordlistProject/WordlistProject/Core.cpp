#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
#include "WordlistProject.h"
#include "FindChain.h"
#include "Initialization.h"
#include "Core.h"

int Core::gen_chain_word(char * words[], int len, char * result[], char head, char tail, bool enable_loop)
{
	FindChain findchain(words, len, 0, head, tail);
	if (enable_loop)
		return findchain.GetWordChain_Ring(result);
	else
	{
		if (findchain.hasRing()) error(4);
		return findchain.GetWordChain_NoRing(result);
	}
	
}

int Core::gen_chain_char(char * words[], int len, char * result[], char head, char tail, bool enable_loop)
{
	FindChain findchain(words, len, 0, head, tail);
	if (enable_loop)
		return findchain.GetWordChain_Ring(result);
	else
	{
		if (findchain.hasRing()) error(4);
		return findchain.GetWordChain_NoRing(result);
	}
}

void Core::printf_chain(char * result[], int result_len)
{
	printf("%d\n", result_len);
	for (int i = 0; i < result_len; i++)
		printf("%s\n", result[i]);
}

Init Core::init_word(int argc, char * argv[])
{
	return Init(argc, argv);
}

void Core::build_map(Init & init)
{
	init.InitWord();
}


