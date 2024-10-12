#include "lib.h"
#include "syslib.h"

#define DIR_SIZE 128

int currentDir[DIR_SIZE];
getArg(1, currentDir, DIR_SIZE);

int text[DIR_SIZE];
getArg(3, text, DIR_SIZE);

int filename[DIR_SIZE];
getArg(2, filename, DIR_SIZE);

//Write file
if (text[0]!=0 && filename[0]!=0) {
    int file = fsopen(currentDir, filename, 'a', 'b');
    fswrite(file, text); 
    fsclose(file);
}