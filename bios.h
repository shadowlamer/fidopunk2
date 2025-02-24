
// FONT FUNCTIONS
#define LOCHAR 0x20	// lowest character value in font
#define HICHAR 0x7f	// highest character value in font
unsigned char __at(0x3d00) font8x8[HICHAR-LOCHAR+1][8];
  
void init_stdio(void);
int putchar(int ch);
char getchar();
int keyscan(void);
void waitkey(int frames);
void scroll();

void setpixel(unsigned char x, unsigned char y);
void beep(int divisor, int duration);

