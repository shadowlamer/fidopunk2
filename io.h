#ifndef __IO_H
#define __IO_H

#include <string.h>

#include "scr_addr.h"
#include "keyboard.h"

typedef struct {
  signed char x;
  signed char y;
} t_point;


#define EMBEDDED_FONT_START 0x3c00

#define CURSOR '_'
#define DEFAULT_ATTR 0b00000100
#define CURSOR_ATTR 0b10000100

void cls();
void putchar_at(char c, unsigned char x, unsigned char y, char attr);
void putchar_at_cursor(char c);
int  putchar(int c);
void puts(const char *str);
void print_int(int n);
void set_cursor(unsigned char x, unsigned char y);
void hide_cursor();
void cursor_left();
void cursor_right();
void printf(char *s, ...);
void scroll();
char getchar();
int  isspace(int c);
void new_line();
t_point *get_cursor();
void set_attr(unsigned char x, unsigned char y, char attr);

#endif // __IO_H
