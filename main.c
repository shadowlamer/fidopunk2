#include "bios.h"
//#link "bios.c"

#define CMD_LENGTH 128

static char cmd_buf[128];

static const char cursor[] = {0x12,0x1,'_',0x12,0x00,0x08};
static const char prompt[] = {'\n','$',' '};


int read_cmd();
void print(char *s, int len);

int main() {
  int len;
  init_stdio();
  for (;;) {
    len = read_cmd();
    print(cmd_buf, len);
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
        return len;
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
