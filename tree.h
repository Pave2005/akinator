#ifndef AKINATOR
#define AKINATOR

#include "../stack/stack_cd.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


#define RED "\x1b[31;41m"

const int size_of_arrays = 200;
const int enmpty_elem = -1;
const int objects_size = 21;

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

    long long status;
};

enum AKINATOR_ERRORS
{
    INCORRECT_ARGV = 0,
};

enum WAY
{
    FIRST_WAY = 1,
    SECOND_WAY = 2,
    DEFAULT_WAY = 3,
};

enum STACK_WAY
{
    YES = 1,
    NO = 0,
};

enum SPEAKER
{
    SPEAKER_OFF = 1,
    SPEAKER_ON = 0,
};

void CreateNewObject (TreeNode* node, char* answer);
int DoWay (stack* stack, TreeNode* node, char* object);
int CheckAkinatorErr (Tree* tree);
void AkinatorMode (TreeNode** node, int first_arg, char* mode);
void HandleCommandLine (int argc, const char *argv[], int* first_arg, Tree* tree);
char* TreeCtor (Tree* tree);
void PrintNode (const TreeNode* node, const Tree* tree);
void InsertNode (TreeNode** node, char* answer, const char* command);
void AkinatorRequestNewObject (TreeNode** node, const char* command);
void Akinator (int speaker, TreeNode** node);
void TreeDtor (Tree* tree, TreeNode* node);
void AkinatorDefinition (TreeNode* node, int speaker, char* object);
void AkinatorCompare (TreeNode* node, int SPEAKER, char* object_1, char* object_2);
void AkinatorVerifier (Tree* tree, TreeNode* node);
void DumpTreeNode (TreeNode* node, FILE* file);
void AkinatorDump (Tree* tree, TreeNode* node);
void PrintfPhrase (int speaker, char* format, ...);

#endif
