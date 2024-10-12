void print(int[] s) {
    int i = 0;
    while(s[i] != 0) {
        write_char(s[i]);
        i = i + 1;
    }
}

void read(int[] s, int size) {
    int i = 0;
    int rchar = -1;
    while(rchar != 0 || i < size) {
        rchar = read_char();
        s[i] = rchar;
        i = i + 1;
    }
}

int read_short() {
    return read_char() * 256 + read_char();
}

void write_short(int s) {
    if (s>255) {
        write_char(255);
        write_char(s-256);
    } else {
        write_char(0);
        write_char(s);
    }
}

int sys[] = "OPENrthello.txt";
print(sys);
write_char(0);
int file = read_char();
int sys2[] = "REAB";
print(sys2);
write_char(file);
write_short(20);
int out[20];
read(out, 20);
int sys4[] = "CLOS";
print(sys4);
write_char(file);
int sys5[] = "PRNT";
print(sys5);
print(out);
write_char(0);
