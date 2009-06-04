#ifndef KEYBOARD_US_H
#define KEYBOARD_US_H


#define CANT_KEYS 104
#define LANG_US 0
#define LANG_LAT 1
/*scancodes particulares*/
#define ESC 0x01
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define CAPSLOCK 0x3A
#define NUMLOCK 0x45
#define SCROLLLOCK 0x46
#define BACKSPACE 0x0E
#define UPKEY 0x99
#define DOWNKEY 0x98

#define IS_MAKE_CODE(c) (c)<128
#define IS_BREAK_CODE(c) (c)>=128

/*Mascara para saber si el codigo es un break code
 *breakcode=scancode|0x0080*/
#define MASK 0x0080

char ToAsciiUS(char code);

#endif
