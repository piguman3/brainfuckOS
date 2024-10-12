#include "lib.h"
#include "syslib.h"

#define DIR_SIZE 128

int currentDir[DIR_SIZE];
getArg(1, currentDir, DIR_SIZE);

int dirName[DIR_SIZE];
getArg(2, dirName, DIR_SIZE);

//Remove path
if (dirName[0]!=0) {
    deletePath(currentDir, dirName);
}