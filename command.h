#ifndef __COMMAND_H
#define __COMMAND_H

#include "io.h"

// Тип для функции обработки команд
typedef void (*command_handler)(int argc, char *argv[]);

// Структура для описания команды
typedef struct {
    const char *name;           // Имя команды
    command_handler handler;    // Функция обработки команды
} command_entry;

void process_command(int argc, char *argv[]);

#endif // __COMMAND_H
