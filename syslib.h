#ifndef SYSLIB_H
#define SYSLIB_H

void clearScreen() {
    f(syssend, "CLRS");
}

void setCursorPos(int x, int y) {
    f(syssend, "SCPX");
    write_short(x);
    f(syssend, "SCPY");
    write_short(y);
}

int getWidth() {
    f(syssend, "GETW");
    return read_short();
}

int getHeight() {
    f(syssend, "GETH");
    return read_short();
}

int getCursorX() {
    f(syssend, "GCPX");
    return read_short();
}

int getCursorY() {
    f(syssend, "GCPY");
    return read_short();
}

void print(int[] string) {
    f(syssend, "PRNT");
    syssend(string);
    sysend();
}

void read(int[] output, int size) {
    f(syssend, "READ");
    sysread(output, size);
}

bool fsexists(int[] name, int[] currentDir) {
    f(syssend, "EDIR");
    syssend(currentDir);
    syssend(name);
    sysend();
    bool ret = read_char()==1;
    return ret;
}

int fslist(int[] name, int[] currentDir) {
    f(syssend, "LSSZ");
    syssend(currentDir);
    syssend(name);
    sysend();
    int ret = read_char();
    return ret;
}

int fslist1dir(int[] name) {
    f(syssend, "LSSZ");
    syssend(name);
    sysend();
    int ret = read_char();
    return ret;
}

void fsgetindex(int[] currentDir, int index, int[] out, int size) {
    f(syssend, "GTLS");
    write_char(index);
    syssend(currentDir);
    sysend();
    sysread(out, size);
}

int fsopen(int[] currentDir, int[] filename, int mode1, int mode2) {
    f(syssend, "OPEN");
    write_char(mode1);
    write_char(mode2);
    syssend(currentDir);
    syssend(filename);
    sysend();
    return read_char();
}

void fsclose(int fileslot) {
    f(syssend, "CLOS");
    write_char(fileslot);
}

void fsread(int fileslot, int size, int[] out) {
    f(syssend, "REAB");
    write_char(fileslot);
    write_short(size);
    sysread(out, size);
}

void fswrite(int fileslot, int[] string) {
    f(syssend, "WRTE");
    write_char(fileslot);
    syssend(string);
    sysend();
}

int fssize(int[] currentDir, int[] filename) {
    f(syssend, "SIZE");
    syssend(currentDir);
    syssend(filename);
    sysend();
    return read_short();
}

void fsmove(int[] currentDir, int[] fileA, int[] fileB) {
    f(syssend, "FMOV");
    int i=0;
    while (fileA[i]!=0) {
        i = i + 1;
    }
    write_char(i);
    syssend(currentDir);
    syssend(fileA);
    syssend(currentDir);
    syssend(fileB);
    sysend();
}

void fscopy(int[] currentDir, int[] fileA, int[] fileB) {
    f(syssend, "FCPY");
    int i=0;
    while (fileA[i]!=0) {
        i = i + 1;
    }
    write_char(i);
    syssend(currentDir);
    syssend(fileA);
    syssend(currentDir);
    syssend(fileB);
    sysend();
}

void execInDir(int[] currentDir, int[] command) {
    f(syssend, "EXEC");
    syssend(currentDir);
    syssend(command);
    sysend();
}

void exec(int[] file) {
    f(syssend, "EXEC");
    syssend(file);
    sysend();
}

void getArg(int index, int[] out, int size) {
    f(syssend, "GARG");
    write_char(index);
    sysread(out, size);
}

void makeDir(int[] currentDir, int[] string) {
    f(syssend, "MDIR");
    syssend(currentDir);
    syssend(string);
    sysend();
}

void deletePath(int[] currentDir, int[] string) {
    f(syssend, "DELF");
    syssend(currentDir);
    syssend(string);
    sysend();
}

#endif
