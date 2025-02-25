#include <string.h>

#include "scr_addr.h"

typedef struct {
  signed char x;
  signed char y;
} t_point;


#define EMBEDDED_FONT_START 0x3c00

#define CURSOR '_'
#define DEFAULT_ATTR 0b00000100
#define CURSOR_ATTR 0b10000100

#define MAX_CMD_LEN 128
#define MAX_PARAMS 16

#define PROMPT "$ "

__at (SCREEN_BUFFER_START) char screen_buf[SCREEN_BUFFER_SIZE];
__at (ATTR_SCREEN_BUFFER_START) char screen_attr_buf[ATTR_SCREEN_BUFFER_SIZE];
__at (EMBEDDED_FONT_START) char font[];

static t_point cursor = {.x=-1, .y=-1};

static char cmd_buf[MAX_CMD_LEN];
static char *argv[MAX_PARAMS];
static unsigned char argc; 

void cls();
void putchar(char c, unsigned char x, unsigned char y, char attr);
void putchar_at_cursor(char c);
void set_cursor(unsigned char x, unsigned char y);
void hide_cursor();
void cursor_left();
void cursor_right();
void printz(char *s, unsigned char x, unsigned char y);
void scroll();
char getchar();
int read_cmd();


int main() {
  char *p;
  cls();
  for (;;) {
    read_cmd();
    p = cmd_buf;
    p = strtok(p, " ");
    printz(p, 0, 0);
  }
  return 0;
}

void putchar(char c, unsigned char x, unsigned char y, char attr) {
  char *p_font = font;
  char *p_char = p_font + (c * 8);
  char *p_scr = (char *)screen_line_addrs[y * 8];
  p_scr += x;
  for (unsigned char i = 0; i < 8; i++) {
    *(p_scr + i * 256) = *p_char;
    p_char++;
  }
  screen_attr_buf[y * 32 + x] = attr;
}

void cursor_left() {
  hide_cursor();
  cursor.x--;
  if (cursor.x < 0) {
    cursor.y--;
    cursor.x = SCR_CHAR_WIDTH - 1;
  }
  set_cursor(cursor.x, cursor.y);
}

void cursor_right() {
  cursor.x++;
  if (cursor.x >= SCR_CHAR_WIDTH) {
    cursor.x = 0;
    scroll();
  }
  set_cursor(cursor.x, cursor.y);
}

void putchar_at_cursor(char c) {
  putchar(c, cursor.x, cursor.y, DEFAULT_ATTR);
  cursor_right();
}

void set_cursor(unsigned char x, unsigned char y) {
  hide_cursor();
  cursor.x = x;
  cursor.y = y;
  putchar (CURSOR, x, y, CURSOR_ATTR); 
  
}

void hide_cursor() {
  if (cursor.x >=0 
      && cursor.y < SCR_CHAR_HEIGHT 
      && cursor.x >= 0 
      && cursor.x < SCR_CHAR_WIDTH) {
  putchar(' ', cursor.x, cursor.y, DEFAULT_ATTR);
  }
}

void cls() {
  memset(screen_buf, 0x00, SCREEN_BUFFER_SIZE);
  memset(screen_attr_buf, DEFAULT_ATTR, ATTR_SCREEN_BUFFER_SIZE);
}


void printz(char *s, unsigned char x, unsigned char y) {
  for (char *p = s; *p != 0x00; p++) {
    if (x >= SCR_CHAR_WIDTH) {
      scroll();
      x = 0;
    }
    putchar(*p, x, y, DEFAULT_ATTR);
    x++;
  }
}

int read_cmd() {

  int len = 0;
  char key;
  printz(PROMPT, 0, SCR_CHAR_HEIGHT - 1);
  set_cursor(strlen(PROMPT), SCR_CHAR_HEIGHT - 1);
  
  for (;;) {
    key = getchar();
    switch (key) {
      case 0x0d:  
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
        if (key >= 0x20 && key < 0x80) {
          putchar_at_cursor(key);
          cmd_buf[len] = key;
          len++;
        }
    }
  } 
}


void scroll() {
  __asm
    ld b, #SCR_CHAR_HEIGHT
    call 0x0dfe
  __endasm;  
}

char getchar() {
  __asm
getc_wait_key: 
  ei
  halt
  ld a,#0b00001000
  ld (0x5C6A), a  

  ld a, #0
  ld (0x5C41), a  
  ld a, (0x5C3B)
  or a, #0b00001000
  ld (0x5C3B), a  
  call 0x02BF  
  ld a, (0x5C3B)
  and a, #0b00100000
  jr z, getc_wait_key  
  ld a, (0x5C08)
  ld l, a  
  ld a, (0x5C3B)
  and a, #0b11011111
  ld (0x5C3B), a  
  __endasm;  
}
