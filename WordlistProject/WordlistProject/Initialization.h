#ifndef INITIALIZATION_H
#define INITIALIZATION_H


class Init
{
public:

	bool w, c, r;
	char h, t;
	char *file;
	char*  words[10000];
	int n;

	Init(int argc,char *argv[]);
	void parse(int argc, char *argv[]);
	void InitWord();
};
#endif