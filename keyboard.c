#include "keyboard.h"

// Раскладка клавиатуры
static t_keyboard_row keyboard_layout[KEYBOARD_NUM_ROWS] = {
    [0] = { // Row 0
        .port = 0xFE, 
        .keys = {K_MOD, 'z', 'x', 'c', 'v'},
        .caps = {K_MOD, 'Z', 'X', 'C', 'V'},
        .symb = {K_MOD, ':', '`', '?', '/'}},
    
    [1] = { // Row 1
        .port = 0xFD, 
        .keys = {'a', 's', 'd', 'f', 'g'},
        .caps = {'A', 'S', 'D', 'F', 'G'},
        .symb = {'~', '|', '\\', '{', '}'}},
    
    [2] = { // Row 2
        .port = 0xFB, 
        .keys = {'q', 'w', 'e', 'r', 't'},
        .caps = {'Q', 'W', 'E', 'R', 'T'},
        .symb = {'\0', '\0', '\0', '<', '>'}},
    
    [3] = { // Row 3
        .port = 0xF7, 
        .keys = {'1', '2', '3', '4', '5'},
        .caps = {'\0', '\0', '\0', '\0', K_LT},
        .symb = {'!', '@', '#', '$', '%'}},
    
    [4] = { // Row 4
        .port = 0xEF, 
        .keys = {'0', '9', '8', '7', '6'},
        .caps = {K_DL, '\0', K_RT, K_UP, K_DN},
        .symb = {'_', ')', '(', '\'', '&'}},
    
    [5] = { // Row 5
        .port = 0xDF, 
        .keys = {'p', 'o', 'i', 'u', 'y'},
        .caps = {'P', 'O', 'I', 'U', 'Y'},
        .symb = {'"', ';', '\0', ']', '['}},
    
    [6] = { // Row 6
        .port = 0xBF, 
        .keys = {'\n', 'l', 'k', 'j', 'h'},
        .caps = {'\n', 'L', 'K', 'J', 'H'},
        .symb = {'\n', '=', '+', '-', '^'}},
    
    [7] = { // Row 7
        .port = 0x7F, 
        .keys = {' ', K_MOD, 'm', 'n', 'b'},
        .caps = {' ', K_MOD, 'M', 'N', 'B'},
        .symb = {' ', K_MOD, '.', ',', '*'}}
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
    unsigned char caps_shift = (scan(0xfe) & 0x01) == 0;  // Флаг Caps Shift
    unsigned char symbol_shift = (scan(0x7f) & 0x02) == 0; // Флаг Symbol Shift

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
