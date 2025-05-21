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
//#link "initfs.c"
#include "snake.h"
//#link "snake.c"


#define MAX_CMD_LEN 64
#define MAX_ARGS 16
#define MAX_HISTORY_SIZE 8

#define PROMPT "$ "

static char history[MAX_HISTORY_SIZE][MAX_CMD_LEN]; // Буфер истории
static int history_index = -1;                      // Индекс текущей команды в истории
static int history_count = 0;                       // Количество сохранённых команд
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
  set_cursor(0, SCR_CHAR_HEIGHT - 1);
  
  printf("Press any key...");
  while (!getchar());
  cls();
  printf(cat_file("/etc/motd"));
  
  for (;;) {
    new_line();
    argc = parse_cmd();
    new_line();
    process_command(argc, argv);
  }
  return 0;
}

void add_to_history(const char *cmd) {
    if (strlen(cmd) == 0) return;

    strncpy(history[(history_count++) % MAX_HISTORY_SIZE], cmd, MAX_CMD_LEN - 1);
    history_count = (history_count > MAX_HISTORY_SIZE) ? MAX_HISTORY_SIZE : history_count;
}

int read_cmd() {
    int len = 0;
    char key;
    int current_hist_index = -1; // Текущий индекс истории (-1 = нет)
    
    prompt();

    for (;;) {
        key = getchar();

        switch (key) {
            case '\n':  // Enter
                cmd_buf[len] = '\0';
                new_line();
                add_to_history(cmd_buf); // Добавляем в историю
                return len + 1;

            case K_DL:  // Удаление символа слева (Backspace)
                if (len > 0) {
                    cursor_left();
                    len--;
                }
                break;

            case K_UP:  // Стрелка ВВЕРХ — предыдущая команда
                if (current_hist_index < history_count - 1) {
                    current_hist_index++;
                    set_cursor(0, SCR_CHAR_HEIGHT - 1);
                    printf("%s%s", get_name(get_pwd()), PROMPT);

                    // Очистка строки
                    for (int i = 0; i < len; i++) putchar_at_cursor(' ');
                    set_cursor(0, SCR_CHAR_HEIGHT - 1);  // Замена cursor_goto на set_cursor

                    printf("%s%s", get_name(get_pwd()), PROMPT);

                    strcpy(cmd_buf, history[(history_count - 1 - current_hist_index + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE]);
                    len = strlen(cmd_buf);
                    printf("%s", cmd_buf);
                }
                break;

            case K_DN:  // Стрелка ВНИЗ — следующая команда
                if (current_hist_index >= 0) {
                    current_hist_index--;

                    set_cursor(0, SCR_CHAR_HEIGHT - 1);  // Замена cursor_goto
                    printf("%s%s", get_name(get_pwd()), PROMPT);

                    // Очистка строки
                    for (int i = 0; i < len; i++) putchar_at_cursor(' ');
                    set_cursor(0, SCR_CHAR_HEIGHT - 1);  // Замена cursor_goto

                    printf("%s%s", get_name(get_pwd()), PROMPT);

                    if (current_hist_index >= 0) {
                        strcpy(cmd_buf, history[(history_count - 1 - current_hist_index + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE]);
                        len = strlen(cmd_buf);
                    } else {
                        cmd_buf[0] = '\0';
                        len = 0;
                    }
                    printf("%s", cmd_buf);
                }
                break;

            default:
                if (key >= 0x20 && key < 0x80 && len < MAX_CMD_LEN) {
                    putchar_at_cursor(key);
                    cmd_buf[len] = key;
                    len++;
                }
                break;
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
  printf("%s%s", get_name(get_pwd()), PROMPT);
}
