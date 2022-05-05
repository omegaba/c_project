#include <stdlib.h>
#include <stdio.h>
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

/*void test(FILE *f1, FILE *f2)
{
    //int alphabet[26];
    char ligne[1024];
    while (fgets(ligne, 1024, f1))
    {
        for (int i = 0; i < strlen(ligne); i++)
        {
            if (ligne[i] == '=')
            {
                char variable[1];
                char nb1[1024];
                char operation[1024];
                char nb2[1024];
                unbounded_int nbr1, nbr2;
                sscanf(ligne, "%s%*s%s %s %s", variable, nb1, operation, nb2);

                if (nb1 != NULL && operation != NULL && nb2 != NULL)
                {

                    nbr1 = string2unbounded_int(nb1);
                    nbr2 = string2unbounded_int(nb2);
                    unbounded_int res;
                    switch (*operation)
                    {
                    case '*':
                        res = unbounded_int_produit(nbr1, nbr2);
                        break;
                    case '+':
                        res = unbounded_int_somme(nbr1, nbr2);
                        break;
                    case '-':
                        res = unbounded_int_difference(nbr1, nbr2);
                        break;
                    }
                    char *res2 = unbounded_int2string(res);
                    fputs(res2, f2);
                    fputs(" \n", f2);
                }
                if (operation == NULL && nb2 == NULL)
                {
                    printf("LF?EF");
                    unbounded_int res;
                    nbr1 = string2unbounded_int(nb1);
                    char *res2 = unbounded_int2string(res);
                    fputs(res2, f2);
                }
                break;
            }
            if(ligne[i]=='p' && ligne[i+1]=='r'){

            }
        }
    }
}

void test2(FILE *f, FILE *f2)
{
    char ligne[1024];
    while (fgets(ligne, 1024, f1))
    {
        char *token = strtok(str, separator);
        if (strcmp(token, "print") == 0)
        {
            token = strtok(NULL, separator);
            if (atoi(token))
            {
                unbounded_int nbr = string2unbounded_int(token);
                char *res = unbounded_int2string(nbr);
                fputs(res, f2);
                break;
            }
        }
        else
        {
            char tab[4];
            int i = 0;
            while (token != NULL)
            {
                tab[i] = *token;
                i++;
                token = strtok(NULL, separator);
            }
        }
    }
}
*/
void test2(FILE *f1, FILE *f2)
{
    char alphabet[1024];
    char ligne[1024];
    while (fgets(ligne, 1024, f1))
    {
        char separator[] = " =";
        char *token = strtok(ligne, separator);
        if (strcmp(token, "print") == 0)
        {
            token = strtok(NULL, separator);
            /*if (atoi(token))
            {
                unbounded_int nbr = string2unbounded_int(token);
                char *res = unbounded_int2string(nbr);
                fputs(res, f2);
                break;
            }
            else
            {*/
            if (alphabet[97 - (char)*token] != NULL)
            {
                unbounded_int nbr = string2unbounded_int(alphabet[97 - (char)*token]);
                char *res = unbounded_int2string(nbr);
                fputs(res, f2);
            }
            //}
        }
        else
        {
            char tab[4];
            int i = 0;
            while (token != NULL)
            {
                tab[i] = *token;
                i++;
                token = strtok(NULL, separator);
            }
            printf("%c\n", tab[0]);
            printf("%c\n", tab[1]);
            if (alphabet[97 - (char)tab[0]] != NULL)
            {

                if (tab[2] == 0)
                {
                    alphabet[97 - (char)tab[0]] = tab[1];
                }
                else
                {
                    if (alphabet[97 - (char)tab[1]] != NULL && alphabet[97 - (char)tab[3]] != NULL)
                    {
                        switch (tab[2])
                        {
                        case '+':
                            alphabet[97 - (char)tab[0]] = unbounded_int2string(unbounded_int_somme(string2unbounded_int(alphabet[97 - (char)tab[1]]), string2unbounded_int(alphabet[97 - (char)tab[3]])));
                            break;

                        case '-':
                            alphabet[97 - (char)tab[0]] = unbounded_int2string(unbounded_int_difference(string2unbounded_int(alphabet[97 - (char)tab[1]]), string2unbounded_int(alphabet[97 - (char)tab[3]])));
                            break;
                        case '*':
                            alphabet[97 - (char)tab[0]] = unbounded_int2string(unbounded_int_produit(string2unbounded_int(alphabet[97 - (char)tab[1]]), string2unbounded_int(alphabet[97 - (char)tab[3]])));
                            break;
                        }
                    }
                }
            }
            else
            {
                alphabet[97 - (char)tab[0]] = tab[1];
            }
        }
    }
}

int main(int argc, char **argv)
{
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "w");
    if (f2 == NULL)
    {
        FILE *f3 = fopen(argv[2], "w");
        test2(f1, f3);
    }
    else
    {
        test2(f1, f2);
    }
    fclose(f1);
    fclose(f2);*/
    /* char str[] = "a = 19789797 + 675454";
      char alphabet[26];
     char tab[5];
     const char *separator = " =";
     char *token = strtok(str, separator);
     int i = 0;
     while (token != NULL)
     {
         printf("%c\n", *token);
         tab[i] = *token;
         i++;
         token = strtok(NULL, separator);
     }


     alphabet[97-(char)tab[0]]=tab[2];
     if (alphabet[97 - (char)tab[0]] != NULL){
         printf("    aaaaaaaaa");
     }
     printf("%c\n",alphabet[97-(char)tab[0]]);*/
}
