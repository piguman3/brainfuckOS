void print(int[] s) {
    int i = 0;
    while(s[i] != 0) {
        write_char(s[i]);
        i = i + 1;
    }
}

void print_small_int(int x) {
    write_char('0' + x);
}

int PRNT[] = "PRNT";
int string[] = "PRNTInput 2 1-digit numbers:\n";
print(string);
write_char(0);
int string2[] = "READ";
print(string2);
int a = read_char() - '0';
print(string2);
int b = read_char() - '0';

int result = a + b;
int string4[] = "PRNTa + b = ";
print(string4);

if (result>9) {
    int last = result - 10;
    print_small_int(1);
    print_small_int(last);
} else {
    print_small_int(result);
}

write_char('\n');
write_char(0);