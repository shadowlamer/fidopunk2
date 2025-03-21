#include "command.h"

#include <stdio.h>

#define MAX_ARGS 32

// Пример функций обработки команд
void handle_echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printz(argv[i], 0, 0);
    }
    printz("\n", 0, 0);
}

void handle_help(int argc, char *argv[]) {
    (void) argc, argv;
    printf("Supported commands:\n");
    printf("  echo [args] - Print arguments\n");
    printf("  help        - Show this help message\n");
}

void handle_unknown(int argc, char *argv[]) {
    (void) argc;
    printf("Unknown command: '%s'\n", argv[0]);
    printf("Type 'help' for a list of supported commands.\n");
}

// Список поддерживаемых команд
command_entry supported_commands[] = {
    {"echo", handle_echo},
    {"help", handle_help},
    {NULL, NULL} // Окончание списка
};

// Функция для обработки команд
void process_command(int argc, char *argv[]) {
    if (argc == 0 || argv[0] == NULL) {
        printf("No command provided.\n");
        return;
    }

    // Поиск команды в списке
    for (int i = 0; supported_commands[i].name != NULL; i++) {
        if (strcmp(argv[0], supported_commands[i].name) == 0) {
            // Команда найдена, вызываем соответствующую функцию
            supported_commands[i].handler(argc, argv);
            return;
        }
    }

    // Если команда не найдена, вызываем обработчик неизвестной команды
    handle_unknown(argc, argv);
}
