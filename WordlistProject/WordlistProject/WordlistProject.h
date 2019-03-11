#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <queue>

#define NUM 26
#define MAX 100000

void error();
void parse(int argc, char *argv[]);
void BuildWordMap(char* words[], int len, bool isWord);
bool hasRing();
int GetWordChain_NoRing(char head, char tail);
void dfs(int node, int dist, int num, char tail);
int GetWordChain_Ring(char head, char tail);
void Init(char *words[], int &n, char *file);
