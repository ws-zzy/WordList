// WordlistProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <string>
#include <cstring>
#include <iostream>
#include "cmdline.h"
#define MAX 100000


void ParsePara(int paraNum, char *paras[])
{
	cmdline::parser arguements;
	
	arguements.add<std::string>("Word", 'w', "longestWord", true, "");
	arguements.add<std::string>("Char", 'c', "longestChar", true, "");
	arguements.add<char>("Head", 'h', "beginChar", true, '\0');
	arguements.add<char>("Tail", 't', "endChar", true, '\0');
	arguements.add("Ring", 'r', "ringed");
	arguements.parse_check(paraNum, paras);

	std::cout << arguements.get<std::string>("Word") << "://"
		<< arguements.get<std::string>("Char") << ":"
		<< arguements.get<char>("Head") << ":"
		<< arguements.get<char>("Tail") << std::endl;

	if (arguements.exist("Ring")) std::cout << "r!!!" << std::endl;

}



struct Wordedge
{
	char* word;
	int key;
};

std::vector<Wordedge> edges[26][26];


int BuildWordMap(char* words[], int len, bool isWord)
{
	for (int i = 0; i < len; i++)
	{
		int key = strlen(words[i]);
		if (isWord) key = 1;
		char* newWord = strlwr(words[i]);
		int wordLen = strlen(newWord);
		edges[newWord[0] - 'a'][newWord[wordLen - 1] - 'a'].push_back({ newWord,key });
	}
}

int GetWordChain_NoRing( char* result[], char head, char tail)
{
	
}

int main(int argc, char *argv[])
{
	ParsePara(argc, argv);
    std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
