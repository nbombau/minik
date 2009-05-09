#include"../include/keyboardUS.h"
#include"../include/buffer.h"
#include"../include/defs.h"

#define IsLetter(c) ( ( (c)>='a' && (c)<='z' ) || ((c)>='A' && (c)<='Z') )?1:0
#define IS_VALID_ASCII(c) (c)!=0
#define ES_VOCAL_MIN(c) ( (c)=='a'||(c)=='e'||(c)=='i'||(c)=='o'||(c)=='u' )


extern int numlock,caps,scrolllock,shift,altgr;

static char tecladoUS [][CANT_KEYS+1] =
{
	{0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p',
        '[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.',
         '/',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	/*Shift*/
        {0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R','T','Y','U','I','O','P','{',
        '}','\n',0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B','N','M','<','>','?',0,
        0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        /*AltGr*/
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        }
};


static char
ToUpper(char c)
{
	return ( (c)>='a' && (c)<='z' )?(c - 'a' + 'A'):(c);
}

static char
ToLower(char c)
{
	return ( (c)>='A' && (c)<='Z' )?(c - 'A' + 'a'):(c);
}

char
ToAsciiUS(char code)
{
        char ascii;
        ascii=tecladoUS[shift][code];
        if( caps && !shift && IsLetter(ascii) )
            ascii=ToUpper(ascii);
        else if( caps && shift && IsLetter(ascii) )
            ascii=ToLower(ascii);
        return ascii;
}



