#ifndef __SNAKE_H
#define __SNAKE_H

#include "scr_addr.h"
#include "keyboard.h"
#include "io.h"

#define SNAKE_MIN_SIZE 3
#define MAX_ENCRYPTED_CHARS 100
#define SNAKE_MAX_SIZE (SNAKE_MIN_SIZE + MAX_ENCRYPTED_CHARS)

#define SNAKE_BODY_ATTR 0b00001100
#define HIGHLITED_CHAR_ATTR 0b10000010

typedef enum {
  DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} t_direction;

typedef enum {
  COLL_NONE, COLL_SELF, COLL_CHAR
} t_snake_collision;

typedef struct {
  unsigned char x;
  unsigned char y;
  char symbol;
} t_encrypted_char;

unsigned char snake_run(char *text);

#endif //__SNAKE_H
