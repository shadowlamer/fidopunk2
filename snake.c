#include "snake.h"
#include <string.h>

t_point snake_body[SNAKE_MAX_SIZE + 1];
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

void snake_init() {
  for (int i=0; i<SNAKE_MIN_SIZE; i++) {
    snake_body[i].x = SCR_CHAR_WIDTH / 2;
    snake_body[i].y = SCR_CHAR_HEIGHT / 2 + i;
  }
  num_encrypted_chars = 0;
  current_char = 0;
}

void snake_draw_body() {
    for (int i = 0; i < snake_size; i++) {
        set_attr(snake_body[i].x, snake_body[i].y, SNAKE_BODY_ATTR);
    }      
}

void snake_move() {
    set_attr(snake_body[snake_size - 1].x, snake_body[snake_size - 1].y, DEFAULT_ATTR);
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

void snake_control(char c) {
  switch(c) {
    case 'w':
    case 'W':
      if (snake_direction != DIR_DOWN) snake_direction = DIR_UP;
      break;
    case 's':
    case 'S':
      if (snake_direction != DIR_UP) snake_direction = DIR_DOWN;
      break;
    case 'a':
    case 'A':
      if (snake_direction != DIR_RIGHT) snake_direction = DIR_LEFT;
      break;
    case 'd':
    case 'D':
      if (snake_direction != DIR_LEFT) snake_direction = DIR_RIGHT;
      break;
  }
}

t_snake_collision check_collision() {
    for (int i = 1; i < snake_size; i++) {
      if (snake_body[i].x == encrypted_chars[current_char].x && snake_body[i].y == encrypted_chars[current_char].y) {
         return COLL_CHAR; 
      }
      if (snake_body[0].x == snake_body[i].x && snake_body[0].y == snake_body[i].y) {
         return COLL_SELF;
        }
    }
    return COLL_NONE;
}

char wait_key(int t) {
    char c = 0;
    for (t; t > 0; t--) {
        if (c == 0) c = getkey();
        __asm
          ei
          halt
        __endasm;  
    }
    return c;
}

unsigned char snake_loop() {
    putchar_at('#', encrypted_chars[current_char].x, encrypted_chars[current_char].y, HIGHLITED_CHAR_ATTR);
    while (1) {
        snake_move();
        snake_draw_body();
        switch (check_collision()) {
          case COLL_SELF:
            return 1;
          case COLL_CHAR:
            putchar_at(encrypted_chars[current_char].symbol, encrypted_chars[current_char].x, encrypted_chars[current_char].y, DEFAULT_ATTR);
            current_char++;
            putchar_at('#', encrypted_chars[current_char].x, encrypted_chars[current_char].y, HIGHLITED_CHAR_ATTR);
            set_cursor(0, 0);
            printf("%d,%d", encrypted_chars[current_char].x, encrypted_chars[current_char].y);
       if (snake_size < SNAKE_MAX_SIZE) {
           snake_size++;
       }
        }
        snake_control(wait_key(10));
    }
  return 0;
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
      if (num_encrypted_chars < MAX_ENCRYPTED_CHARS && (random() & 0x0f > 3) && cursor->x < SCR_CHAR_WIDTH) {
      encrypted_chars[num_encrypted_chars].x = cursor->x;
      encrypted_chars[num_encrypted_chars].y = SCR_CHAR_HEIGHT - 1;
      encrypted_chars[num_encrypted_chars].symbol = c;
      c = '_';
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


unsigned char snake_run(char *text) {
    snake_init(); 
    puts_broken(text);
    shuffle_array();
    return snake_loop();
}
