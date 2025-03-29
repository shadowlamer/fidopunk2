#include "fs.h"
#include <string.h>
#include "io.h"

// Глобальные переменные
node nodes[MAX_NODES + 1]; // Массив узлов
int pwd;
char show_hidden = 0;
char name_buf[NAME_BUF_SIZE + 1];

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
void list_dir(const char *name) {
    int current;

    int dir_index = find_node(name) ;
  
    if (nodes[dir_index].type != NODE_DIR) {
      printf("%s is not a directory.", nodes[dir_index].name);
        return; // Без вывода сообщений
    }
  
    current = nodes[dir_index].first_child;

    while (current != -1) {
        if (strstr(nodes[current].name, "/.") == NULL || show_hidden) {
            if (nodes[current].type == NODE_DIR) {
                printf("%s\n", nodes[current].name);
            } else {
                printf("%s\n", nodes[current].name);
            }
        }
        current = nodes[current].next_sibling;
    }
}

char *compose_path(const char *name) {
    if (name[0] == '/') { // абсолютный путь
        strcpy(name_buf, name);
    } else {
        strcpy(name_buf, nodes[pwd].name);
        if (name_buf[strlen(name_buf) - 1] != '/') strcat(name_buf, "/");
        strcat(name_buf, name);
    }
    return name_buf;
}

const char *cat_file(const char *name){
  int file_index = find_node(name);
    if (file_index == -1 || nodes[file_index].type != NODE_FILE) {
        return NULL; // Без вывода сообщений
    }
  return nodes[file_index].content;
}

// Функция для поиска узла по имени в текущей директории
int find_node(const char *name) {
    char *path = compose_path(name);
    for (int i = 0; i < MAX_NODES; i++) {
        if (strcmp(nodes[i].name, path) == 0) return i;
    }
    return -1;
}

// Функция для перехода в другую директорию
int change_dir(const char *name) {
    int target;

    if (strcmp(name, ".") == 0) {
        return pwd;
    }
 
    if (strcmp(name, "..") == 0) {
        if (nodes[pwd].parent_index != -1) {
          pwd = nodes[pwd].parent_index;
        }
        return pwd;
    }

    target = find_node(name);   
    if (target != -1 && nodes[target].type == NODE_DIR) {
        pwd = target;
        return pwd;
    }
  
    printf("No such dir: %s\n", name_buf);
    return pwd;
}

const char *get_name(int index) {
    return nodes[index].name;
}

void set_show_hidden(char val) {
    show_hidden = val;
}

char *encrypt(char *text, char *passwd) {
    memset(name_buf, 0x00, NAME_BUF_SIZE);
    for (char i = 0; name_buf[i] != 0xff && i < NAME_BUF_SIZE; i++) {
         name_buf[i] = text[i] ^ passwd[i % strlen(passwd)];
    }
    return name_buf;
}
