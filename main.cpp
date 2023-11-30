#include <stdio.h>
#include "tree.h"

int main (int argc, const char *argv[])
{
    Tree node = {};
    int first_argv = -1;
    int second_argv = -1;
    HandleCommandLine (argc, argv, &first_argv, &second_argv, &node);

    char* data_buf = TreeCtor (&node);

    int tmp_count = 0;
    BaseOfData (&node, &(node.root), data_buf, &tmp_count);

    if (CheckAkinatorErr (&node) == 1)
        return 0;

    AkinatorMode (&node, &(node.root), first_argv, second_argv);

    if (CheckAkinatorErr (&node) == 1)
        return 0;

    PrintfNode (node.root, &node);

    TreeDtor (&node, node.root);
}
