#include "fs.h"
#include <string.h>
#include "io.h"

// Глобальные переменные
node nodes[MAX_NODES + 1]; // Массив узлов

int pwd;

// Инициализация файловой системы
void init_fs() {
    int root;
    int mikhailov_home;
    int project_dir;
    int personal_dir;
    int var;
    int var_log;
    int corrupted_dir;
    int usr;
    int usr_bin;
    int volkov_home;  
  
    memset(nodes, 0, sizeof(nodes)); // Обнуляем массив узлов
// Корневой каталог
    root = make_dir(-1, "/");

    // Домашний каталог Mikhailov
    mikhailov_home = make_dir(root, "/mikhailov");

    // Проект Echo
    project_dir = make_dir(mikhailov_home, "/mikhailov/project");
    touch(project_dir, "/mikhailov/project/overview.txt", "Project \"Echo\"\nObjective: Digital consciousness replication\nStages:\n- Neural recording\n- Modeling\n- Transfer");
    touch(project_dir, "/mikhailov/project/experiment.log", "Experiment #01\nDate: 05/17/1991\nResults:\n- Neural patterns recorded\n- Secondary effects detected\n- System unstable");

    // Личные файлы Mikhailov
    personal_dir = make_dir(mikhailov_home, "/mikhailov/personal");
    touch(personal_dir, "/mikhailov/personal/diary.txt", "May 20, 1991\nSomething strange... The system responds differently now.\n\nJune 10, 1991\nBoundaries between real and digital blur...\n\nJuly 01, 1991\nFinal attempt today...");
    touch(personal_dir, "/mikhailov/personal/notes.sec", "[ENCRYPTED CONTENT]\nKey: XXXX-YYYY-ZZZZ\nHypothesis: Secondary consciousness emerging in system");

    // Системные логи
    var = make_dir(root, "/var");
    var_log = make_dir(var, "/var/log");
    touch(var_log, "/var/log/experiment.log", "[INFO] Experiment started\n[DATA] Neural stream established\n[WARN] Unstable patterns detected\n[ERROR] System response unknown");

    // Поврежденные данные
    corrupted_dir = make_dir(root, "/corrupted");
    touch(corrupted_dir, "/corrupted/data.corrupted", "ERROR: FILE CORRUPTED\nData fragments:\nNeural match: 78%\nSecondary entity?...");

    // Скрипты
    usr = make_dir(root, "/usr");
    usr_bin = make_dir(usr, "/usr/bin");
    touch(usr_bin, "check_system.sh", "#!/bin/sh\necho \"System stability check\"\nif [ $(random 1 100) -lt 80 ]; then\necho \"Warning: Instability detected\"\nfi");

    // Файлы Volkov
    volkov_home = make_dir(root, "/volkov");
    touch(volkov_home, "/volkov/notes.txt", "Concerns about experiment data\nUnusual activity logs\nNeed to discuss with Dr. Mikhailov");
  
    pwd = root;
}

int get_pwd() {
    return pwd;
}

// Функция для поиска доступного узла
int find_empty_node() {
  for (int i = 0; i < MAX_NODES; i++) {
    if (nodes[i].name == NULL) return i;
  }
  return -1;
}

// Функция для создания нового узла
int create_node(const char *name, node_type type, int parent_index) {
    int index = find_empty_node();
    if (index == -1) {
        printf("Disk full!");
        return -1; // Нет свободных узлов
    }
    nodes[index].name = name;
    nodes[index].type = type;
    nodes[index].parent_index = parent_index;
    nodes[index].first_child = -1;  // Нет дочерних узлов
    nodes[index].next_sibling = -1; // Нет соседей
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
int make_dir(int current_dir_index, const char *name) {
    int new_dir_index = create_node(name, NODE_DIR, current_dir_index);
    if (new_dir_index != -1) {
        add_node_to_dir(current_dir_index, new_dir_index);
    }
    return new_dir_index;
}

// Функция для создания файла
int touch(int current_dir_index, const char *name, const char *content) {
    int new_file_index = create_node(name, NODE_FILE, current_dir_index);
    if (new_file_index != -1) {
        nodes[new_file_index].content = content;
        add_node_to_dir(current_dir_index, new_file_index);
    }
    return new_file_index;
}

// Функция для вывода содержимого директории
void list_dir(int dir_index) {
    int current;
  
    if (nodes[dir_index].type != NODE_DIR) {
      printf("%s is not a directory.", nodes[dir_index].name);
        return; // Без вывода сообщений
    }
  
    current = nodes[dir_index].first_child;

    while (current != -1) {
        if (nodes[current].type == NODE_DIR) {
            printf("%s\n", nodes[current].name);
        } else {
            printf("%s\n", nodes[current].name);
        }
        current = nodes[current].next_sibling;
    }
}

void cat_file(int file_index){
    if (nodes[file_index].type != NODE_FILE) {
      printf("%s is not a file.", nodes[file_index].name);
        return; // Без вывода сообщений
    }
  printf("%s", nodes[file_index].content);
}

// Функция для поиска узла по имени в текущей директории
int find_node(const char *name) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (strcmp(nodes[i].name, name) == 0) return i;
    }
    return -1;
}

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
