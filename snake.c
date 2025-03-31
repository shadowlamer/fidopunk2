#include "snake.h"
#include <string.h>

__at (ATTR_SCREEN_BUFFER_START) char attributes[ATTR_SCREEN_BUFFER_SIZE];

#define SNAKE_MAX_SIZE 10
#define SNAKE_MIN_SIZE 3

typedef enum {
  DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} t_direction;

t_point snake_body[SNAKE_MAX_SIZE];
int snake_size = SNAKE_MIN_SIZE;
t_direction snake_direction = DIR_UP;

void init_game() {
  for (int i=0; i<SNAKE_MIN_SIZE; i++) {
    snake_body[i].x = SCR_CHAR_WIDTH / 2;
    snake_body[i].y = SCR_CHAR_HEIGHT / 2;
  }
}

void snake_draw_body() {
    for (int i = 0; i < snake_size; i++) {
        attributes[snake_body[i].y * SCR_CHAR_WIDTH + snake_body[i].x] = 0b00001100;
    }      
}

void snake_move() {
    attributes[snake_body[snake_size - 1].y * SCR_CHAR_WIDTH + snake_body[snake_size - 1].x] = 0b00000100;
    for (int i = snake_size - 1; i > 0; i--) {
        snake_body[i].y = snake_body[i - 1].y;
        snake_body[i].x = snake_body[i - 1].x;
    }      
    switch (snake_direction) {
      case DIR_UP:
        if (snake_body[0].y == 0) {
          snake_body[0].y = SCR_CHAR_HEIGHT - 1;        
        } else {
          snake_body[0].y--;        
        }
        break;
      case DIR_DOWN:
        if (snake_body[0].y == SCR_CHAR_HEIGHT - 1) {
          snake_body[0].y = 0;        
        } else {
          snake_body[0].y++;        
        }
        break;
      case DIR_LEFT:
        if (snake_body[0].x == 0) {
          snake_body[0].x = SCR_CHAR_WIDTH - 1;        
        } else {
          snake_body[0].x--;        
        }
        break;
      case DIR_RIGHT:
        if (snake_body[0].x == SCR_CHAR_WIDTH - 1) {
          snake_body[0].x = 0;        
        } else {
          snake_body[0].x++;        
        }
        break;
    }
}

void snake_control() {
  switch(getkey()) {
    case '1':
      snake_direction = DIR_UP;
      break;
    case '2':
      snake_direction = DIR_DOWN;
      break;
    case '3':
      snake_direction = DIR_LEFT;
      break;
    case '4':
      snake_direction = DIR_RIGHT;
      break;
  }
}

void game_loop() {
    while (1) {
      snake_move();
      snake_control();
      snake_draw_body();
        delay(10);
    }
}

void delay(unsigned int t) {
  for (t;t>0;t--) {
  __asm
    ei
    halt
  __endasm;  
  }
}
