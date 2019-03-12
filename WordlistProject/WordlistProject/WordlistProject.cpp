#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
#define MAX 100000
#define NUM 26

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
		char* newWord = 0;//strlwr(words[i]);
		int wordLen = strlen(newWord);
		edges[newWord[0] - 'a'][newWord[wordLen - 1] - 'a'].push_back({ newWord,key });
	}
}

int GetWordChain_NoRing( char* result[], char head, char tail)
{
	
}

bool hasRing()
{
    int inDegree[NUM];
    int n = 0;
	bool p[NUM];

	for (int i = 0; i<NUM; i++) inDegree[i] = 0;
	for (int i = 0; i < NUM; i++) 
		for (int j = 0; j < NUM; j++) {
			std::vector<Wordedge> &e = edges[i][j];
			if (i == j && e.size() > 1) 
				return true;
			if (e.size() > 0) {
				inDegree[j]++;
				p[i] = p[j] = true;
			}
		}
	for (int i = 0; i < NUM; i++) n += p[i];

	std::queue<int> Q;
	for(int i = 0;i< NUM; i++)
		if (inDegree[i]==0 && p[i]) {
			Q.push(i);
		}
	int cnt = 0;
	while (!Q.empty()) {
		int newP = Q.front();
		Q.pop();
		printf("%c\n", newP+'a');
		cnt++;
		for (int i = 0; i<NUM; i++) {
			if (edges[newP][i].size() == 0) continue; 
			inDegree[i]--;
			if (inDegree[i]==0) {
				Q.push(i);
			}
		}
	}
	if (cnt==n) 
		return false;
	return true;
}

int main(int argc, char *argv[])
{
	printf("%s", hasRing() ? "True" : "False");
    std::cout << "Hello World!\n"; 
}