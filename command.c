#include "command.h"
#include "fs.h"
#include "snake.h"

#define MAX_ARGS 32

// Список поддерживаемых команд
t_command_entry supported_commands[] = {
    {"help", "       - Show help",         handle_help},
    {"cat", "<file>  - Show file content", handle_cat},
    {"cd", "[dir]    - Change dir",        handle_cd},
    {"echo", "[args] - Print arguments",   handle_echo},
    {"decrypt", "<file> <passwd>\n"
    "  - Decrypt file with password",      handle_decrypt},
    {"ls", "[dir]    - List dir",          handle_ls},
    {"pwd", "        - Show current dir",  handle_pwd},
    {"restore", NULL,  handle_restore},
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

void handle_decrypt(int argc, char *argv[], void *ce) {
  char *content;
  switch (argc) {
    case 3:
      content = cat_file(argv[1]);
      if (content == NULL) {
            printf("No such file: %s", argv[1]);
      } else {
            printf(encrypt(content, argv[2]));
      }
      break;
    default:
      print_usage((t_command_entry *) ce);
  }
}

char *is_broken(char *name) {
    return strstr(name, ".dump");
}

void handle_restore(int argc, char *argv[], void *ce) {
  char *content;
  char *name;
  (void) ce;
  switch (argc) {
    case 2:
      name = argv[1];
      if (is_broken(name)) {
          content = cat_file(name);
          if (content == NULL) {
                printf("No such file: %s", name);
          } else {
                if (snake_run(content)) {
                     printf("\n\nRestore over!!!\n");
                } else {
                     printf("\n\nFile restored: %s\n", name);
                     strcpy(is_broken(name), ".restored");
                }
          }
      } else {
        printf("File is not broken: %s", name);
      }
      break;
  }
}

void handle_pwd(int argc, char *argv[], void *ce) {
    (void) argc, argv, ce;
    printf("%s\n", get_name(get_pwd()));
}

void handle_cd(int argc, char *argv[], void *ce) {
  switch (argc) {
    case 2:
      change_dir(argv[1]);
      break;
    default:
      print_usage((t_command_entry *) ce);
  }
}

void handle_ls(int argc, char *argv[], void *ce) {
  int show_hidden = 0;
  char *path = NULL;
  for (int i = 1; i <  argc; i++) {
    if (strcmp("-la", argv[i]) == 0) {
      show_hidden = 1;
    } else if (path == NULL) {
      path = argv[i];
    } else {
      print_usage((t_command_entry *) ce);
      return;    
    }
  }
  if (path == NULL) path = get_name(get_pwd());
  list_dir(path, show_hidden);  
}

void handle_cat(int argc, char *argv[], void *ce) {
  char *content;
  char *name;
  switch (argc) {
    case 2:
      name = argv[1];
      if (is_broken(name)) {
          printf("File is broken: %s", name);
      } else {
          content = cat_file(name);
          if (content == NULL) {
              printf("No such file: %s", argv[1]);
          } else {
              printf(content);
          }
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
        printf("%s %s\n", ce->name, ce->description);
    }
}
