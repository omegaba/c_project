#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdlib.h>
#include <stdio.h>

typedef struct chiffre
{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct
{
    char signe;
    size_t length;
    chiffre *premier;
    chiffre *dernier;
} unbounded_int;

extern unbounded_int string2unbounded_int(const char *e);

extern unbounded_int ll2unbounded_int(long long i);

extern char *unbounded_int2string(unbounded_int i);

extern int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

extern int unbounded_int_cmp_ll(unbounded_int a, long long b);

extern unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

extern unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

extern unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

#endif
