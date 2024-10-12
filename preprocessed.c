void syssend(int[] s) {
    int i = 0;
    while(s[i] != 0) {
        write_char(s[i]);
        i = i + 1;
    }
}
void sysread(int[] s, int size) {
    int i = 0;
    int rchar = -1;
    while(rchar != 0 && i < size) {
        rchar = read_char();
        s[i] = rchar;
        i = i + 1;
    }
}
int read_short() {
    return read_char() * 256 + read_char();
}
void write_short(int s) {
    write_char(0);
    write_char(s);
}
void sysend() {
    write_char(0);
}
int cmpstr(int[] a, int[] b, int checksize) {
    int ia = 0;
    int ib = 0;
    while(a[ia]!=0 && b[ib]!=0 && a[ia] == b[ib] && ia<checksize-1) {
        ia = ia + 1;
        ib = ib + 1;
    }
    return a[ia]-b[ib];
}
void clearScreen() {
    int temp_macro_var5[] = "CLRS";syssend(temp_macro_var5);
}
void setCursorPos(int x, int y) {
    int temp_macro_var9[] = "SCPX";syssend(temp_macro_var9);
    write_short(x);
    int temp_macro_var11[] = "SCPY";syssend(temp_macro_var11);
    write_short(y);
}
int getWidth() {
    int temp_macro_var16[] = "GETW";syssend(temp_macro_var16);
    return read_short();
}
int getHeight() {
    int temp_macro_var21[] = "GETH";syssend(temp_macro_var21);
    return read_short();
}
int getCursorX() {
    int temp_macro_var26[] = "GCPX";syssend(temp_macro_var26);
    return read_short();
}
int getCursorY() {
    int temp_macro_var31[] = "GCPY";syssend(temp_macro_var31);
    return read_short();
}
void print(int[] string) {
    int temp_macro_var36[] = "PRNT";syssend(temp_macro_var36);
    syssend(string);
    sysend();
}
void read(int[] output, int size) {
    int temp_macro_var42[] = "READ";syssend(temp_macro_var42);
    sysread(output, size);
}
bool fsexists(int[] name, int[] currentDir) {
    int temp_macro_var47[] = "EDIR";syssend(temp_macro_var47);
    syssend(currentDir);
    syssend(name);
    sysend();
    bool ret = read_char()==1;
    return ret;
}
int fslist(int[] name, int[] currentDir) {
    int temp_macro_var56[] = "LSSZ";syssend(temp_macro_var56);
    syssend(currentDir);
    syssend(name);
    sysend();
    int ret = read_char();
    return ret;
}
int fslist1dir(int[] name) {
    int temp_macro_var65[] = "LSSZ";syssend(temp_macro_var65);
    syssend(name);
    sysend();
    int ret = read_char();
    return ret;
}
void fsgetindex(int[] currentDir, int index, int[] out, int size) {
    int temp_macro_var73[] = "GTLS";syssend(temp_macro_var73);
    write_char(index+1);
    syssend(currentDir);
    sysend();
    sysread(out, size);
}
int fsopen(int[] currentDir, int[] filename, int mode1, int mode2) {
    int temp_macro_var81[] = "OPEN";syssend(temp_macro_var81);
    write_char(mode1);
    write_char(mode2);
    syssend(currentDir);
    syssend(filename);
    sysend();
    return read_char();
}
void fsclose(int fileslot) {
    int temp_macro_var91[] = "CLOS";syssend(temp_macro_var91);
    write_char(fileslot);
}
void fsread(int fileslot, int size, int[] out) {
    int temp_macro_var96[] = "REAB";syssend(temp_macro_var96);
    write_char(fileslot);
    write_short(size);
    sysread(out, size);
}
void fswrite(int fileslot, int[] string) {
    int temp_macro_var103[] = "WRTE";syssend(temp_macro_var103);
    write_char(fileslot);
    syssend(string);
    sysend();
}
int fssize(int[] currentDir, int[] filename) {
    int temp_macro_var110[] = "SIZE";syssend(temp_macro_var110);
    syssend(currentDir);
    syssend(filename);
    sysend();
    return read_short();
}
void fsmove(int[] currentDir, int[] fileA, int[] fileB) {
    int temp_macro_var118[] = "FMOV";syssend(temp_macro_var118);
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
    int temp_macro_var132[] = "FCPY";syssend(temp_macro_var132);
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
    int temp_macro_var146[] = "EXEC";syssend(temp_macro_var146);
    syssend(currentDir);
    syssend(command);
    sysend();
}
void exec(int[] file) {
    int temp_macro_var153[] = "EXEC";syssend(temp_macro_var153);
    syssend(file);
    sysend();
}
void getArg(int index, int[] out, int size) {
    int temp_macro_var159[] = "GARG";syssend(temp_macro_var159);
    write_char(index);
    sysread(out, size);
}
void makeDir(int[] currentDir, int[] string) {
    int temp_macro_var165[] = "MDIR";syssend(temp_macro_var165);
    syssend(currentDir);
    syssend(string);
    sysend();
}
void deletePath(int[] currentDir, int[] string) {
    int temp_macro_var172[] = "DELF";syssend(temp_macro_var172);
    syssend(currentDir);
    syssend(string);
    sysend();
}
int currentDir[128];
getArg(1, currentDir, 128);
int text[128];
getArg(3, text, 128);
int filename[128];
getArg(2, filename, 128);
if (text[0]!=0 && filename[0]!=0) {
    int file = fsopen(currentDir, filename, 'a', 'b');
    fswrite(file, text);
    fsclose(file);
}
