#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getMem(int size);
void *getValue(int numPage);
int setValue(int numPage, void *value, int cant);

#endif
