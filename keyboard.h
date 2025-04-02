#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define KEYBOARD_NUM_ROWS 8
#define KEYBOARD_KEYS_PER_ROW 5

// Специальные символы
#define K_TB 0x06 // Tab
#define K_DL 0x07 // Delete
#define K_LT 0x08 // Left Arrow
#define K_RT 0x09 // Right Arrow
#define K_UP 0x0B // Up Arrow
#define K_DN 0x0C // Down Arrow
#define K_MOD 0xFF // Скан-код модификатора

typedef struct {
  unsigned char port;
  char keys[KEYBOARD_KEYS_PER_ROW];
  char caps[KEYBOARD_KEYS_PER_ROW];
  char symb[KEYBOARD_KEYS_PER_ROW];
} t_keyboard_row;

char getkey();
char scan(unsigned char port);

#endif // __KEYBOARD_H
