#ifndef __COMMAND_H
#define __COMMAND_H

#include "io.h"

// Тип для функции обработки команд
typedef void (*command_handler)(int argc, char *argv[], void *ce);

// Структура для описания команды
typedef struct {
    const char *name;           // Имя команды
    const char *description;    // Описание команды
    command_handler handler;    // Функция обработки команды
} t_command_entry;

void print_usage(t_command_entry *ce);
void process_command(int argc, char *argv[]);

#endif // __COMMAND_H
