#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define KEYBOARD_NUM_ROWS 8
#define KEYBOARD_KEYS_PER_ROW 5

#define K_TB 0x06
#define K_DL 0x07
#define K_LT 0x08
#define K_RT 0x09
#define K_UP 0x0B
#define K_DN 0x0C

typedef struct {
  unsigned char port;
  char keys[KEYBOARD_KEYS_PER_ROW];
  char caps[KEYBOARD_KEYS_PER_ROW];
  char symb[KEYBOARD_KEYS_PER_ROW];
} t_keyboard_row;

char getkey();
char scan(unsigned char port);

#endif // __KEYBOARD_H
