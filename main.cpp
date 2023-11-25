#include <stdio.h>
#include "tree.h"

int main ()
{
    Tree node = {};
    TreeCtor (&(node.root));
    node.file = fopen ("data.txt", "r");
    long file_size = FileSize (node.file);
    char* data_buf = ReadText (file_size, node.file);
    int tmp_count = 0;
    BaseOfData (&(node.root), data_buf, &tmp_count);
    fclose (node.file);

    node.file = fopen ("data.txt", "w");
    Akinator (GUESS ,&(node.root));
    PrintfNode (node.root, &node);
    fclose (node.file);
}
