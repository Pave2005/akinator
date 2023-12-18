#include "files.h"
#include "tree.h"

char* ReadText (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

long FileSize (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);
    return size;
}

void ReadDatabase (TreeNode** node, char* data_buf, int* tmp_count)
{
    int count = 0;
    char tmp_elem[size_of_arrays] = {};

    sscanf (data_buf + *tmp_count, "%s%n", tmp_elem, &count);

    *tmp_count += count;
    while (strcmp (")", tmp_elem) == 0)
    {
        sscanf (data_buf + *tmp_count, "%s%n", tmp_elem, &count);
        *tmp_count += count;
    }

    if (strcmp ("(", tmp_elem) == 0)
    {
        if (*tmp_count != count)
        {
            *node = (TreeNode*)calloc (1, sizeof (TreeNode));
        }
        sscanf (data_buf + *tmp_count, "%s%n", tmp_elem, &count);
        *tmp_count += count;
    }
    if (strcmp ("nil", tmp_elem) == 0)
    {
        *node = NULL;
        return;
    }
    if (strcmp ("\0", tmp_elem) == 0)
    {
        return;
    }
    (*node)->data = strdup (tmp_elem);

    ReadDatabase (&((*node)->left), data_buf, tmp_count);
    ReadDatabase (&((*node)->right), data_buf, tmp_count);
}

void PrintNode (const TreeNode* node, const Tree* tree)
{
    if (node == NULL)
    {
        fprintf (tree->file, " nil ");
        return;
    }
    fprintf (tree->file, " ( ");
    fprintf (tree->file, " %s ", node->data);
    PrintNode (node->left, tree);
    PrintNode (node->right, tree);
    fprintf (tree->file, " ) ");
}
