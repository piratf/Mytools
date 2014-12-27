#include <windows.h>
#include <fstream>
#include <stdio.h>

const int N = 1000;
const int LIM = 1000; //LIM means shift.

int change[N] = {};

void init(){
    int i;
    for(i = 'a'; i <= 'z'; i++)
        change[i] = 'a' + i;
    for(i = 'A'; i <= 'Z'; i++)
        change[i] = 'a' + i + LIM;
    change[','] = 188;
    change['.'] = 190;
    change['\n'] = 13;
    change[';'] = 186;
    change['\''] = 222;
    change['-'] = 189;
    change['+'] = 107;
    change['*'] = 106;
    change['/'] = 191;
    change['\\'] = 220;
    change['`'] = 192;
    change['~'] = change['`'] + LIM;
    change['!'] = '1' + LIM;
    change['@'] = '2' + LIM;
    change['#'] = '3' + LIM;
    change['$'] = '4' + LIM;
    change['%'] = '5' + LIM;
    change['^'] = '6' + LIM;
    change['&'] = '7' + LIM;
    change['*'] = '8' + LIM;
    change['('] = '9' + LIM;
    change[')'] = '0' + LIM;
    change['_'] = change['-'] + LIM;
    change['{'] = change['['] + LIM;
    change['}'] = change[']'] + LIM;
    change['<'] = change[','] + LIM;
    change['>'] = change['.'] + LIM;
    change['?'] = change['/'] + LIM;
    change[':'] = change[';'] + LIM;
    change['"'] = change['\''] + LIM;
}

int main(){
    init();
    freopen("file.txt", "r", stdin);
    puts("Now put the cursor in the right place you want to input.");
    Sleep(3000);
    puts("Start..");
    int ch;
    while((ch = getchar()) != EOF){
        printf("%c", ch);
        ch = change[ch];
        printf("%d", ch);
        if(ch > LIM){
            ch -= LIM;
            keybd_event(16, (BYTE)0, 0 ,0);
            keybd_event(ch, (BYTE)0, 0 ,0);
            keybd_event(ch, (BYTE)0, KEYEVENTF_KEYUP,0);
            keybd_event(16, (BYTE)0, KEYEVENTF_KEYUP,0);
            continue;
        }
        keybd_event(ch, (BYTE)0, 0 ,0);
        keybd_event(ch, (BYTE)0, KEYEVENTF_KEYUP,0);
        //Sleep(5);
    }
    puts("End..");
    return 0;
}