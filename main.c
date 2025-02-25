#include "bios.h"
//#link "bios.c"

#define CMD_LENGTH 128
#define MAX_ARGS 16

static char cmd_buf[CMD_LENGTH];
static char* argv[MAX_ARGS];
static int argc = 0;

static const char cursor[] = {0x12,0x1,'_',0x12,0x00,0x08};
static const char prompt[] = {'\n','$',' '};


int read_cmd();
void parse_cmd(int len);
void print(char *s, int len);
void printz(char *s);

int main() {
  int len;
  init_stdio();
  for (;;) {
    len = read_cmd();
    parse_cmd(len);
    printz(argv[0]);
  }
}

int read_cmd() {
  int len = 0;
  char key;
  print(prompt, 3);
  print(cursor, 6);

  for (;;) {
    key = getchar();
    switch (key) {
      case 0x0d:  
        putchar (' ');
        cmd_buf[len] = 0x00;
        return len + 1;
      case 0x08:
        if (len <= 0) break;
        len--;
        putchar (' ');
        putchar(0x08);
        putchar(0x08);
        print(cursor, 6);
        break;
      default:  
        putchar(key);
        print(cursor, 6);
        cmd_buf[len] = key;
        len++;
    }
  } 
}

void print(char *s, int len) {
  for (int i=0; i<len; i++) putchar(s[i]);
}

void printz(char *s) {
    for (char *p = s; *p != 0x00; p++) putchar(*p);
}

void parse_cmd(int len) {
  char *p = cmd_buf;
  argv[0] = cmd_buf;
  argc = 1;
  for (int i = 0; i < len && cmd_buf[i] != 0x00; i++) {
    if (cmd_buf[i] == ' ') {
      cmd_buf[i] = 0x00;
      argv[argc++] = p;
      p = cmd_buf + i;
    }
  }
}
