#ifndef __SNAKE_H
#define __SNAKE_H

#include "scr_addr.h"
#include "keyboard.h"
#include "io.h"

#define SNAKE_MIN_SIZE 3
#define MAX_ENCRYPTED_CHARS 50
#define SNAKE_MAX_SIZE (SNAKE_MIN_SIZE + MAX_ENCRYPTED_CHARS)

#define SNAKE_BODY_ATTR 0b00001100
#define HIGHLITED_CHAR_ATTR 0b10000010

#define SNAKE_HEAD snake.body[0]

typedef enum {
  DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} t_direction;

typedef enum {
  RES_DONE, RES_FAIL, RES_BREAK
} t_restore_result;

typedef enum {
  COLL_NONE, COLL_SELF, COLL_CHAR
} t_snake_collision;

typedef struct {
  unsigned char x;
  unsigned char y;
  char symbol;
} t_encrypted_char;

typedef struct {
  int size;
  t_direction direction;
  t_point body[SNAKE_MAX_SIZE + 1];
} t_snake;

void snake_grow();

t_restore_result snake_run(char *text);

#endif //__SNAKE_H
