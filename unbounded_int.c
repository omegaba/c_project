#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

unbounded_int string2unbounded_int(const char *e)
{
    unbounded_int *unbounded = malloc(sizeof(unbounded_int));
    if (unbounded == NULL)
    {
        unbounded->signe = '*';
        return *unbounded;
    }
    int j = 0;
    size_t len = strlen(e);
    if (e[j] == '-')
    {
        unbounded->signe = '-';
        j++;
        unbounded->length = len - 1;
    }
    else if (e[j] == '+')
    {
        unbounded->signe = '+';
        j++;
        unbounded->length = len - 1;
    }
    else
    {
        unbounded->length = len;
        unbounded->signe = '+';
    }
    unbounded->premier = new_chiffre(e[j], NULL, NULL);
    unbounded->dernier = unbounded->premier;
    chiffre *tmp = unbounded->premier;
    for (int i = 1 + j; i < len; i++)
    {
        tmp->suivant = new_chiffre(e[i], NULL, unbounded->dernier);
        tmp = tmp->suivant;
        unbounded->dernier = tmp;
    }
    // printf("%zu\n", unbounded->length);
    // printf("%c\n", unbounded->premier->c);
    // printf("%c\n", unbounded->dernier->c);
    return *unbounded;
}

unbounded_int ll2unbounded_int(long long i)
{
    int long_to_int = i;
    char *c = malloc(sizeof(char));
    sprintf(c, "%d", long_to_int);
    return string2unbounded_int(c);
}

char *unbounded_int2string(unbounded_int i)
{
    chiffre *tmp = (&i)->premier;
    char *c = malloc(sizeof(char) * (&i)->length);
    int j = 0;
    while (tmp != NULL)
    {
        c[j++] = tmp->c;
        tmp = tmp->suivant;
    }
    return c;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b)
{

    if ((&a)->length == (&b)->length)
    {
        chiffre *tmp_a = (&a)->premier;
        chiffre *tmp_b = (&b)->premier;
        while (tmp_a != NULL && tmp_a->c == tmp_b->c)
        {
            tmp_a = tmp_a->suivant;
            tmp_b = tmp_b->suivant;
        }

        return tmp_a == NULL ? 0 : tmp_a->c < tmp_b->c ? -1
                                                       : 1;
    }
    else
    {
        return (&a)->length < (&b)->length ? -1 : 1;
    }
    // int entier1 = atoi(unbounded_int2string(a));
    // int entier2 = atoi(unbounded_int2string(b));
    // return entier1 < entier2 ? -1 : entier1 == entier2 ? 0
    //    : 1;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b)
{
    // int entier = atoi(unbounded_int2string(a));
    int b_to_int = b;
    char *c = malloc(sizeof(char));
    sprintf(c, "%d", b_to_int);
    unbounded_int entier2 = string2unbounded_int(c);
    return unbounded_int_cmp_unbounded_int(a, entier2);
    // return entier < b ? -1 : entier == b ? 0
    //  : 1;
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

    char *s_reverse = malloc(sizeof(char));
    int length = strlen(s);
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

    char *s_reverse = malloc(sizeof(char));
    int length = strlen(s);
    int j = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        s_reverse[j] = s[i];
        j++;
    }

    return string2unbounded_int(s_reverse);
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b)
{

    if ((&a)->signe == '+' && (&b)->signe == '+')
    {
        unbounded_int res = unbounded_int_addition_nb_positif(a, b);
        (&res)->signe = '+';
        return res;
    }
    else if ((&a)->signe == '-' && (&b)->signe == '-')
    {
        unbounded_int res = unbounded_int_addition_nb_positif(a, b);
        (&res)->signe = '-';
        return res;
    }
    else if ((&a)->signe == '+' && (&b)->signe == '-')
    {
        if (unbounded_int_cmp_unbounded_int(a, b) == -1)
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(b, a);
            (&res)->signe = '-';
            return res;
        }
        else
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(a, b);
            (&res)->signe = '+';
            return res;
        }
    }
    else
    {
        if (unbounded_int_cmp_unbounded_int(a, b) == -1)
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(b, a);
            (&res)->signe = '+';
            return res;
        }
        else
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(a, b);
            (&res)->signe = '-';
            return res;
        }
    }
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b)
{
    if ((&a)->signe == '+' && (&b)->signe == '+')
    {
        if (unbounded_int_cmp_unbounded_int(a, b) == -1)
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(b, a);
            (&res)->signe = '-';
            return res;
        }
        else
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(a, b);
            (&res)->signe = '+';
            return res;
        }
    }
    else if ((&a)->signe == '-' && (&b)->signe == '-')
    {
        if (unbounded_int_cmp_unbounded_int(a, b) == -1)
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(b, a);
            (&res)->signe = '+';
            return res;
        }
        else
        {
            unbounded_int res = unbounded_int_soustraction_nb_positif(a, b);
            (&res)->signe = '-';
            return res;
        }
    }
    else if ((&a)->signe == '+' && (&b)->signe == '-')
    {
        unbounded_int res = unbounded_int_addition_nb_positif(a, b);
        (&res)->signe = '+';
        return res;
    }
    else
    {
        unbounded_int res = unbounded_int_addition_nb_positif(b, a);
        (&res)->signe = '-';
        return res;
    }
}

static unbounded_int unbounded_int_multiplication_nb_positif(unbounded_int a, unbounded_int b)
{
    print_liste(&a);
    printf("\n");
    print_liste(&b);
    printf("\n");

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
    printf("%s\n", s);

    return string2unbounded_int(s + nb_zero);
}

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b)
{
    unbounded_int res;
    if ((&a)->length > (&b)->length)
    {
        res = unbounded_int_multiplication_nb_positif(a, b);
    }
    else
    {
        res = unbounded_int_multiplication_nb_positif(b, a);
    }
    if (((&a)->signe == '-' && (&b)->signe == '+') || ((&a)->signe == '+' && (&b)->signe == '-'))
    {
        (&res)->signe = '-';
        return res;
    }
    else
    {
        (&res)->signe = '+';
        return res;
    }
}

int main(void)
{
    unbounded_int unbounded = string2unbounded_int("421");
    print_liste((&unbounded));
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