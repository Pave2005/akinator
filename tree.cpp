#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"
#include "string.h"

int CheckAkinatorErr (Tree* tree)
{
    if (tree->status != 0)
    {
        AkinatorVerifier (tree, tree->root);
        return 1;
    }
    return 0;
}

void HandleCommandLine (int argc, const char *argv[], int* first_arg, Tree* tree)
{
    if (argc == 1)
    {
        *first_arg = NO_SAY;
        return;
    }
    else if (argc > 1)
    {
        if (strcmp ("say", argv [1]) == 0)
        {
            *first_arg = SAY;
            return;
        }
        else
        {
            tree->status |= (1 << INCORRECT_ARGV);
            return;
        }
    }
}

void AkinatorMode (Tree* tree, TreeNode** node, int first_arg, char* mode)
{
    if (strcmp ("guess", mode) == 0)
    {
        Akinator ((enum SPEAKER)first_arg, node, tree);
    }
    else if (strcmp ("definition", mode) == 0)
    {
        char object[objects_size] = {};

        PrintfPhrase (first_arg, "Enter the word I should describe\n");

        scanf ("%20s", object);
        AkinatorDefinition (*node, (enum SPEAKER)first_arg, object);
    }
    else if (strcmp ("compare", mode) == 0)
    {
        char object_1[objects_size] = {};
        char object_2[objects_size] = {};

        PrintfPhrase (first_arg, "Enter the first word\n");
        scanf ("%20s", object_1);

        PrintfPhrase (first_arg, "Enter the second word\n");
        scanf ("%20s", object_2);

        AkinatorCompare (*node, (enum SPEAKER)first_arg, object_1, object_2);
    }
}

char* ReadText (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

void AkinatorRequestNewObject (Tree* tree, TreeNode** node, const char* command)
{
    printf ("So who is that?\n");
    char answer[size_of_arrays] = "";
    scanf ("%s", answer);
    InsertNode (tree, node, answer, command);
}

void Akinator (enum SPEAKER speaker, TreeNode** node, Tree* tree)
{
    char answer[size_of_arrays] = "";
    static int start_phrase = 0;

    if (start_phrase == 0)
    {
        PrintfPhrase (speaker, "Now I'll guess the object you made!\n");
        start_phrase = 1;
    }

    if (strcmp ("Unknown...", (*node)->data) == 0)
    {
        PrintfPhrase (speaker, "I don't know who it is:/\n");

        AkinatorRequestNewObject (tree, node, NULL);

        PrintfPhrase (speaker, "I knew it!!!\nNext time I won't give in to you!\n\n");

        return;
    }

    PrintfPhrase (speaker, "This is %s?\n", (*node)->data);

    scanf ("%s", answer);
    if (strcmp ("yes", answer) == 0)
    {
        if ((*node)->left == NULL)
        {
            PrintfPhrase (speaker, "GG!\n");
            return;
        }
        Akinator (speaker, &((*node)->left), tree);
    }
    else if (strcmp ("no", answer) == 0)
    {
        if ((*node)->right == NULL)
        {
            PrintfPhrase (speaker, "I don't know who it is:/\n");

            AkinatorRequestNewObject (tree, node, answer);

            PrintfPhrase (speaker, "I knew it!!!\nNext time I won't give in to you!\n\n");

            return;
        }
        Akinator (speaker, &((*node)->right), tree);
    }
    else
    {
        PrintfPhrase (speaker, "Incorrect command entered\ntry again\n");
        Akinator (speaker, node, tree);
    }
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

long FileSize (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);
    return size;
}

char* TreeCtor (Tree* tree)
{
    tree->root = (TreeNode*)calloc (1, sizeof (TreeNode));
    tree->root->data = strdup ("Unknown...");
    tree->root->left = NULL;
    tree->root->right = NULL;

    tree->status = 0;

    tree->file = fopen ("data.txt", "r");
    long file_size = FileSize (tree->file);
    char* tmp_buf = ReadText (file_size, tree->file);
    fclose (tree->file);
    tree->file = fopen ("data.txt", "w");
    return tmp_buf;
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

void InsertNode (Tree* tree, TreeNode** node, char* answer, const char* command)
{
    if (strcmp ("Unknown...", (*node)->data) == 0)
    {
        (*node)->data = strdup (answer);
    }
    else if (strcmp ("yes", command) == 0)
    {
        CreateNewObject ((*node)->left, answer);
    }
    else if (strcmp ("no", command) == 0)
    {
        CreateNewObject ((*node)->right, answer);
    }
}

void CreateNewObject (TreeNode* node, char* answer)
{
    node = (TreeNode*)calloc (1, sizeof (TreeNode));
    node->data = strdup (answer);
    node->left = NULL;
    node->right = NULL;
}

void TreeDtor (Tree* tree, TreeNode* node)
{
    static int default_data = 0;
    if (default_data == 0)
    {
        fclose (tree->file);
        tree->status = 0;
        default_data = 1;
    }
    if (node != NULL)
    {
        free (node->data);
        TreeDtor (tree, node->left);
        TreeDtor (tree, node->right);
        free (node);
    }
}

void AkinatorCompare (TreeNode* node, enum SPEAKER speaker, char* object_1, char* object_2)
{
    if (node == NULL)
        return;

    static int status = 0;
    static int first_divergence = 0;
    static int stack_on = 0;
    static stack compare_1 = {};
    static stack compare_2 = {};

    if (stack_on == 0)
    {
        StackCtor (&compare_1, 20);
        StackCtor (&compare_2, 20);
        stack_on = 1;
    }

    if (status == 0)
    {
        if (DoWay (&compare_1, node, object_1) == 0)
        {
            PrintfPhrase (speaker, "There is no such object in the database\n");
            return;
        }
        if (DoWay (&compare_2, node, object_2) == 0)
        {
            PrintfPhrase (speaker, "There is no such object in the database\n");
            return;
        }
        status = 1;
    }

    int way_1 = enmpty_elem;
    int way_2 = enmpty_elem;

    StackPop (&compare_1, &way_1);
    if (compare_2.size != 0)
        StackPop (&compare_2, &way_2);

    if (first_divergence == 0)
    {
        if (way_1 == way_2 && way_1 == 1)
        {
            PrintfPhrase (speaker, "%s is %s and %s too\n", object_1, node->data, object_2);

            AkinatorCompare (node->left, speaker, object_1, object_2);
            return;
        }
        else if (way_1 == way_2 && way_1 == 0)
        {
            PrintfPhrase (speaker, "%s is not %s and %s too\n", object_1, node->data, object_2);

            AkinatorCompare (node->right, speaker, object_1, object_2);
            return;
        }
    }

    if ((node->left == NULL && node->right == NULL) || compare_1.size == 0)
    {
        PrintfPhrase (speaker, "GG!\n");

        StackDtor (&compare_1);
        StackDtor (&compare_2);
        return;
    }

    if (way_1 == NO)
    {
        if (first_divergence == 0)
            PrintfPhrase (speaker, "%s is not %s, but %s is\n", object_1, node->data, object_2);
        else
            PrintfPhrase (speaker, "%s is not %s, and %s too\n", object_1, node->data, object_2);

        first_divergence = 1;
        AkinatorCompare (node->right, speaker, object_1, object_2);
    }
    else if (way_1 == YES)
    {
        PrintfPhrase (speaker, "%s is %s, but %s isn't\n", object_1, node->data, object_2);

        first_divergence = 1;
        AkinatorCompare (node->left, speaker, object_1, object_2);
    }
}

int DoWay (stack* stack, TreeNode* node, char* object)
{
    if (node == NULL)
        return 0;

    if (strcmp (object, node->data) == 0)
        return 1;

    if (DoWay (stack, node->left, object) == 1)
    {
        StackPush (stack, YES);
        return 1;
    }
    if (DoWay (stack, node->right, object) == 1)
    {
        StackPush (stack, NO);
        return 1;
    }

    return 0;
}

void AkinatorDefinition (TreeNode* node, enum SPEAKER speaker, char* object)
{
    static int status = 0;
    static int stack_on = 0;
    static stack compare_1 = {};

    if (node == NULL && status == 0)
    {
        return;
    }
    else if (node == NULL && status == 1)
    {
        StackDtor (&compare_1);
        return;
    }

    if (stack_on == 0)
    {
        StackCtor (&compare_1, 20);
        stack_on = 1;
    }

    if (status == 0)
    {
        if (DoWay (&compare_1, node, object) == 0)
            PrintfPhrase (speaker, "There is no such object in the database\n");

        status = 1;
    }

    int way_elem = enmpty_elem;
    StackPop (&compare_1, &way_elem);

    if (way_elem == YES && node->left != NULL)
    {
        if (strcmp (node->left->data, object) == 0)
            PrintfPhrase (speaker, "and %s.\nGG!\n", node->data);
        else
            PrintfPhrase (speaker, "%s, ", node->data);

        AkinatorDefinition (node->left, speaker, object);
    }
    else if (way_elem == NO && node->right != NULL)
    {
        if (strcmp (node->right->data, object) == 0)
            PrintfPhrase (speaker, "and not %s.\nGG!\n", node->data);
        else
            PrintfPhrase (speaker, "not %s, ", node->data);

        AkinatorDefinition (node->right, speaker, object);
    }
}

void AkinatorDump (Tree* tree, TreeNode* node)
{
    FILE* file = fopen ("tree.dot", "w");
    fprintf (file, "digraph G { \n"
                    "node [shape = record];\n"
                    " %o ", node);
    DumpTreeNode (node, file);
    fprintf (file, "}\n");
    fclose (file);
    system ("dot -T png tree.dot -o tree.png");
}

void DumpTreeNode (TreeNode* node, FILE* file)
{
    fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = cornflowerblue "
                          " label = \" %s \"];\n",node, node->data);

    if (node->left != NULL)
        fprintf (file, "%o -> %o", node, node->left);
    else
        return;

    if (node->right != NULL)
        fprintf (file, "\n %o -> %o \n", node, node->right);
    else
        return;

    DumpTreeNode (node->left, file);
    DumpTreeNode (node->right, file);
}


void AkinatorVerifier (Tree* tree, TreeNode* node)
{
    if ((tree->status & 1) == 1)
    {
        printf ("%sIncorrect command in the command line\n", RED);
        system ("say \"Incorrect command in the command line\"");
    }

    PrintNode (node, tree);
}

void PrintfPhrase (int speaker, char* format, ...)
{
    char for_say[size_of_arrays] = {};
    char array[size_of_arrays] = {};

    va_list ptr;
    va_start(ptr, format);
    vprintf(format, ptr);
    vsprintf (array, format, ptr);
    va_end(ptr);
    sprintf (for_say, "say \"%s\"", array);
    if (speaker == SAY)
        system (for_say);
}
