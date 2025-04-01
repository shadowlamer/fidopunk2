#include "snake.h"
#include <string.h>

__at (ATTR_SCREEN_BUFFER_START) char attributes[ATTR_SCREEN_BUFFER_SIZE];

#define SNAKE_MAX_SIZE 50
#define SNAKE_MIN_SIZE 3
#define MAX_ENCRYPTED_CHARS 100

typedef enum {
  DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} t_direction;

typedef struct {
  unsigned char x;
  unsigned char y;
  char symbol;
} t_encrypted_char;

t_point snake_body[SNAKE_MAX_SIZE];
int snake_size = SNAKE_MIN_SIZE;
t_direction snake_direction = DIR_UP;
t_encrypted_char encrypted_chars[MAX_ENCRYPTED_CHARS];
int current_char;
int num_encrypted_chars;
static unsigned int seed = 12345;

unsigned int random() {
  __asm
  ld hl, (_seed)
  ld a,r
  ld d,a
  ld e,(hl)
  add hl,de
  add a,l
  xor h
  ld (_seed),hl
  __endasm;
}

void highlight_char(int index) {
    attributes[encrypted_chars[index].y * SCR_CHAR_WIDTH + encrypted_chars[index].x] = 0b10000010;
}

void snake_init() {
  for (int i=0; i<SNAKE_MIN_SIZE; i++) {
    snake_body[i].x = SCR_CHAR_WIDTH / 2;
    snake_body[i].y = SCR_CHAR_HEIGHT / 2;
  }
  num_encrypted_chars = 0;
  current_char = 0;
}

void snake_draw_body() {
    for (int i = 0; i < snake_size; i++) {
        attributes[snake_body[i].y * SCR_CHAR_WIDTH + snake_body[i].x] = 0b00001100;
    }      
}

void snake_move() {
    attributes[snake_body[snake_size - 1].y * SCR_CHAR_WIDTH + snake_body[snake_size - 1].x] = DEFAULT_ATTR;
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
    case 'w':
    case 'W':
      snake_direction = DIR_UP;
      break;
    case 's':
    case 'S':
      snake_direction = DIR_DOWN;
      break;
    case 'a':
    case 'A':
      snake_direction = DIR_LEFT;
      break;
    case 'd':
    case 'D':
      snake_direction = DIR_RIGHT;
      break;
  }
}

check_collision() {
    if (snake_body[0].x == encrypted_chars[current_char].x &&
       snake_body[0].y == encrypted_chars[current_char].y) {
       putchar_at(encrypted_chars[current_char].symbol, encrypted_chars[current_char].x, encrypted_chars[current_char].y, DEFAULT_ATTR);
       current_char++;
        if (snake_size < SNAKE_MAX_SIZE)  snake_size++;
    }
}

void snake_loop() {
    while (current_char < MAX_ENCRYPTED_CHARS) {
        snake_move();
        snake_draw_body();
        highlight_char(current_char);
        check_collision();
        for (int t = 0; t < 10; t++) {
            snake_control();
            __asm
              ei
              halt
            __endasm;  
        }
    }
}

// Функция для перемешивания массива в случайном порядке
void shuffle_array() {
  t_encrypted_char temp;
    for (int i = num_encrypted_chars - 1; i > 0; i--) {
        // Генерируем случайный индекс от 0 до i включительно
        int j = random() % (i + 1);

        // Меняем местами элементы с индексами i и j
        temp.x = encrypted_chars[i].x;
        temp.y = encrypted_chars[i].y;
        temp.symbol = encrypted_chars[i].symbol;
        encrypted_chars[i].x = encrypted_chars[j].x;
        encrypted_chars[i].y = encrypted_chars[j].y;
        encrypted_chars[i].symbol = encrypted_chars[j].symbol;
        encrypted_chars[j].x = temp.x;
        encrypted_chars[j].y = temp.y;
        encrypted_chars[j].symbol = temp.symbol;
    }
}

void new_line_broken() {
  t_point *cursor = get_cursor();
  hide_cursor();
  cursor->x = 0;
  scroll();
  for (int i = 0; i < num_encrypted_chars; i++) encrypted_chars[i].y--;
}


void puts_broken(const char *str) {
  t_point *cursor = get_cursor();
    while (*str) {
      char c = *str++;
  switch (c) {
    case '\n':
      new_line_broken();
      break;
    default:
      if (num_encrypted_chars < MAX_ENCRYPTED_CHARS && (random() & 0x0f > 3)) {
      encrypted_chars[num_encrypted_chars].x = get_cursor()->x;
      encrypted_chars[num_encrypted_chars].y = SCR_CHAR_HEIGHT - 1;
      encrypted_chars[num_encrypted_chars].symbol = c;
      c = '#';
      num_encrypted_chars++;
      }
      putchar_at(c, cursor->x, cursor->y, DEFAULT_ATTR);
      cursor->x++;
      if (cursor->x >= SCR_CHAR_WIDTH) {
        new_line_broken();
      }
      set_cursor(cursor->x, cursor->y);
  }
    }
}


void snake_run(char *text) {
    snake_init(); 
    puts_broken(text);
    shuffle_array();
    snake_loop();
}
