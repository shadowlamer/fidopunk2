#include <string.h>

#include "scr_addr.h"

#include "keyboard.h"
//#link "keyboard.c"

#include "io.h"
//#link "io.c"


#define MAX_CMD_LEN 128
#define MAX_PARAMS 16

#define PROMPT "$ "

static char cmd_buf[MAX_CMD_LEN];
static char *argv[MAX_PARAMS];
static unsigned char argc; 


int read_cmd();


int main() {
  char *p;
#include "filler.h"
  cls();
  for (;;) {
    read_cmd();
    p = cmd_buf;
    p = strtok(p, " ");
    printz(p, 0, 0);
  }
  return 0;
}


int read_cmd() {

  int len = 0;
  char key;
  printz(PROMPT, 0, SCR_CHAR_HEIGHT - 1);
  set_cursor(strlen(PROMPT), SCR_CHAR_HEIGHT - 1);
  
  for (;;) {
    key = getchar();
    switch (key) {
      case '\n':  
        cmd_buf[len] = 0x00;
        hide_cursor();
        scroll();
        return len + 1;
      case 0x08:
        if (len <= 0) break;
        cursor_left();
        len--;
        break;
      default:  
        if (key >= 0x20 && key < 0x80 && len < MAX_CMD_LEN) {
          putchar_at_cursor(key);
          cmd_buf[len] = key;
          len++;
        }
    }
  } 
}

