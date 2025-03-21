#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define KEYBOARD_NUM_ROWS 8
#define KEYBOARD_KEYS_PER_ROW 5

typedef struct {
  unsigned char port;
  char keys[KEYBOARD_KEYS_PER_ROW];
  char caps[KEYBOARD_KEYS_PER_ROW];
  char symb[KEYBOARD_KEYS_PER_ROW];
} t_keyboard_row;

char getkey();
char scan(unsigned char port);

#endif // __KEYBOARD_H
