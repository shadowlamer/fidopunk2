#include "snake.h"
#include <string.h>

static unsigned int seed = 12345;
static t_encrypted_char encrypted_chars[MAX_ENCRYPTED_CHARS + 1];

t_snake snake;


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
  snake.size = SNAKE_MIN_SIZE;
  snake.direction = DIR_UP;
  for (int i = 0; i < snake.size && i < SNAKE_MAX_SIZE; i++) {
    snake.body[i].x = SCR_CHAR_WIDTH / 2;
    snake.body[i].y = SCR_CHAR_HEIGHT / 2 + i;
  }
}

void snake_draw_body() {
    for (int i = 0; i < snake.size; i++) {
        set_attr(snake.body[i].x, snake.body[i].y, SNAKE_BODY_ATTR);
    }      
}

void snake_move() {
    set_attr(snake.body[snake.size - 1].x, snake.body[snake.size - 1].y, DEFAULT_ATTR);
    for (int i = snake.size - 1; i > 0; i--) {
        snake.body[i].y = snake.body[i - 1].y;
        snake.body[i].x = snake.body[i - 1].x;
    }      
    switch (snake.direction) {
      case DIR_UP:
        if (SNAKE_HEAD.y == 0) {
          SNAKE_HEAD.y = SCR_CHAR_HEIGHT - 1;        
        } else {
          SNAKE_HEAD.y--;        
        }
        break;
      case DIR_DOWN:
        if (SNAKE_HEAD.y == SCR_CHAR_HEIGHT - 1) {
          SNAKE_HEAD.y = 0;        
        } else {
          SNAKE_HEAD.y++;        
        }
        break;
      case DIR_LEFT:
        if (SNAKE_HEAD.x == 0) {
          SNAKE_HEAD.x = SCR_CHAR_WIDTH - 1;        
        } else {
          SNAKE_HEAD.x--;        
        }
        break;
      case DIR_RIGHT:
        if (SNAKE_HEAD.x == SCR_CHAR_WIDTH - 1) {
          SNAKE_HEAD.x = 0;        
        } else {
          SNAKE_HEAD.x++;        
        }
        break;
    }
}

void snake_control(char c) {
  switch(c) {
    case 'w': case 'W':
      if (snake.direction != DIR_DOWN) snake.direction = DIR_UP;
      break;
    case 's': case 'S':
      if (snake.direction != DIR_UP) snake.direction = DIR_DOWN;
      break;
    case 'a': case 'A':
      if (snake.direction != DIR_RIGHT) snake.direction = DIR_LEFT;
      break;
    case 'd': case 'D':
      if (snake.direction != DIR_LEFT) snake.direction = DIR_RIGHT;
      break;
  }
}

t_snake_collision check_collision(t_encrypted_char *current_char) {
    for (int i = 1; i < snake.size; i++) {
      if (snake.body[i].x == current_char->x && snake.body[i].y == current_char->y) {
         return COLL_CHAR; 
      }
      if (SNAKE_HEAD.x == snake.body[i].x && SNAKE_HEAD.y == snake.body[i].y) {
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

void put_bug(int current_char) {
  putchar_at('#', encrypted_chars[current_char].x, encrypted_chars[current_char].y, HIGHLITED_CHAR_ATTR);
}

t_restore_result snake_loop(int num_encrypted_chars) {
    int current_char = 0;
    char key;
    put_bug(current_char);
    while (current_char < num_encrypted_chars) {
        snake_move();
        snake_draw_body();
        switch (check_collision(encrypted_chars + current_char)) {
          case COLL_SELF:
            return RES_FAIL;
          case COLL_CHAR:
            putchar_at(encrypted_chars[current_char].symbol, encrypted_chars[current_char].x, encrypted_chars[current_char].y, DEFAULT_ATTR);
            current_char++;
            put_bug(current_char);
            set_cursor(0, SCR_CHAR_HEIGHT - 1);
            printf("%d of %d restored at %d,%d", current_char, num_encrypted_chars,  encrypted_chars[current_char].x, encrypted_chars[current_char].y);
            snake_grow();
            continue;
        }
        key = wait_key(10);
        if (key == ' ') return RES_BREAK;
        snake_control(key);
    }
  return RES_DONE;
}

// Функция для перемешивания массива в случайном порядке
void shuffle_array(int num_encrypted_chars) {
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

void new_line_broken(int num_encrypted_chars) {
  t_point *cursor = get_cursor();
  hide_cursor();
  cursor->x = 0;
  scroll();
  for (int i = 0; i < num_encrypted_chars; i++) encrypted_chars[i].y--;
}


int puts_broken(const char *str) {
  t_point *cursor = get_cursor();
  int num_encrypted_chars = 0;
    while (*str) {
      char c = *str++;
      switch (c) {
      case '\n':
        new_line_broken(num_encrypted_chars);
        break;
      default:
        if (num_encrypted_chars < MAX_ENCRYPTED_CHARS && (random() & 0x0f > 4)) {
        encrypted_chars[num_encrypted_chars].x = cursor->x;
        encrypted_chars[num_encrypted_chars].y = SCR_CHAR_HEIGHT - 1;
        encrypted_chars[num_encrypted_chars].symbol = c;
        c = '_';
        num_encrypted_chars++;
        }
        putchar_at(c, cursor->x, cursor->y, DEFAULT_ATTR);
        cursor->x++;
        if (cursor->x >= SCR_CHAR_WIDTH) {
          new_line_broken(num_encrypted_chars);
        }
        set_cursor(cursor->x, cursor->y);
      }
    }
  return num_encrypted_chars;
}

t_restore_result snake_run(char *text) {
    int num_encrypted_chars;
    snake_init(); 
    num_encrypted_chars = puts_broken(text);
    new_line_broken(num_encrypted_chars);
    new_line_broken(num_encrypted_chars);
    shuffle_array(num_encrypted_chars);
    return snake_loop(num_encrypted_chars);
}

void snake_grow() {
  if (snake.size < SNAKE_MAX_SIZE) snake.size++;
}
