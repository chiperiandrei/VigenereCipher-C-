#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALPHABET_SIZE 26
#define ASCII_MAJ_OFFSET 65
#define ASCII_MIN_OFFSET 97
#define ENGLISH_COINCIDENCE 0.063
#define MOST_USE_LETTER 4

char* read_file (char* my_file, long* length);
double compute_coincidence (int *tab, int s_length);
double compute_mutual_coincidence(int *tab, int *tab2, int s_length, int g);
int decrypt (char* my_string, int length, char* key, int key_length);
int find_index_max_tab (int* tab, int tab_size);
int print_key (char* key, int key_length);
int rez_tab (int *tab, int tab_length);


#endif
