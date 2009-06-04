#include"../include/buffer.h"
#include"../include/defs.h"
#include"../include/keyboardLAT.h"

/*Hex ¡*/
#define EXCLAM 0xAD
/*Hex ¿*/
#define INTER 0xA8
/*Hex circulito*/
#define CIRC 0xF8
/*Hex de la enie minuscula*/
#define ENIE_MINUS 0xA4
/*Hex de la enie mayuscula*/
#define ENIE_MAYUS 0xA5


/*Hex a con tilde*/
#define A_MIN 0xA0
/*Hex e con tilde*/
#define E_MIN 0x82
/*Hex i con tilde*/
#define I_MIN 0xA1
/*Hex o con tilde*/
#define O_MIN 0xA2
/*Hex u con tilde*/
#define U_MIN 0xA3

/*Hex a con tilde inverso*/
#define A_MIN2 0x85
/*Hex e con tilde inverso*/
#define E_MIN2 0x8A
/*Hex i con tilde inverso*/
#define I_MIN2 0x8D
/*Hex o con tilde inverso*/
#define O_MIN2 0x95
/*Hex u con tilde inverso*/
#define U_MIN2 0x97

/*Hex a con dierecis*/
#define A_MIN3 0x84
/*Hex e con dierecis*/
#define E_MIN3 0x89
/*Hex i con dierecis*/
#define I_MIN3 0x8B
/*Hex o con dierecis*/
#define O_MIN3 0x94
/*Hex u con dierecis*/
#define U_MIN3 0x81

/*Hex a con tilde sombrero*/
#define A_MIN4 0x83
/*Hex e con tilde sombrero*/
#define E_MIN4 0x88
/*Hex i con tilde sombrero*/
#define I_MIN4 0x8C
/*Hex o con tilde sombrero*/
#define O_MIN4 0x93
/*Hex u con tilde sombrero*/
#define U_MIN4 0x96


#define MAX_ACCENT 5

#define IsLetter(c) ( ( (c)>='a' && (c)<='z' ) || ((c)>='A' && (c)<='Z') )?1:0

#define ES_VOCAL_MIN(c) ( (c)=='a'||(c)=='e'||(c)=='i'||(c)=='o'||(c)=='u' )


extern int numlock,caps,scrolllock,shift,altgr;

static int accent=0;

static char tecladoLAT [][CANT_KEYS+1] =
{
	{0,0,'1','2','3','4','5','6','7','8','9','0','\'',INTER,'\b','\t','q','w','e','r','t','y','u','i','o','p',
        0,'+','\n',0,'a','s','d','f','g','h','j','k','l',ENIE_MINUS,'{','|',0,'}','z','x','c','v','b','n','m',',','.',
         '-',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	/*Shift*/
        {0,0,'!','"','#','$','%','&','/','(',')','=','?',EXCLAM,'\b','\t','Q','W','E','R','T','Y','U','I','O','P',0,
        '*','\n',0,'A','S','D','F','G','H','J','K','L',ENIE_MAYUS,'[',CIRC,0,']','Z','X','C','V','B','N','M',';',':','_',0,
        0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        },
        /*AltGr*/
        {0,0,'A',0,0,0,0,0,0,0,0,0,'\\',0,0,0,'@',0,0,0,0,0,0,0,0,0,0,'~',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0
        }
};


static char accentChar[][MAX_ACCENT]=
{
        {A_MIN,E_MIN,I_MIN,O_MIN,U_MIN},/*tilde*/
        {A_MIN2,E_MIN2,I_MIN2,O_MIN2,U_MIN2},/*tilde inverso*/
        {A_MIN3,E_MIN3,I_MIN3,O_MIN3,U_MIN3},/*dierecis*/
	{A_MIN4,E_MIN4,I_MIN4,O_MIN4,U_MIN4,}/*tilde sombrero*/
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

static char
GetAccent(char ascii,int accent)
{
	/*Segun que vocal sea busca en la matriz el ascii de dicha vocal acentuada*/
        switch(ascii){
            case 'a':ascii=accentChar[accent-1][0];
                    break;
            case 'e':ascii=accentChar[accent-1][1];
                    break;
            case 'i':ascii=accentChar[accent-1][2];
                    break;
            case 'o':ascii=accentChar[accent-1][3];
                    break;
            case 'u':ascii=accentChar[accent-1][4];
                    break;
            default:break;
                     }

        return ascii;
}


static int
IsAccentCode(char code)
{
	/*Checkea los flags y en funcion de la tecla presionada
	 * avisa que se trata de una tecla acentuada;*/
	int resp=0;
	if( code==0x1A && !shift && !altgr)
	{
		/*Acento comun*/
		resp=1;
		accent=1;
	}
	else if( code==0x1A && shift && !altgr )
	{
		/*Dierecis*/
		resp=1;
		accent=3;
	}


	return resp;

}

char
ToAsciiLAT(char code)
{
        char ascii=0;
        if(!IsAccentCode(code))
        {

        	if( accent==0 && altgr==0 )
        	{
            		ascii=tecladoLAT[shift][code];
            		if( caps && !shift && IsLetter(ascii) )
                		ascii=ToUpper(ascii);
            		else if( caps && shift && IsLetter(ascii) )
                		ascii=ToLower(ascii);
        	}
        	else if( accent==0 && altgr!=0 )
        	{
        		ascii=tecladoLAT[2][code];
        	}
        	else
        	{
        		ascii=tecladoLAT[shift][code];
           		if( (!caps && !shift) || ((caps && shift) && ES_VOCAL_MIN(ascii)) )
              		ascii=GetAccent(ascii,accent);
           		accent=0;/*Una vez que tengo la vocal acentuada seteo el flag de vuelta a 0*/
        	}
        }
        return ascii;
}
