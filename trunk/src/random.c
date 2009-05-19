#include "../include/random.h"

int seed = 1;
int
random (int tope) {
    int reloj;
    int resp;
    reloj = _leo_reloj ();
    reloj *= NUMERO_PRIMO;
    reloj *= seed;
    seed = (seed > 30000)?1:seed+1;
    if (reloj <= 0)
        reloj *= -1;
    resp = reloj % tope;
    return resp;
}

