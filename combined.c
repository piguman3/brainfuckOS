#include "lib.h"
#include "syslib.h"

#define CMD_SIZE 64

#define DIR_SIZE 128
int currentDir[DIR_SIZE];

//Set position to 1, 1, print motd
clearScreen();
setCursorPos(0, 0);
f(print, "BoluDOS v1.0\nFoots be damned\n");

//Commands
int CD[] = "cd";
int LS[] = "ls";
int CLR[] = "clr";

//Main loop
while (true) {
    print(currentDir);
    f(print, "> "); //Cursor
    
    int command[CMD_SIZE];
    read(command, CMD_SIZE);

    int args[CMD_SIZE];
    int i = 0;
    bool notcalled = true; //Check if any of the base commands where called
    while(command[i] != ' ') {
        i = i + 1;
    }
    i = i + 1;
    for (int x=i; , x<CMD_SIZE-1, x=x+1;) { //Copy args, everything after the first space char.
        args[x-i] = command[x];
    }

    if (cmpstr(CD, command, 2)==0) {
        notcalled = false;
        int dotdot[] = "..";
        int f = 0;
        bool isdotdot = cmpstr(dotdot, args, CMD_SIZE)==0;
        if (isdotdot) {
            f = DIR_SIZE-1;
            int count = 0;
            while(f>=0 && count < 2) {
                if (f-1>=0) {
                    if (currentDir[f-1] == '/') { //Remove all text until we reach /
                        count = count + 1;
                    }
                }
                currentDir[f] = 0;
                f = f - 1;
            }
        } 
        if (!isdotdot) {
            while(args[f]!=0 && f<CMD_SIZE) {
                f = f + 1;
            }
            f = f - 1;
            while(args[f]=='/' && f>=0) { //Remove last slashes
                args[f] = 0;
                f = f - 1;
            }
            f = f + 1;
            args[f] = '/'; //Add / at the end of the directory
            int offsetslashes = 0;
            while(args[offsetslashes]=='/' && offsetslashes<CMD_SIZE) { //Remove first slashes by adding offset
                offsetslashes = offsetslashes + 1;
            }
            int argsfixed[CMD_SIZE];
            int fixiter = 0;
            int fixcycle = 0;
            bool slashoff = true;
            while(args[fixiter]!=0 && fixiter<CMD_SIZE) { //Remove first slashes by adding offset
                if (args[fixiter]=='/') {
                    if (slashoff) {
                        argsfixed[fixcycle] = '/';
                        fixcycle = fixcycle + 1;
                    }
                    slashoff = false;
                } else {
                    argsfixed[fixcycle] = args[fixiter];
                    fixcycle = fixcycle + 1;
                    slashoff = true;
                }
                fixiter = fixiter + 1;
            }
            if (fsexists(argsfixed, currentDir)) {
                f = 0;
                while(currentDir[f] != 0) {
                    f = f + 1;
                }
                int max = CMD_SIZE-offsetslashes;
                for (int x=f; , x<max, x=x+1;) { //Copy args
                    currentDir[x] = argsfixed[x-f+offsetslashes];
                }
            } 
        }
    } 
    
    if (cmpstr(LS, command, 2)==0) {
        notcalled = false;
        int filecount = fslist1dir(currentDir);
        for (int x=0; , x<filecount, x=x+1;) {
            int filename[DIR_SIZE];
            fsgetindex(currentDir, x, filename, DIR_SIZE);
            f(syssend, "PRNT  ");
            if (filename[0]=='D') {
                f(syssend, "/");
            }
            int y=1;
            while (y<DIR_SIZE && filename[y]!=0) {
                write_char(filename[y]);
                y = y + 1;
            }
            write_char('\n');
            sysend();
            f(syssend, "SLEP");
            write_short(0);
        }
    }

    if (cmpstr(CLR, command, 3)==0) {
        notcalled = false;
        clearScreen();
        setCursorPos(0, 0);
    }

    if (notcalled) {
        execInDir(currentDir, command);
    }
}