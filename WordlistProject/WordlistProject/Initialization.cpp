#include "pch.h"
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "Initialization.h"
#include "WordlistProject.h"

Init::Init(int argc,char *argv[])
{
	w = false, c = false, r = false;
	h = '\0', t = '\0';
	n = 0;
	Init::parse(argc,argv);
}



void Init::parse(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0 && !h)
		{
			i++;
			if (i >= argc) error(5);
			if (strlen(argv[i]) > 1) error(3);
			h = argv[i][0];
		}
		else if (strcmp(argv[i], "-t") == 0 && !t)
		{
			i++;
			if (i >= argc) error(5);
			if (strlen(argv[i]) > 1) error(3);
			t = argv[i][0];
		}
		else if (strcmp(argv[i], "-r") == 0 && !r)
		{
			r = true;
		}
		else if (strcmp(argv[i], "-w") == 0 && !w)
		{
			w = true;
			i++;
			if (i >= argc) error(5);
			file = argv[i];
		}
		else if (strcmp(argv[i], "-c") == 0 && !c)
		{
			c = true;
			i++;
			if (i >= argc) error(5);
			file = argv[i];
		}
		else
		{
			error(0);
		}
	}
	if (w && c) error(1);
	if (!w && !c) error(2);
	if (h && !isalpha(h)) error(3);
	if (t && !isalpha(t)) error(3);
}

void Init::InitWord()
{
	FILE *pf = fopen(file, "r");
	char text[100000];
	size_t size = 0;
	while ((text[size] = fgetc(pf)) != EOF) size++;
	text[size] = '\0';
	char stream[100];
	size_t j = 0, i = 0;
	while (1)
	{
		if (text[i] == '\0')
		{
			if (j > 0)
			{
				stream[j] = '\0';
				words[n] = (char*)malloc(j + 1);
				memcpy(words[n], stream, j + 1);
				n++;
				j = 0;
			}
			break;
		}

		if (isalpha(text[i]))
		{
			stream[j] = tolower(text[i]);
			j++;
		}
		else if (j > 0)
		{
			stream[j] = '\0';
			words[n] = (char*)malloc(j + 1);
			memcpy(words[n], stream, j + 1);
			n++;
			j = 0;
		}
		i++;
	}
}

