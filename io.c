#include <stdarg.h>
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
  set_cursor(0, SCR_CHAR_HEIGHT - 1);
}


// Упрощенная версия printf
void printf(const char *format, ...) {
    va_list args; // Список аргументов переменной длины
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++; // Переходим к следующему символу после '%'
            switch (*format) {
                case 'c': { // Вывод символа
                    char c = (char)va_arg(args, int); // Получаем символ
                    putchar(c);
                    break;
                }
                case 's': { // Вывод строки
                    const char *str = va_arg(args, const char*); // Получаем строку
                    puts(str);
                    break;
                }
                case 'd': { // Вывод целого числа
                    int num = va_arg(args, int); // Получаем число
                    print_int(num);
                    break;
                }
                case '%': { // Вывод символа '%'
                    putchar('%');
                    break;
                }
                default: { // Неизвестный спецификатор
                    putchar('%');
                    putchar(*format);
                    break;
                }
            }
        } else {
            // Обычный символ, просто выводим его
            putchar(*format);
        }
        format++;
    }

    va_end(args); // Завершаем работу со списком аргументов
}
void scroll() {
  __asm
    ld b, #SCR_CHAR_HEIGHT
    call 0x0dfe
  __endasm;  
  memset(screen_attr_buf, DEFAULT_ATTR, ATTR_SCREEN_BUFFER_SIZE);
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
      new_line();
      break;
    default:
      putchar_at_cursor(c);
  }
  return c;
}

// Функция для вывода строки с помощью putchar
void puts(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

void print_int(int n) {
    // Буфер для хранения цифр числа
    char buffer[20]; // Максимальная длина int (включая знак) — 11 символов
    int index = 0;
    if (n == 0) {
        putchar('0'); // Особый случай: число 0
        return;
    }
    // Обработка отрицательных чисел
    if (n < 0) {
        putchar('-');
        n = -n; // Преобразуем число в положительное
    }
    // Заполняем буфер цифрами числа в обратном порядке
    while (n > 0) {
        buffer[index++] = '0' + (n % 10); // Сохраняем последнюю цифру
        n /= 10; // Убираем последнюю цифру
    }
    // Выводим цифры в правильном порядке
    while (index > 0) {
        putchar(buffer[--index]);
    }
}

void new_line() {
  hide_cursor();
  cursor.x = 0;
  scroll();
}
