#include "io.h"

__at (SCREEN_BUFFER_START) char screen_buf[SCREEN_BUFFER_SIZE];
__at (ATTR_SCREEN_BUFFER_START) char screen_attr_buf[ATTR_SCREEN_BUFFER_SIZE];
__at (EMBEDDED_FONT_START) char font[];

static t_point cursor = {.x=-1, .y=-1};

void putchar_at(char c, unsigned char x, unsigned char y, char attr) {
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
  putchar_at(c, cursor.x, cursor.y, DEFAULT_ATTR);
  cursor_right();
}

void set_cursor(unsigned char x, unsigned char y) {
  hide_cursor();
  cursor.x = x;
  cursor.y = y;
  putchar_at(CURSOR, x, y, CURSOR_ATTR); 
  
}

void hide_cursor() {
  if (cursor.x >=0 
      && cursor.y < SCR_CHAR_HEIGHT 
      && cursor.x >= 0 
      && cursor.x < SCR_CHAR_WIDTH) {
  putchar_at(' ', cursor.x, cursor.y, DEFAULT_ATTR);
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
    putchar_at(*p, x, y, DEFAULT_ATTR);
    x++;
  }
}

void scroll() {
  __asm
    ld b, #SCR_CHAR_HEIGHT
    call 0x0dfe
  __endasm;  
  memset(screen_attr_buf, DEFAULT_ATTR, ATTR_SCREEN_BUFFER_SIZE);
  cursor.x = 0;
}


// Глобальная переменная для хранения состояния последней нажатой клавиши
static char last_key = 0;

char getchar() {
    // Получаем текущий код клавиши
    char current_key = getkey();

    // Если клавиша не нажата (current_key == 0)
    if (current_key == 0) {
        // Сбрасываем состояние последней нажатой клавиши
        last_key = 0;
        return 0; // Ничего не возвращаем
    }

    // Если текущая клавиша уже была нажата ранее (повторное нажатие)
    if (current_key == last_key) {
        return 0; // Игнорируем повторное нажатие
    }

    // Если это новое нажатие
    last_key = current_key; // Обновляем состояние последней нажатой клавиши
    return current_key;     // Возвращаем код клавиши
}

int isspace(int c) {
    // Проверяем, является ли символ одним из пробельных символов
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int putchar(int c) {
  switch (c) {
    case '\n':
      scroll();
      break;
    default:
      putchar_at_cursor(c);
  }
  return c;
}
