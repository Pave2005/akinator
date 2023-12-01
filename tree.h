#ifndef AKINATOR
#define AKINATOR
#include "../stack/stack_cd.h"

#define RED "\x1b[31;41m"

const int size_of_arrays = 100;

struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    char* data;
};

struct Tree
{
    TreeNode* root;
    FILE* file;
    stack compare_1;
    stack compare_2;

    int counter;
    char* object_1;
    char* object_2;

    long long status;
};

enum AKINATOR_ERRORS
{
    INCORRECT_ARGV = 0,     // не правильный ввод в коммандную строку
};

enum PASS_NUM
{
    FIRST_PASS = 1,
    SECOND_PASS = 2,
    DEFAULT = 0,
};

enum WAY
{
    FIRST_WAY = 1,
    SECOND_WAY = 2,
    DEFAULT_WAY = 3,
};

enum INSTR
{
    GUESS = 1,
    COMPARE = 2,
    DEFINITION = 3,
};

enum SPEAKER
{
    SAY = 1,
    NO_SAY = 0,
};

int CheckAkinatorErr (Tree* tree);
void AkinatorMode (Tree* tree, TreeNode** node, int first_argv, int second_argv);
void HandleCommandLine (int argc, const char *argv[], int* first_argv, int* second_argv, Tree* tree);
char* ReadText (long file_size, FILE* file);
long FileSize (FILE* file);
char* TreeCtor (Tree* tree);
void PrintfNode (const TreeNode* node, const Tree* tree);
void ReadFile (Tree* tree);
void Insert (Tree* tree, TreeNode** node, char* answer, const char* command);
void AkinatorInput (Tree* tree, TreeNode** node, const char* command);
void Akinator (enum INSTR COMMAND, enum PASS_NUM PASS, enum SPEAKER SAY, TreeNode** node, Tree* tree);
void BaseOfData (Tree* tree, TreeNode** node, char* data_buf, int* tmp_count);
void TreeDtor (Tree* tree, TreeNode* node);
void AkinatorDefinition (TreeNode* node, Tree* tree, enum SPEAKER SAY);
void AkinatorCompare (TreeNode** node, Tree* tree, enum SPEAKER SAY);
void AkinatorVerifier (Tree* tree, TreeNode* node);
void TreeBody (TreeNode* node, FILE* file);
void AkinatorDump (Tree* tree, TreeNode* node);

#endif
