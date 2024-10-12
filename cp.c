#include "lib.h"
#include "syslib.h"

#define DIR_SIZE 128

int currentDir[DIR_SIZE];
getArg(1, currentDir, DIR_SIZE);

int fileA[DIR_SIZE];
getArg(2, fileA, DIR_SIZE);

int fileB[DIR_SIZE];
getArg(3, fileB, DIR_SIZE);

//Copy path
if (fileA[0]!=0 && fileB[0]!=0) {
    fscopy(currentDir, fileA, fileB);
}