#include "keyboard.h"

static t_keyboard_row keyboard_layout[KEYBOARD_NUM_ROWS] = {
  { // 0
    .port = 0xfe, 
    .keys = {0xff, 'z', 'x', 'c', 'v'},
    .caps = {0xff, 'Z', 'X', 'C', 'V'},
    .symb = {0xff, ':', '`', '?', '/'}},
  { // 1
    .port = 0xfd, 
    .keys = {'a', 's', 'd', 'f', 'g'},
    .caps = {'A', 'S', 'D', 'F', 'G'},
    .symb = {'~', '|', '\\', '{', '}'}},
  { // 2
    .port = 0xfb, 
    .keys = {'q', 'w', 'e', 'r', 't'},
    .caps = {'Q', 'W', 'E', 'R', 'T'},
    .symb = {0x0, 0x0, 0x0, '<', '>'}},
  { // 3
    .port = 0xf7, 
    .keys = {'1', '2', '3', '4', '5'},
    .caps = {0x0, 0x0, 0x0, 0x0, K_LT},
    .symb = {'!', '@', '#', '$', '%'}},
  { // 4
    .port = 0xef, 
    .keys = {'0', '9', '8', '7', '6'},
    .caps = {K_DL, 0x0, K_RT, K_UP, K_DN},
    .symb = {'_', ')', '(', '\'', '&'}},
  { // 5
    .port = 0xdf, 
    .keys = {'p', 'o', 'i', 'u', 'y'},
    .caps = {'P', 'O', 'I', 'U', 'Y'},
    .symb = {'\"', ';', 0x0, ']', '['}},
  { // 6
    .port = 0xbf, 
    .keys = {'\n', 'l', 'k', 'j', 'h'},
    .caps = {'\n', 'L', 'K', 'J', 'H'},
    .symb = {'\n', '=', '+', '-', '^'}},
  { // 7y
    .port = 0x7f, 
    .keys = {' ', 0xff, 'm', 'n', 'b'},
    .caps = {' ', 0xff, 'M', 'N', 'B'},
    .symb = {' ', 0xff, '.', '\,', '*'}},
};

char scan(unsigned char port) {
  (void) port;
  __asm
     ld ix, #4
     add ix, sp  ; set ix to arguments  
     ld c, #0xfe
     ld b, 0(ix)
     in a,(c)  
     and #0x1f
     ld l, a
  __endasm;  
}

char getkey() {
    unsigned char scanline; // Переменная для хранения состояния текущей строки клавиатуры
    char key = 0; // Переменная для хранения кода нажатой клавиши (результат)

        // Переменные для отслеживания состояния модификаторов клавиш:
    unsigned char caps_shift = (scan(0xfe) & 0x01) == 0;  // Флаг Caps Shift (например, для переключения регистра)
    unsigned char symbol_shift = (scan(0x7f) & 0x02) == 0; // Флаг Symbol Shift (например, для специальных символов)

    // Цикл по всем строкам клавиатуры
    for (unsigned char r = 0; r < KEYBOARD_NUM_ROWS; r++) {
        // Получаем указатель на текущую строку клавиатуры из массива keyboard_layout
        t_keyboard_row *row = keyboard_layout + r;

        // Сканируем текущую строку клавиатуры через порт (scan возвращает состояние строки)
        scanline = scan(row->port);

        // Цикл по всем клавишам в текущей строке
        for (unsigned char i = 0; i < KEYBOARD_KEYS_PER_ROW; i++) {
            // Проверяем, нажата ли текущая клавиша (бит равен 0, если клавиша нажата)
            if ((scanline & 0x01) == 0) {
                // Если нажата клавиша Caps Shift (первая строка, первая клавиша)
                if (row->keys[i] != 0xff) {
                    // Выбираем символ в зависимости от состояния модификаторов:
                    // - Если активен Symbol Shift, берем символ из массива symb
                    // - Если активен Caps Shift, берем символ из массива caps
                    // - Иначе берем символ из массива keys
                    key = symbol_shift ? row->symb[i] : caps_shift ? row->caps[i] : row->keys[i];
                }
            }
            // Сдвигаем биты вправо, чтобы проверить следующую клавишу в строке
            scanline = scanline >> 1;
        }
    }

    // Возвращаем код нажатой клавиши (или 0, если ничего не нажато)
    return key;
}
