#include "unbounded_int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void print_chiffre(chiffre *ch)
{
    if (ch->suivant != NULL)
    {
        printf("%c", ch->c);
        print_chiffre(ch->suivant);
    }
    else
        printf("%c", ch->c);
}

static void print_liste(unbounded_int *nombre)
{
    if (nombre->signe == '-')
        printf("-");
    print_chiffre(nombre->premier);
}

static chiffre *new_chiffre(char e, chiffre *suivant, chiffre *precedent)
{
    chiffre *c = malloc(sizeof(chiffre));
    if (c == NULL)
        return NULL;
    c->suivant = suivant;
    c->precedent = precedent;
    c->c = e;
    return c;
}

static unbounded_int unbounded_int_addition_nb_positif(unbounded_int a, unbounded_int b)
{
    int r = 0;
    char *s = malloc(sizeof(char));
    int len = 0;
    chiffre *tmp_a = (&a)->dernier;
    chiffre *tmp_b = (&b)->dernier;
    int int_a;
    int int_b;
    for (int i = 0; i < (&a)->length && i < (&b)->length; i++)
    {
        int_a = tmp_a->c - '0';
        int_b = tmp_b->c - '0';
        s[len] = (char)((int_a + int_b + r) % 10 + '0');
        r = (int_a + int_b + r) / 10;
        tmp_a = tmp_a->precedent;
        tmp_b = tmp_b->precedent;
        len++;
    }
    while (tmp_a != NULL)
    {
        int_a = tmp_a->c - '0';
        s[len] = (char)((int_a + r) + '0');
        r = 0;
        tmp_a = tmp_a->precedent;
        len++;
    }
    while (tmp_b != NULL)
    {
        int_b = tmp_b->c - '0';
        s[len] = (char)((int_b + r) + '0');
        r = 0;
        tmp_b = tmp_b->precedent;
        len++;
    }

    int length = strlen(s);
    char *s_reverse = malloc(sizeof(char) * length + 1);
    int j = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        s_reverse[j] = s[i];
        j++;
    }

    return string2unbounded_int(s_reverse);
}

static unbounded_int unbounded_int_soustraction_nb_positif(unbounded_int a, unbounded_int b)
{
    int r = 0;
    char *s = malloc(sizeof(char));
    int len = 0;
    chiffre *tmp_a = (&a)->dernier;
    chiffre *tmp_b = (&b)->dernier;
    int int_a;
    int int_b;
    for (int i = 0; i < (&a)->length && i < (&b)->length; i++)
    {
        int_a = tmp_a->c - '0';
        int_b = tmp_b->c - '0';
        if (int_a - int_b + r >= 0)
        {
            s[len] = (char)(int_a - int_b + r) + '0';
            r = 0;
        }
        else
        {
            s[len] = (char)(int_a - int_b + r + 10) + '0';
            r = -1;
        }
        tmp_a = tmp_a->precedent;
        tmp_b = tmp_b->precedent;
        len++;
    }
    while (tmp_a != NULL)
    {
        int_a = tmp_a->c - '0';
        s[len] = (char)(int_a + r) + '0';
        r = 0;
        tmp_a = tmp_a->precedent;
        len++;
    }

    int length = strlen(s);
    char *s_reverse = malloc(sizeof(char) * length + 1);
    int j = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        s_reverse[j] = s[i];
        j++;
    }

    return string2unbounded_int(s_reverse);
}
static unbounded_int unbounded_int_multiplication_nb_positif(unbounded_int a, unbounded_int b)
{
    int m = (&b)->length;
    int n = (&a)->length;
    int r;
    int int_a;
    int int_b;
    int len = 0;
    chiffre *chiffre_b = (&b)->dernier;
    char *s = malloc(sizeof(char));
    int c[m + n - 1];
    for (int i = 0; i < m + n - 1; i++)
    {
        c[i] = 0;
    }
    for (int j = 0; j < m; j++)
    {
        r = 0;
        int_b = chiffre_b->c - '0';
        if (int_b == 0)
            continue;
        chiffre *chiffre_a = (&a)->dernier;
        for (int i = 0; i < n; i++)
        {
            int_a = chiffre_a->c - '0';
            int v = c[i + j] + int_a * int_b + r;
            c[i + j] = v % 10;
            r = v / 10;
            chiffre_a = chiffre_a->precedent;
        }
        c[j + n] = r;
        chiffre_b = chiffre_b->precedent;
    }
    for (int i = m + n - 1; i >= 0; i--)
    {
        s[len++] = (char)(c[i] + '0');
    }
    int nb_zero = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == '0')
            nb_zero++;
        else
            break;
    }
    return string2unbounded_int(s + nb_zero);
}

int main(void)
{
    unbounded_int unbounded = string2unbounded_int("421");
    print_liste(&unbounded);
    printf("\n");
    long long x = 421;
    unbounded_int unbounded2 = ll2unbounded_int(x);
    print_liste((&unbounded2));
    printf("\n");
    char *c = unbounded_int2string(unbounded);
    printf("%s\n", c);

    unbounded_int a = string2unbounded_int("71");
    unbounded_int b = string2unbounded_int("421");
    printf("%d\n", unbounded_int_cmp_unbounded_int(a, b));
    printf("\n");
    printf("%d\n", unbounded_int_cmp_ll(a, x));

    printf("\n");
    unbounded_int add = unbounded_int_addition_nb_positif(a, b);
    unbounded_int sous = unbounded_int_soustraction_nb_positif(b, a);
    print_liste((&add));
    printf("\n");
    print_liste((&sous));
    printf("\n");

    unbounded_int sum = unbounded_int_somme(a, b);
    print_liste((&sum));
    printf("\n");

    unbounded_int a_multi = string2unbounded_int("897");
    unbounded_int b_multi = string2unbounded_int("897");
    unbounded_int a_fois_b = unbounded_int_produit(a_multi, b_multi);
    print_liste(&a_fois_b);
    printf("\n");

    printf("\n\n================TEST================\n\n");

    unbounded_int a_positif = string2unbounded_int("897");
    unbounded_int b_positif = string2unbounded_int("897");

    unbounded_int a_negatif = string2unbounded_int("-245");
    unbounded_int b_negatif = string2unbounded_int("-93");

    printf("Additions:\n");

    unbounded_int a_pos_plus_b_pos = unbounded_int_somme(a_positif, b_positif);
    print_liste((&a_positif));
    printf(" + ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_pos_plus_b_pos));
    printf("        | Attendu: 380\n");

    unbounded_int a_neg_plus_b_neg = unbounded_int_somme(a_negatif, b_negatif);
    print_liste((&a_negatif));
    printf(" + ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_neg_plus_b_neg));
    printf("        | Attendu: -338\n");

    unbounded_int a_pos_plus_b_neg = unbounded_int_somme(a_positif, b_negatif);
    print_liste((&a_positif));
    printf(" + ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_pos_plus_b_neg));
    printf("        | Attendu: 223\n");

    unbounded_int a_neg_plus_b_pos = unbounded_int_somme(a_negatif, b_positif);
    print_liste((&a_negatif));
    printf(" + ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_neg_plus_b_pos));
    printf("        | Attendu: -181\n\n");

    printf("Soustraction:\n");

    unbounded_int a_pos_moins_b_pos = unbounded_int_difference(a_positif, b_positif);
    print_liste((&a_positif));
    printf(" - ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_pos_moins_b_pos));
    printf("        | Attendu: 252\n");

    unbounded_int a_neg_moins_b_neg = unbounded_int_difference(a_negatif, b_negatif);
    print_liste((&a_negatif));
    printf(" - ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_neg_moins_b_neg));
    printf("        | Attendu: -152\n");

    unbounded_int a_pos_moins_b_neg = unbounded_int_difference(a_positif, b_negatif);
    print_liste((&a_positif));
    printf(" - ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_pos_moins_b_neg));
    printf("        | Attendu: 409\n");

    unbounded_int a_neg_moins_b_pos = unbounded_int_difference(a_negatif, b_positif);
    print_liste((&a_negatif));
    printf(" - ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_neg_moins_b_pos));
    printf("        | Attendu: -309\n\n");

    printf("Multiplication:\n");

    unbounded_int a_pos_fois_b_pos = unbounded_int_produit(a_positif, b_positif);
    print_liste((&a_positif));
    printf(" x ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_pos_fois_b_pos));
    printf("        | Attendu: 20224\n");

    unbounded_int a_neg_fois_b_neg = unbounded_int_produit(a_negatif, b_negatif);
    print_liste((&a_negatif));
    printf(" x ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_neg_fois_b_neg));
    printf("        | Attendu: 22785\n");

    unbounded_int a_pos_fois_b_neg = unbounded_int_produit(a_positif, b_negatif);
    print_liste((&a_positif));
    printf(" x ");
    print_liste((&b_negatif));
    printf(" = ");
    print_liste((&a_pos_fois_b_neg));
    printf("        | Attendu: -29388\n");

    unbounded_int a_neg_fois_b_pos = unbounded_int_produit(a_negatif, b_positif);
    print_liste((&a_negatif));
    printf(" x ");
    print_liste((&b_positif));
    printf(" = ");
    print_liste((&a_neg_fois_b_pos));
    printf("        | Attendu: -15680\n");

    printf("\n\n================FIN TEST================\n");
}
