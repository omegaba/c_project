#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
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

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b)
{
    if (((&a)->signe == '-' && (&b)->signe == '+') || ((&a)->signe == '+' && (&b)->signe == '-'))
    {
        unbounded_int res = unbounded_int_multiplication_nb_positif(a, b);
        (&res)->signe = '-';
        return res;
    }
    else
    {
        unbounded_int res = unbounded_int_multiplication_nb_positif(a, b);
        (&res)->signe = '+';
        return res;
    }
}

char *catch_word(char *str, int *taille)
{
    int i = 0;
    size_t x = 0;
    size_t len_string = strlen(str);
    while (str[i] != ' ' && str[i] != '=')
    {
        x += 1;
        i++;
    }
    char *res = malloc(x + 1);
    *taille = x;
    memmove(res, str, x);
    return res;
}

unbounded_int test3(unbounded_int a, unbounded_int b)
{
    unbounded_int res = unbounded_int_produit(a, b);
    return res;
}

void test2(FILE *f1, FILE *f2)
{
    char *alphabet[1024];
    char *ligne = malloc(sizeof(char) * 1024);
    while (!feof(f1))
    {
        fgets(ligne, 1024, f1);
        size_t size_ligne = strlen(ligne);
        char *tab[4];

        for (int h = 0; h < 4; h++)
        {
            tab[h] = NULL;
        }

        int i = 0;
        int j = 0;

        while (i < size_ligne)
        {
            if (ligne[i] == ' ' || ligne[i] == '=')
            {
                i++;
            }
            else
            {
                int t = 0;

                tab[j] = catch_word(ligne + i, &t);
                i += t;
                j++;
            }
        }
        if (strcmp(tab[0], "print") == 0)
        {
            if (atoi(tab[1]))
            {
                unbounded_int nbr = string2unbounded_int(tab[1]);
                char *res = unbounded_int2string(nbr);
                fprintf(f2, "%s\n", res);
            }
            else
            {
                if (alphabet[*tab[1] - 97] != NULL)
                {
                    unbounded_int nbr = string2unbounded_int(alphabet[*tab[1] - 97]);
                    char *res = unbounded_int2string(nbr);
                    fprintf(f2, "%s\n", res);
                }
            }
        }
        else
        {
            if (tab[2] == NULL)
            {
                alphabet[*tab[0] - 97] = tab[1];
            }
            else
            {
                if (tab[1] != NULL && tab[3] != NULL)
                {

                    if (atoi(tab[1]) && alphabet[*tab[3] - 97] != NULL)
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_somme(string2unbounded_int(tab[1]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                        else if (strcmp(tab[2], "-") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_difference(string2unbounded_int(tab[1]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_produit(string2unbounded_int(tab[1]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                    }

                    else if (alphabet[*tab[1] - 97] != NULL && atoi(tab[3]))
                    {
                        if (strcmp(tab[2], "-") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_difference(string2unbounded_int(tab[3]), string2unbounded_int(alphabet[*tab[1] - 97])));
                        }
                        else if (strcmp(tab[2], "+") == 0)
                        {

                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_somme(string2unbounded_int(tab[3]), string2unbounded_int(alphabet[*tab[1] - 97])));
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_produit(string2unbounded_int(tab[3]), string2unbounded_int(alphabet[*tab[1] - 97])));
                        }
                    }
                    else if (atoi(tab[1]) && atoi(tab[3]))
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_somme(string2unbounded_int(tab[3]), string2unbounded_int(tab[1])));
                        }
                        else if (strcmp(tab[2], "-") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_difference(string2unbounded_int(tab[3]), string2unbounded_int(tab[1])));
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_produit(string2unbounded_int(tab[3]), string2unbounded_int(tab[1])));
                        }
                    }
                    else if (alphabet[*tab[1] - 97] != NULL && alphabet[*tab[3] - 97] != NULL)
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_somme(string2unbounded_int(alphabet[*tab[1] - 97]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                        if (strcmp(tab[2], "-") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_difference(string2unbounded_int(alphabet[*tab[1] - 97]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                        if (strcmp(tab[2], "*") == 0)
                        {
                            alphabet[*tab[0] - 97] = unbounded_int2string(unbounded_int_produit(string2unbounded_int(alphabet[*tab[1] - 97]), string2unbounded_int(alphabet[*tab[3] - 97])));
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0)
    {
        FILE *f1 = fopen(argv[2], "r");
        FILE *f2 = fopen(argv[4], "w");
        if (f1 == NULL)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
        test2(f1, f2);
        fclose(f1);
        fclose(f2);
    }
    else if (strcmp(argv[1], "-i") == 1 && strcmp(argv[3], "-o") == 0)
    {
        FILE *f2 = fopen(argv[4], "w");
        test2(stdin, f2);
        fclose(f2);
    }
    else if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 1)
    {
        FILE *f1 = fopen(argv[2], "r");
        if (f1 == NULL)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
        test2(f1, stdout);
        fclose(f1);
    }

}
