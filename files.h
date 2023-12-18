#ifndef FILES
#define FILES

#include <stdio.h>
#include "tree.h"

char* ReadText (long file_size, FILE* file);
long FileSize (FILE* file);
void ReadFile (Tree* tree);
void ReadDatabase (TreeNode** node, char* data_buf, int* tmp_count);

#endif
