#include <stdio.h>
#include "tree.h"

int main (int argc, const char *argv[])
{
    Tree node = {};
    int first_arg = -1;
    char mode[11] = {};
    printf ("Choose the mode in which you want to start the game:\n"
            "Write \"definition\" if you want me to give the difinition to the object!\n\n"
            "Write \"guess\" if you want me to guess the intended object!\n\n"
            "Write \"compare\" if you want me to compare two objeects!\n\n");
    scanf ("%s", mode);

    HandleCommandLine (argc, argv, &first_arg, &node);

    char* data_buf = TreeCtor (&node);

    int tmp_count = 0;
    ReadDatabase (&(node.root), data_buf, &tmp_count);

    if (CheckAkinatorErr (&node) == 1)
        return 0;

    AkinatorDump (&node, node.root);

    AkinatorMode (&node, &(node.root), first_arg, mode);

    if (CheckAkinatorErr (&node) == 1)
        return 0;

    PrintNode (node.root, &node);

    TreeDtor (&node, node.root);
}
