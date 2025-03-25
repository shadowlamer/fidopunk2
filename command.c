#include "command.h"
#include "fs.h"

#define MAX_ARGS 32

// Список поддерживаемых команд
t_command_entry supported_commands[] = {
    {"echo", "[args] - Print arguments",   handle_echo},
    {"help", "       - Show help",         handle_help},
    {"cat", " [file] - Show file content", handle_cat},
    {"ls", "  [dir]  - List dir",          handle_ls},
    {NULL} // Окончание списка
};

// Пример функций обработки команд
void handle_echo(int argc, char *argv[], void *ce) {
  (void) ce;
    for (int i = 1; i < argc; i++) {
        printf(argv[i]);
    }
    printf("\n");
}

void handle_help(int argc, char *argv[], void *ce) {
    (void) argc, argv, ce;
    printf("Supported commands:\n");
    for (t_command_entry *p = supported_commands; p->name != NULL; p++) print_usage(p);
}

void handle_ls(int argc, char *argv[], void *ce) {
  int dir;
  switch (argc) {
    case 1:
      dir = get_pwd();
      list_dir(dir);
      break;
    case 2:
      dir = find_node(argv[1]);
      if (dir == -1) {
        printf("No such directory.");
      } else {
        list_dir(dir);
      }
      break;
    default:
      print_usage((t_command_entry *) ce);
  }
}

void handle_cat(int argc, char *argv[], void *ce) {
  int file;
  switch (argc) {
    case 2:
      file = find_node(argv[1]);
      if (file == -1) {
        printf("No such file.");
      } else {
        cat_file(file);
      }
      break;
    default:
      print_usage((t_command_entry *) ce);
  }
}

void handle_unknown(int argc, char *argv[], void *ce) {
    (void) argc, ce;
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
    for (t_command_entry *p = supported_commands; p->name != NULL; p++) {
        if (strcmp(argv[0], p->name) == 0) {
            // Команда найдена, вызываем соответствующую функцию
            p->handler(argc, argv, p);
            return;
        }
    }

    // Если команда не найдена, вызываем обработчик неизвестной команды
    handle_unknown(argc, argv, NULL);
}

void print_usage(t_command_entry *ce) {
    if (ce->description != NULL) {
        printf(" %s %s\n", ce->name, ce->description);
    }
}
