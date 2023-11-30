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
    printf ("Так кто же это?\n");
    char answer[100] = "";
    scanf ("%s", answer);
    Insert (tree, node, answer, command);
}

void Akinator (enum INSTR COMMAND, enum PASS_NUM PASS, enum SPEAKER SPEAKER, TreeNode** node, Tree* tree)
{
    char for_say[100] = {};
    char first_answer[100] = "";
    if (tree->counter == 0)
    {
        if (COMMAND == DEFINITION)
        {
            printf ("Сейчас я угадаю объект, который вы загадали! И дам ему определение!\n");
            if (SPEAKER == SAY)
                system ("say \"Сейчас я угадаю объект, который вы загадали! И дам ему определение!\"");
        }
        else if (COMMAND == COMPARE)
        {
            printf ("Сейчас я отгадаю два объекта! А потом сравню их!\n");
            if (SPEAKER == SAY)
                system ("say \"Сейчас я отгадаю два объекта! А потом сравню их!\"");
        }
        else if (COMMAND == GUESS)
        {
            printf ("Сейчас я угадаю объект, который вы загадали!\n");
            if (SPEAKER == SAY)
                system ("say \"Сейчас я угадаю объект, который вы загадали!\"");
        }
        tree->counter++;
    }
    if (strcmp ("неизвестно кто...", (*node)->data) == 0)
    {
        printf ("Я не знаю кто это:/\n");
        if (SPEAKER == SAY)
            system ("say \"Я не знаю кто это\"");
        AkinatorInput (tree, node, NULL);
        printf ("Я так и знал!!!\nВ следующий раз я не поддамя тебе!\n\n");
        if (SPEAKER == SAY)
            system ("say \"Я так и знал. В следующий раз я не поддамя тебе!\"");
        tree->counter = 0;
        return;
    }
    printf ("Это %s?\n", (*node)->data);
    if (SPEAKER == SAY)
    {
        strcat (for_say, "say \"Это ");
        strcat (for_say, (*node)->data);
        strcat (for_say, " \"");
        system (for_say);
    }

    scanf ("%s", first_answer);
    if (strcmp ("да", first_answer) == 0)
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
                printf ("Хорошая была игра!\n");
                if (SPEAKER == SAY)
                    system ("say \"Хорошая была игра!\"");
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
    else if (strcmp ("нет", first_answer) == 0)
    {
        if ((*node)->right == NULL)
        {
            StackPush (&(tree->compare_1), 0);
            printf ("Я не знаю кто это:/\n");
            if (SPEAKER == SAY)
                system ("say \"Я не знаю кто это\"");
            AkinatorInput (tree, node, first_answer);
            StackPush (&(tree->compare_1), 0);
            printf ("Я так и знал!!!\nВ следующий раз я не поддамя тебе!\n\n");
            if (SPEAKER == SAY)
                system ("say \"Я так и знал. В следующий раз я не поддамя тебе!\"");
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
        printf ("Введена неправильная комманда\nпопробуйте еще раз\n");
        if (SPEAKER == SAY)
        {
            system ("say \"Введена неправильная комманда\nпопробуйте еще раз\"");
        }
        Akinator (COMMAND, PASS, SPEAKER, node, tree);
    }
}

void BaseOfData (Tree* tree, TreeNode** node, char* data_buf, int* tmp_count)
{
    int count = 0;
    char tmp_elem[100] = {};
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
        printf ("OK\n");
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
    tree->root->data = strdup ("неизвестно кто...");
    tree->root->left = NULL;
    tree->root->right = NULL;

    tree->counter = 0;
    tree->status = 0;

    StackCtor (&(tree->compare_1), 20);
    StackCtor (&(tree->compare_2), 20);

    tree->file = fopen ("data.txt", "r");
    long file_size = FileSize (tree->file);
    printf ("%lu\n", file_size);
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
        if (tree->object_1 == NULL)
            tree->object_1 = (*node)->left->data;
        else
            tree->object_2 = (*node)->left->data;
    }
    else if (strcmp ("нет", command) == 0)
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
    char for_say[100] = {}; // подумать над динамическим выделением
    if (tree->counter == 0)
    {
        Akinator (COMPARE, FIRST_PASS, SPEAKER, node, tree);
        printf ("Теперь я попытаюсь отгадать второй объект, с которым вы хотите сравнить уже отгаданный...\n");
        if (SPEAKER == SAY)
            system ("say \"Теперь я попытаюсь отгадать второй объект, с которым вы хотите сравнить уже отгаданный.\"");
        Akinator (COMPARE, SECOND_PASS, SPEAKER,  node, tree);
        printf ("Сейчас я сравню %s и %s:\n", tree->object_1, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"Сейчас я сравню ");
            strcat (for_say, tree->object_1);
            strcat (for_say, " и ");
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
            printf ("%s - %s и %s тоже\n", tree->object_1, (*node)->data, tree->object_2);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"");
                strcat (for_say, tree->object_1);
                strcat (for_say, " это ");
                strcat (for_say, (*node)->data);
                strcat (for_say, " и ");
                strcat (for_say, tree->object_2);
                strcat (for_say, "тоже\"");
                system (for_say);
            }
            AkinatorCompare (&((*node)->left), tree, SPEAKER);
            return;
        }
        else if (way_1 == way_2 && way_1 == 0 && (*node)->left != NULL && (*node)->right != NULL)
        {
            printf ("%s - не %s и %s тоже\n", tree->object_1, (*node)->data, tree->object_2);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"");
                strcat (for_say, tree->object_1);
                strcat (for_say, " это не ");
                strcat (for_say, (*node)->data);
                strcat (for_say, " и ");
                strcat (for_say, tree->object_2);
                strcat (for_say, "тоже\"");
                system (for_say);
            }
            AkinatorCompare (&((*node)->right), tree, SPEAKER);
            return;
        }
        tree->counter++;
    }
    if ((((*node)->left == NULL && (*node)->right == NULL)) || tree->compare_1.size == 0)
    {
        printf ("Хорошая была игра!");
        if (SPEAKER == SAY)
            system ("say \"Хорошая была игра!\"");
        StackDtor (&(tree->compare_1));
        StackDtor (&(tree->compare_2));
        tree->counter = 0;
        return;
    }

    if (way_1 == 0)
    {
        printf ("%s - не %s, и %s тоже\n", tree->object_1, (*node)->data, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " это не ");
            strcat (for_say, (*node)->data);
            strcat (for_say, " и ");
            strcat (for_say, tree->object_2);
            strcat (for_say, "тоже\"");
            system (for_say);
        }
        AkinatorCompare (&((*node)->right), tree, SPEAKER);
    }
    else if (way_1 == 1)
    {
        printf ("%s - %s, а %s нет\n", tree->object_1, (*node)->data, tree->object_2);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " это ");
            strcat (for_say, (*node)->data);
            strcat (for_say, " а ");
            strcat (for_say, tree->object_2);
            strcat (for_say, "нет\"");
            system (for_say);
        }
        AkinatorCompare (&((*node)->left), tree, SPEAKER);
    }
}

void AkinatorDefinition (TreeNode* node, Tree* tree, enum SPEAKER SPEAKER)
{
    char for_say[100] = {};
    if (tree->counter == 0)
    {
        Akinator (DEFINITION, FIRST_PASS, SPEAKER, &node, tree);
        if (tree->status != 0)
        {
            return;
        }
        printf ("\n%s - это ", tree->object_1);
        if (SPEAKER == SAY)
        {
            strcat (for_say, "say \"");
            strcat (for_say, tree->object_1);
            strcat (for_say, " это \"");
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
            printf ("и %s.\nХорошая была игра!", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"и ");
                strcat (for_say, node->data);
                strcat (for_say, " Хорошая была игра!\"");
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
            printf ("и не %s.\nХорошая была игра!", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"и не ");
                strcat (for_say, node->data);
                strcat (for_say, " Хорошая была игра!\"");
                system (for_say);
            }
        }
        else
        {
            printf ("не %s, ", node->data);
            if (SPEAKER == SAY)
            {
                strcat (for_say, "say \"не ");
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

void AkinatorVerifier (Tree* tree, TreeNode* node)
{
    if ((tree->status & 1) == 1)
    {
        printf ("%sIncorrect command in the command line\n", RED);
        system ("say \"Incorrect command in the command line\"");
    }

    PrintfNode (node, tree);
}
