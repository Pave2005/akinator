#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "string.h"

char* ReadText (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

void AkinatorInput (TreeNode** node, const char* command)
{
    printf ("Так кто же это?\n");
    char answer[100] = "";
    scanf ("%s", answer);
    Insert (node, answer, command);
}

void Akinator (enum INSTR COMMAND ,TreeNode** node)
{

    char first_answer[100] = "";
    printf ("\nСейчас я угадаю объект, который вы загадали!\n");
    if (strcmp ("неизвестно кто...", (*node)->data) == 0)
    {
        printf ("Я не знаю кто это:/\n");
        AkinatorInput (node, NULL);
        printf ("Я так и знал!!!\nВ следующий раз я не поддамя тебе!\n\n");
        return;
    }

    printf ("Это %s?\n", (*node)->data);
    scanf ("%s", first_answer);

    if (strcmp ("да", first_answer) == 0)
    {
        if (COMMAND == GUESS && (*node)->left == NULL)
        {
            printf ("Хорошая была игра!\n");
            return;
        }
        if ((*node)->left == NULL)
        {
            printf ("Я не знаю кто это:/\n");
            AkinatorInput (node, first_answer);
            printf ("Я так и знал!!!\nВ следующий раз я не поддамя тебе!\n\n");
            return;
        }
        Akinator (COMMAND ,&((*node)->left));
    }
    else if (strcmp ("нет", first_answer) == 0)
    {
        if ((*node)->right == NULL)
        {
            printf ("right = NULL\n");
            printf ("Я не знаю кто это:/\n");
            AkinatorInput (node, first_answer);
            printf ("Я так и знал!!!\nВ следующий раз я не поддамя тебе!\n\n");
            return;
        }
        Akinator (COMMAND ,&((*node)->right));
    }
}

void BaseOfData (TreeNode** node, char* data_buf, int* tmp_count)
{
    int count = 0;
    char tmp_elem[100] = {};
    sscanf (data_buf + *tmp_count, "%s%n", tmp_elem, &count);
    *tmp_count += count;
    int k = 0;
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
    BaseOfData (&((*node)->left), data_buf, tmp_count);
    BaseOfData (&((*node)->right), data_buf, tmp_count);
}

long FileSize (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);
    return size;
}

void TreeCtor (TreeNode** node)
{
    *node = (TreeNode*)calloc (1, sizeof (TreeNode));
    (*node)->data = strdup ("неизвестно кто...");
    (*node)->left = NULL;
    (*node)->right = NULL;
}

void PrintfNode (const TreeNode* node, const Tree* tree)
{
    if (node == NULL)
    {
        fprintf (tree->file, " nil ");
        return;
    }
    fprintf (tree->file, " ( ");
    fprintf (tree->file, " %s ", node->data);
    PrintfNode (node->left, tree);
    PrintfNode (node->right, tree);
    fprintf (tree->file, " ) ");
}

void Insert (TreeNode** node, char* answer, const char* command)
{
    if (strcmp ("неизвестно кто...", (*node)->data) == 0)
    {
        (*node)->data = strdup (answer);
    }
    else if (strcmp ("да", command) == 0)
    {
        (*node)->left = (TreeNode*)calloc (1, sizeof (TreeNode));
        (*node)->left->data = strdup (answer);
        (*node)->left->left = NULL;
        (*node)->left->right = NULL;
    }
    else if (strcmp ("нет", command) == 0)
    {
        (*node)->right = (TreeNode*)calloc (1, sizeof (TreeNode));
        (*node)->right->data = strdup (answer);
        (*node)->right->left = NULL;
        (*node)->right->right = NULL;
    }
}
