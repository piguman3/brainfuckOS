#include "lib.h"
#include "syslib.h"

#define DIR_SIZE 128

int currentDir[DIR_SIZE];
getArg(1, currentDir, DIR_SIZE);

int dirName[DIR_SIZE];
getArg(2, dirName, DIR_SIZE);

//Read first 255 bytes of a file
if (dirName[0]!=0) {
    int file = fsopen(currentDir, dirName, 'r', 'b');
    int contents[255];
    fsread(file, 255, contents); 
    print(contents);
    fsclose(file);
}