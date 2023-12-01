#include <stdio.h>
#include <stdlib.h>
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
void HandleCommandLine (int argc, const char *argv[], int* first_argv, int* second_argv, Tree* tree)
{
    if (argc > 1)
    {
        if (strcmp ("guess", argv [1]) == 0)
        {
            *first_argv = GUESS;
        }
        else if (strcmp ("def", argv [1]) == 0)
        {
            *first_argv = DEFINITION;
        }
        else if (strcmp ("compare", argv [1]) == 0)
        {
            *first_argv = COMPARE;
        }
        else
        {
            tree->status |= (1 << INCORRECT_ARGV);
            return;
        }
        if (argc == 2)
        {
            *second_argv = NO_SAY;
                return;
        }
        if (argc > 2)
        {
            if (strcmp ("say", argv [2]) == 0)
            {
                *second_argv = SAY;
                return;
            }
            else
            {
                tree->status |= (1 << INCORRECT_ARGV);
                return;
            }
        }
    }
    tree->status |= (1 << INCORRECT_ARGV);
}

void AkinatorMode (Tree* tree, TreeNode** node, int first_argv, int second_argv)
{
    if (first_argv == GUESS)
    {
        if (second_argv == SAY)
            Akinator (GUESS, DEFAULT, SAY, node, tree);
        else if (second_argv == NO_SAY)
            Akinator (GUESS, DEFAULT, NO_SAY, node, tree);
    }
    else if (first_argv == DEFINITION)
    {
        if (second_argv == SAY)
            AkinatorDefinition (*node, tree, SAY);
        else if (second_argv == NO_SAY)
            AkinatorDefinition (*node, tree, NO_SAY);
    }
    else if (first_argv == COMPARE)
    {
        if (second_argv == SAY)
            AkinatorCompare (node, tree, SAY);
        else if (second_argv == NO_SAY)
            AkinatorCompare (node, tree, NO_SAY);
    }
}

char* ReadText (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

void AkinatorInput (Tree* tree, TreeNode** node, const char* command)
{
    printf ("So who is that?\n");
    char answer[size_of_arrays] = "";
    scanf ("%s", answer);
    Insert (tree, node, answer, command);
}

void Akinator (enum INSTR COMMAND, enum PASS_NUM PASS, enum SPEAKER SPEAKER, TreeNode** node, Tree* tree)
{
    char for_say[size_of_arrays] = {};
    char first_answer[size_of_arrays] = "";
    if (tree->counter == 0)
    {
        if (COMMAND == DEFINITION)
        {
            printf ("Now I'll guess the object you made and define it!\n");
            if (SPEAKER == SAY)
                system ("say \"Now I'll guess the object you made and define it!\"");
        }
        else if (COMMAND == COMPARE)
        {
            printf ("Now I'll guess two objects! And then I'll compare them!\n");
            if (SPEAKER == SAY)
                system ("say \"Now I'll guess two objects! And then I'll compare them!\"");
        }
        else if (COMMAND == GUESS)
        {
            printf ("Now I'll guess the object you made!\n");
            if (SPEAKER == SAY)
                system ("say \"Now I'll guess the object you made!\"");
        }
        tree->counter++;
    }
    if (strcmp ("Unknown...", (*node)->data) == 0)
    {
        printf ("I don't know who it is:/\n");
        if (SPEAKER == SAY)
            system ("say \"I don't know who it is\"");
        AkinatorInput (tree, node, NULL);
        printf ("I knew it!!!\nNext time I won't give in to you!\n\n");
        if (SPEAKER == SAY)
            system ("say \"I knew it. Next time I won't give in to you!\"");
        tree->counter = 0;
        return;
    }
    printf ("This %s?\n", (*node)->data);
    if (SPEAKER == SAY)
    {
        strcat (for_say, "say \"This ");
        strcat (for_say, (*node)->data);
        strcat (for_say, " \"");
        system (for_say);
    }

    scanf ("%s", first_answer);
    if (strcmp ("yes", first_answer) == 0)
    {
        if ((*node)->left == NULL)
        {
            if (PASS == FIRST_PASS)
            {
                tree->object_1 = (*node)->data;
                StackPush (&(tree->compare_1), 1);
            }
            else if (PASS == SECOND_PASS)
            {
                tree->object_2 = (*node)->data;
                StackPush (&(tree->compare_2), 1);
            }
            if (COMMAND == GUESS)
            {
                printf ("GG!\n");
                if (SPEAKER == SAY)
                    system ("say \"GG!\""); // good game
            }
            tree->counter = 0;
            return;
        }
        if (PASS == FIRST_PASS)
        {
            Akinator (COMMAND, FIRST_PASS, SPEAKER, &((*node)->left), tree);
            StackPush (&(tree->compare_1), 1);
        }
        else if (PASS == SECOND_PASS)
        {
            Akinator (COMMAND, SECOND_PASS, SPEAKER, &((*node)->left), tree);
            StackPush (&(tree->compare_2), 1);
        }
        else
        {
            Akinator (COMMAND, DEFAULT, SPEAKER, &((*node)->left), tree);
        }
    }
    else if (strcmp ("no", first_answer) == 0)
    {
        if ((*node)->right == NULL)
        {
            StackPush (&(tree->compare_1), 0);
            printf ("I don't know who it is:/\n");
            if (SPEAKER == SAY)
                system ("say \"I don't know who it is\"");
            AkinatorInput (tree, node, first_answer);
            StackPush (&(tree->compare_1), 0);
            printf ("I knew it!!!\nNext time I won't give in to you!\n\n");
            if (SPEAKER == SAY)
                system ("say \"I knew it. Next time I won't give in to you!\"");
            tree->counter = 0;
            return;
        }
        if (PASS == FIRST_PASS)
        {
            Akinator (COMMAND, FIRST_PASS, SPEAKER, &((*node)->right), tree);
            StackPush (&(tree->compare_1), 0);
        }
        else if (PASS == SECOND_PASS)
        {
            Akinator (COMMAND, SECOND_PASS, SPEAKER, &((*node)->right), tree);
            StackPush (&(tree->compare_2), 0);
        }
        else
        {
            Akinator (COMMAND, DEFAULT, SPEAKER, &((*node)->right), tree);
        }
    }
    else
    {
        printf ("Incorrect command entered\ntry again\n");
        if (SPEAKER == SAY)
        {
            system ("say \"Incorrect command entered\ntry again\"");
        }
        Akinator (COMMAND, PASS, SPEAKER, node, tree);
    }
}

void BaseOfData (Tree* tree, TreeNode** node, char* data_buf, int* tmp_count) // если не нужно будет - убрать tree
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
    if (strcmp ("\0", tmp_elem) == 0) // спросить у криса
    {
        return;
    }
    (*node)->data = strdup (tmp_elem);
    BaseOfData (tree, &((*node)->left), data_buf, tmp_count);
    BaseOfData (tree, &((*node)->right), data_buf, tmp_count);
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

    tree->counter = 0;
    tree->status = 0;

    StackCtor (&(tree->compare_1), 20);
    StackCtor (&(tree->compare_2), 20);

    tree->file = fopen ("data.txt", "r");
    long file_size = FileSize (tree->file);
    char* tmp_buf = ReadText (file_size, tree->file);
    fclose (tree->file);
    tree->file = fopen ("data.txt", "w");
    return tmp_buf;
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

void Insert (Tree* tree, TreeNode** node, char* answer, const char* command)
{
    if (strcmp ("Unknown...", (*node)->data) == 0)
    {
        (*node)->data = strdup (answer);
    }
    else if (strcmp ("yes", command) == 0)
    {
        (*node)->left = (TreeNode*)calloc (1, sizeof (TreeNode));
        (*node)->left->data = strdup (answer);
        (*node)->left->left = NULL;
        (*node)->left->right = NULL;
        if (tree->object_1 == NULL)
            tree->object_1 = (*node)->left->data;
        else
            tree->object_2 = (*node)->left->data;
    }
    else if (strcmp ("no", command) == 0)
    {
        (*node)->right = (TreeNode*)calloc (1, sizeof (TreeNode));
        (*node)->right->data = strdup (answer);
        (*node)->right->left = NULL;
        (*node)->right->right = NULL;

        if (tree->object_1 == NULL)
            tree->object_1 = (*node)->right->data;
        else
            tree->object_2 = (*node)->right->data;
    }
}

void TreeDtor (Tree* tree, TreeNode* node)
{
    if (tree->counter == 0)
    {
        fclose (tree->file);
        tree->object_1 = NULL;
        tree->object_2 = NULL;
        tree->status = 0;
        tree->counter++;
    }
    if (node != NULL)
    {
        free (node->data);
        TreeDtor (tree, node->left);
        TreeDtor (tree, node->right);
        free (node);
        tree->counter = 0;
    }
}

void AkinatorCompare (TreeNode** node, Tree* tree, enum SPEAKER SPEAKER)
{
    char for_say[size_of_arrays] = {};
    if (tree->counter == 0)
    {
        Akinator (COMPARE, FIRST_PASS, SPEAKER, node, tree);
        printf ("Now I'll try to guess the second object with which you want to compare the already guessed one...\n");
        if (SPEAKER == SAY)
            system ("say \"Now I'll try to guess the second object with which you want to compare the already guessed one.\"");
        Akinator (COMPARE, SECOND_PASS, SPEAKER,  node, tree);
        printf ("I'll compare now %s and %s:\n", tree->object_1, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"I'll compare now ");
            strcat (for_say, tree->object_1);
            strcat (for_say, " and ");
            strcat (for_say, tree->object_2);
            strcat (for_say, "\"");
            system (for_say);
        }
        tree->counter++;
    }
    int way_1 = -1;
    int way_2 = -2;
    StackPop (&(tree->compare_1), &way_1);
    if (tree->compare_2.size != 0)
        StackPop (&(tree->compare_2), &way_2);

    if (tree->counter == 1)
    {
        if (way_1 == way_2 && way_1 == 1 && (*node)->left != NULL && (*node)->right != NULL)
        {
            printf ("%s - %s and  %s too\n", tree->object_1, (*node)->data, tree->object_2);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"");
                strcat (for_say, tree->object_1);
                strcat (for_say, " is ");
                strcat (for_say, (*node)->data);
                strcat (for_say, " and ");
                strcat (for_say, tree->object_2);
                strcat (for_say, "too\"");
                system (for_say);
            }
            AkinatorCompare (&((*node)->left), tree, SPEAKER);
            return;
        }
        else if (way_1 == way_2 && way_1 == 0 && (*node)->left != NULL && (*node)->right != NULL)
        {
            printf ("%s is not %s and %s too\n", tree->object_1, (*node)->data, tree->object_2);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"");
                strcat (for_say, tree->object_1);
                strcat (for_say, " is not ");
                strcat (for_say, (*node)->data);
                strcat (for_say, " and ");
                strcat (for_say, tree->object_2);
                strcat (for_say, "too\"");
                system (for_say);
            }
            AkinatorCompare (&((*node)->right), tree, SPEAKER);
            return;
        }
        tree->counter++;
    }
    if ((((*node)->left == NULL && (*node)->right == NULL)) || tree->compare_1.size == 0)
    {
        printf ("GG!");
        if (SPEAKER == SAY)
            system ("say \"GG!\"");
        StackDtor (&(tree->compare_1));
        StackDtor (&(tree->compare_2));
        tree->counter = 0;
        return;
    }

    if (way_1 == 0)
    {
        printf ("%s is not %s, and %s too\n", tree->object_1, (*node)->data, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " is not ");
            strcat (for_say, (*node)->data);
            strcat (for_say, " and ");
            strcat (for_say, tree->object_2);
            strcat (for_say, "too\"");
            system (for_say);
        }
        AkinatorCompare (&((*node)->right), tree, SPEAKER);
    }
    else if (way_1 == 1)
    {
        printf ("%s - %s, but %s isn't\n", tree->object_1, (*node)->data, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " is ");
            strcat (for_say, (*node)->data);
            strcat (for_say, " but ");
            strcat (for_say, tree->object_2);
            strcat (for_say, "isn't\"");
            system (for_say);
        }
        AkinatorCompare (&((*node)->left), tree, SPEAKER);
    }
}

void AkinatorDefinition (TreeNode* node, Tree* tree, enum SPEAKER SPEAKER)
{
    char for_say[size_of_arrays] = {};
    if (tree->counter == 0)
    {
        Akinator (DEFINITION, FIRST_PASS, SPEAKER, &node, tree);
        if (tree->status != 0)
        {
            return;
        }
        printf ("\n%s - is ", tree->object_1);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " is \"");
            system (for_say);
        }
        tree->counter++;
    }
    int way = -1;
    StackPop (&(tree->compare_1), &way);

    if (way == 1 && node->left != NULL)
    {
        if (strcmp (node->left->data, tree->object_1) == 0)
        {
            printf ("and %s.\nGG!", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"and ");
                strcat (for_say, node->data);
                strcat (for_say, " GG!\"");
                system (for_say);
            }
        }
        else
        {
            printf ("%s, ", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"");
                strcat (for_say, node->data);
                strcat (for_say, "\"");
                system (for_say);
            }
        }
        AkinatorDefinition (node->left, tree, SPEAKER);
    }
    else if (way == 0 && node->right != NULL)
    {
        if (strcmp (node->right->data, tree->object_1) == 0)
        {
            printf ("and not %s.\nGG!", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"and not ");
                strcat (for_say, node->data);
                strcat (for_say, " GG!\"");
                system (for_say);
            }
        }
        else
        {
            printf ("not %s, ", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"not ");
                strcat (for_say, node->data);
                strcat (for_say, "\"");
                system (for_say);
            }
        }
        AkinatorDefinition (node->right, tree, SPEAKER);
    }
    else if (node->left == NULL || node->right == NULL)
    {
        StackDtor (&(tree->compare_1));
        tree->counter = 0;
        return;
    }
}

void AkinatorDump (Tree* tree, TreeNode* node)
{
    FILE* file = fopen ("tree.dot", "w");
    fprintf (file, "digraph G { \n"
                    "node [shape = record];\n"
                    " %o ", node);
    TreeBody (node, file);
    fprintf (file, "}\n");
    fclose (file);
    system ("dot -T png tree.dot -o tree.png");
}

void TreeBody (TreeNode* node, FILE* file)
{
    fprintf (file, " %o [shape = doubleoctagon, style = filled, fillcolor = cornflowerblue "
                          " label = \" %s \"];\n",node, node->data);
    if (node->left != NULL)
    {
        fprintf (file, "%o -> %o", node, node->left);
    }
    else
    {
        return;
    }
    if (node->right != NULL)
    {
        fprintf (file, "\n %o -> %o \n", node, node->right);
    }
    else
    {
        return;
    }

    TreeBody (node->left, file);
    TreeBody (node->right, file);
}


void AkinatorVerifier (Tree* tree, TreeNode* node)
{
    if ((tree->status & 1) == 1)
    {
        printf ("%sIncorrect command in the command line\n", RED);
        system ("say \"Incorrect command in the command line\"");
    }

    PrintfNode (node, tree);
}
