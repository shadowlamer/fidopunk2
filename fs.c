#include "fs.h"
#include <string.h>
#include "io.h"

// Глобальные переменные
node nodes[MAX_NODES]; // Массив узлов
int free_node_index = 0; // Индекс первого свободного узла

// Инициализация файловой системы
void init_fs() {
    memset(nodes, 0, sizeof(nodes)); // Обнуляем массив узлов
    free_node_index = 0;            // Начинаем с первого узла
    // Создаем корневую директорию
    create_node("root", NODE_DIR, -1);
    make_dir(0, "tmp");
}

// Функция для создания нового узла
int create_node(const char *name, node_type type, int parent_index) {
    int index = free_node_index;
    if (free_node_index >= MAX_NODES) {
        return -1; // Нет свободных узлов
    }
    nodes[index].name = name;
    nodes[index].type = type;
    nodes[index].parent_index = parent_index;
    nodes[index].first_child = -1;  // Нет дочерних узлов
    nodes[index].next_sibling = -1; // Нет соседей
    free_node_index++;
    return index;
}

// Функция для добавления узла в директорию
void add_node_to_dir(int dir_index, int new_node_index) {
    if (nodes[dir_index].type != NODE_DIR) {
        return; // Без вывода сообщений
    }

    // Добавляем новый узел в конец списка дочерних узлов
    if (nodes[dir_index].first_child == -1) {
        nodes[dir_index].first_child = new_node_index;
    } else {
        int current = nodes[dir_index].first_child;
        while (nodes[current].next_sibling != -1) {
            current = nodes[current].next_sibling;
        }
        nodes[current].next_sibling = new_node_index;
    }
}

// Функция для создания директории
void make_dir(int current_dir_index, const char *name) {
    int new_dir_index = create_node(name, NODE_DIR, current_dir_index);
    if (new_dir_index != -1) {
        add_node_to_dir(current_dir_index, new_dir_index);
    }
}

// Функция для создания файла
//void touch(int current_dir_index, const char *name, const char *content) {
//    int new_file_index = create_node(name, NODE_FILE, current_dir_index);
//    if (new_file_index != -1) {
//        nodes[new_file_index].content = content;
//        add_node_to_dir(current_dir_index, new_file_index);
//    }
//}

// Функция для вывода содержимого директории
void list_dir(int dir_index) {
    int current;
  
    if (nodes[dir_index].type != NODE_DIR) {
        return; // Без вывода сообщений
    }
  
    printf("/%s\n", (char*)nodes[dir_index].name);

    current = nodes[dir_index].first_child;

    while (current != -1) {
        if (nodes[current].type == NODE_DIR) {
            printf("+-/%s\n", nodes[current].name);
        } else {
            printf("[FILE] %s (%s)\n", nodes[current].name, nodes[current].content ? nodes[current].content : "<empty>");
        }
        current = nodes[current].next_sibling;
    }
}

// Функция для поиска узла по имени в текущей директории
//int find_node_in_dir(int dir_index, const char *name) {
//    int current = nodes[dir_index].first_child;
//
//    if (nodes[dir_index].type != NODE_DIR) {
//        return -1; // Без вывода сообщений
//    }
//
//    while (current != -1) {
//        if (strcmp(nodes[current].name, name) == 0) {
//            return current;
//        }
//        current = nodes[current].next_sibling;
//    }
//    return -1;
//}

// Функция для перехода в другую директорию
//int change_dir(int current_dir_index, const char *name) {
//    int target = find_node_in_dir(current_dir_index, name);
//
//    if (strcmp(name, "..") == 0) {
//        return nodes[current_dir_index].parent_index != -1 ? nodes[current_dir_index].parent_index : current_dir_index;
//    }
//
//    if (target == -1 || nodes[target].type != NODE_DIR) {
//        return current_dir_index; // Без вывода сообщений
//    }
//    return target;
//}
