
#include "bios.h"

void init_stdio(void) {
  __asm
    call 0x0daf
    ld a,#1
    call 0x1601
  __endasm;
}

int putchar(int ch) {
  if (ch == 10) ch = 13; // newline -> CR
  __asm
    ld	a,4 (ix)
    rst 0x10
  __endasm;
  return ch;
}

void beep(int divisor, int duration) {
  divisor;
  duration;
  __asm
    ld	l,4 (ix)
    ld	h,5 (ix)
    ld	e,6 (ix)
    ld	d,7 (ix)
    call 0x3b5
  __endasm;
}

int keyscan(void) {
  __asm
    call 0x028e
    ld  l,e
    ld  h,d
  __endasm;
}

void waitkey(int frames) {
  frames;
  __asm
    ld	c,4 (ix)
    ld	b,5 (ix)
    call 0x1f3d
  __endasm;
}

void setpixel(unsigned char x, unsigned char y) {
  x;
  y;
  __asm
    ld	c,4 (ix)
    ld	b,5 (ix)
    call 0x22e5
  __endasm;
}

char getchar() {
  __asm
getc_wait_key: 
  ei
  halt
  call 0x02BF  
  ld a, (0x5C3B)
  and a, #0b00100000
  jr z, getc_wait_key  
  ld a, (0x5C08)
  ld l, a  
  ld a, (0x5C3B)
  and a, #0b11011111
  ld (0x5C3B), a  
  __endasm;  
}

void scroll() {
  __asm
    ld b, #32
    call 0x0dfe
  __endasm;  
}

