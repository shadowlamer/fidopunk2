#include "command.h"
#include "fs.h"

#define MAX_ARGS 32

// Список поддерживаемых команд
command_entry supported_commands[] = {
    {"echo", "[args] - Print arguments", handle_echo},
    {"help", "       - Show help",       handle_help},
    {"ls", "  [dir]  - List dir",        handle_ls},
    {NULL} // Окончание списка
};

// Пример функций обработки команд
void handle_echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf(argv[i]);
    }
    printf("\n");
}

void handle_help(int argc, char *argv[]) {
    (void) argc, argv;
    printf("Supported commands:\n");
    for (command_entry *p = supported_commands; p->name != NULL; p++) {
        if (p->description != NULL) {
            printf("  %s %s\n", p->name, p->description);
        }
    }
}

void handle_ls(int argc, char *argv[]) {
    (void) argc, argv;
    list_dir(0);
}

void handle_unknown(int argc, char *argv[]) {
    (void) argc;
    printf("Unknown command: '%s'\n", argv[0]);
    printf("Type 'help' for a list of supported commands.\n");
}


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
