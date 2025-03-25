#ifndef __FS_H
#define __FS_H

#include <stddef.h>

// Максимальное количество узлов в файловой системе
#define MAX_NODES 10

// Типы узлов: файл или директория
typedef enum {
    NODE_FILE,
    NODE_DIR
} node_type;

// Структура узла файловой системы
typedef struct node {
    const char *name;       // Указатель на имя файла или директории
    node_type type;         // Тип узла (файл или директория)
    int parent_index;       // Индекс родительской директории (-1 для корня)
    union {
        const char *content; // Указатель на содержимое файла
        int first_child;     // Индекс первого дочернего узла (для директории)
    };
    int next_sibling;       // Индекс следующего узла на том же уровне
} node;

// Глобальные переменные
extern node nodes[MAX_NODES]; // Массив узлов
extern int free_node_index;   // Индекс первого свободного узла

// Функции для работы с файловой системой
void init_fs();
int create_node(const char *name, node_type type, int parent_index);
void add_node_to_dir(int dir_index, int new_node_index);
int make_dir(int current_dir_index, const char *name);
int touch(int current_dir_index, const char *name, const char *content);
void list_dir(int dir_index);
void cat_file(int file_index);
int find_node(const char *name);
int find_empty_node();
int get_pwd();
int change_dir(int current_dir_index, const char *name);

#endif // __FS_H
