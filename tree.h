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
};

enum INSTR
{
    GUESS = 1,
    INPUT = 2,
    COMPARE = 3,
};

char* ReadText (long file_size, FILE* file);
long FileSize (FILE* file);
void TreeCtor (TreeNode** node);
void PrintfNode (const TreeNode* node, const Tree* tree);
void ReadFile (Tree* tree);
void Insert (TreeNode** node, char* answer, const char* command);
void AkinatorInput (Tree** node, const char* command);
void Akinator (enum INSTR COMMAND ,TreeNode** node);
void BaseOfData (TreeNode** node, char* data_buf, int* tmp_count);
