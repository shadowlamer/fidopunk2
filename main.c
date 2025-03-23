#include <string.h>
#include "scr_addr.h"
#include "keyboard.h"
//#link "keyboard.c"
#include "io.h"
//#link "io.c"
#include "command.h"
//#link "command.c"
#include "fs.h"
//#link "fs.c"
#include "messages.h"


#define MAX_CMD_LEN 128
#define MAX_ARGS 16

#define PROMPT "$ "

static char cmd_buf[MAX_CMD_LEN];
static char *argv[MAX_ARGS + 1]; // Статический массив для argv (+1 для NULL)
static unsigned char argc; 


void prompt();
int read_cmd();
int parse_cmd();

int main() {
  int argc;
#include "filler.h"
  
  init_fs();
 
  cls();
  printf(msg_moto);
  
  for (;;) {
    new_line();
    argc = parse_cmd();
    new_line();
    process_command(argc, argv);
  }
  return 0;
}


int read_cmd() {

  int len = 0;
  char key;
  
  prompt();
  for (;;) {
    key = getchar();
    switch (key) {
      case '\n':  
        cmd_buf[len] = 0x00;
        new_line();
        return len + 1;
      case K_DL:
        if (len <= 0) break;
        cursor_left();
        len--;
        break;
      default:  
        if (key >= 0x20 && key < 0x80 && len < MAX_CMD_LEN) {
          putchar_at_cursor(key);
          cmd_buf[len] = key;
          len++;
        }
    }
  } 
}

int parse_cmd() {
    char *buf = cmd_buf;              // Указатель на начало буфера
    int arg_count = 0;                // Счетчик аргументов
    unsigned char in_quotes = 0;      // Флаг для обработки кавычек
    int len = read_cmd();             // Читаем команду и получаем её длину
  
    if (len == 0) {
        return 0;                     // Если команда пустая, возвращаем argc = 0
    }

    // Проходим по каждому символу в буфере
    for (int i = 0; i < len && arg_count < MAX_ARGS; i++) {
        char c = buf[i];

        // Пропускаем пробелы, если мы не внутри кавычек
        if (!in_quotes && isspace(c)) {
            buf[i] = '\0'; // Заменяем пробел на нулевой символ (конец строки)
            continue;
        }

        // Начинаем новый аргумент
        if (buf[i] != '\0' && (i == 0 || buf[i - 1] == '\0')) {
            if (arg_count >= MAX_ARGS) {
                break; // Достигнут лимит аргументов
            }
            argv[arg_count++] = &buf[i]; // Сохраняем указатель на начало аргумента
        }

        // Обработка кавычек
        if (c == '"') {
            in_quotes = !in_quotes; // Переключаем флаг кавычек
            buf[i] = '\0';          // Удаляем кавычку из строки
        }
    }

    // Устанавливаем последний аргумент как NULL (требование для argv)
    argv[arg_count] = NULL;

    // Возвращаем количество аргументов
    return arg_count;
}

void prompt() {
  set_cursor(0, SCR_CHAR_HEIGHT - 1);
  printf(PROMPT);
}
