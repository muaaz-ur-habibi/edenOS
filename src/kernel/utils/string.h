#ifndef KUTILSSTR
#define KUTILSSTR

#include <stddef.h>
#include "memory.h"
#include "../drivers/vga/vga.h"

int k_strlen(char *str);
int k_strcmp(char *str1, char *str2);
char **k_strsplt(char *str, char delim, int *toks_n);
void k_strrev(char *str);
char *k_inttostr(int n);
int k_strtoint(char *str);
char *k_hextostr(int h);

#endif