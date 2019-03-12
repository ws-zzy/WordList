#ifndef CORE_H
#define CORE_H
class Core
{
public:
	int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
	int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
	void printf_chain(char* result[], int result_len);
	Init init_word(int argc, char *argv[]);
	void build_map(Init & init);
};




#endif
