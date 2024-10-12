#ifndef LIB_H
#define LIB_H

#define CONCAT_IMPL( x, y ) x##y
#define MAC( x, y ) CONCAT_IMPL( x, y )
#define f(function, string) int MAC(temp_macro_var, __LINE__)[] = string;function(MAC(temp_macro_var, __LINE__))

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

#endif
