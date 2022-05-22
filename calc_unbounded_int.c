#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "unbounded_int.h"

typedef struct variable
{
    char *nom;
    unbounded_int nombre;
    struct variable *suivant;
} variable;

typedef struct list_nombre
{
    variable *premier;
} list_nombre;

static variable *new_var(char *nom, unbounded_int nombre, variable *suivant)
{
    variable *v = malloc(sizeof(variable));
    if (v == NULL)
    {
        abort();
    }
    v->nom = nom;
    v->nombre = nombre;
    v->suivant = suivant;
    return v;
}

static int isOperationSign(char c)
{
    switch (c)
    {
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '*':
        return 1;
        break;
    }
    return -1;
}

static char *catch_word(char *str, int *taille)
{
    size_t x = 0;
    int i = 0;
    while (str[i] != ' ' && str[i] != '=')
    {
        if (str[i] == '(' || str[i] == ')')
        {
            printf("Il ne doit pas y avoir de parenthèse dans le fichier : variable %c\n", str[i]);
            exit(1);
        }
        if (str[i] != ' ' && !atoi(str + i) && isOperationSign(str[i]) == 1 && isOperationSign(str[i + 1]) == 1)
        {
            printf("L'opération est incorrecte : variable %c\n", str[i]);
            exit(1);
        }
        // Ne fonctionne pas avec le cas a=a - b sans qu'on puisse trouver la raison
        /*if ((str[i] != ' ' && isOperationSign(str[i + 1]) == 1) || (isOperationSign(str[i]) == 1 && isalpha(str[i + 1])))
        {
            printf("Les opérations doivent être entourées d'espaces : variable %c\n", str[i]);
            exit(1);
        }*/
        x += 1;
        i++;
    }
    char *res = malloc(x + 1);
    *taille = x;
    memmove(res, str, x);
    return res;
}

static list_nombre *create_list()
{
    list_nombre *l = malloc(sizeof(list_nombre));
    if (l == NULL)
        abort();
    l->premier = NULL;
    return l;
}

static variable *find_var(char *nom, list_nombre *l)
{
    variable *v = l->premier;
    while (v != NULL)
    {
        if (strcmp(v->nom, nom) == 0)
        {
            return v;
        }
        v = v->suivant;
    }
    return NULL;
}

// static void print_variable(variable *v)
// {
//     printf("%s = %s\n", v->nom, unbounded_int2string(v->nombre));
// }

// static void print_l(list_nombre *l)
// {
//     variable *v = l->premier;
//     while (v != NULL)
//     {
//         print_variable(v);
//         v = v->suivant;
//     }
// }

static char *supprimeEspace(char *str)
{
    char *fin;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    fin = str + strlen(str) - 1;
    while (fin > str && isspace((unsigned char)*fin))
        fin--;
    fin[1] = '\0';
    return str;
}

static void interpreter(FILE *f1, FILE *f2)
{
    list_nombre *liste_nombre = create_list();
    char *ligne = malloc(sizeof(char) * 1024);
    while (fgets(ligne, 1024, f1) != NULL)
    {
        size_t size_ligne = strlen(ligne);
        char *tab[4];

        for (int h = 0; h < 4; h++)
        {
            tab[h] = NULL;
        }

        int i = 0;
        int j = 0;
        int nbOperation = 0;
        while (i < size_ligne)
        {
            if (ligne[i] == ' ' || ligne[i] == '=')
            {
                i++;
            }
            else
            {
                int t = 0;
                char *word = catch_word(ligne + i, &t);
                size_t word_len = strlen(word);
                if (word_len == 1 && isOperationSign(word[0]) == 1)
                {
                    nbOperation += 1;
                }
                tab[j] = word;
                i += t;
                j++;
            }
        }
        if (nbOperation > 1)
        {
            printf("Une seule opération est possible par ligne\n");
            exit(1);
        }

        if (strcmp(supprimeEspace(tab[0]), "print") == 0)
        {
            if (atoi(supprimeEspace(tab[1])))
            {
                unbounded_int nbr = string2unbounded_int(supprimeEspace(tab[1]));
                char *res = unbounded_int2string(nbr);
                fprintf(f2, "%s", res);
            }
            else
            {
                variable *v = find_var(supprimeEspace(tab[1]), liste_nombre);
                if (v != NULL)
                {
                    char *res = unbounded_int2string(v->nombre);
                    fprintf(f2, "%s\n", res);
                }
            }
        }
        else
        {
            if (tab[2] == NULL)
            {
                unbounded_int n = string2unbounded_int(supprimeEspace(tab[1]));
                variable *v = new_var(supprimeEspace(tab[0]), n, liste_nombre->premier);
                liste_nombre->premier = v;
            }
            else
            {
                if (supprimeEspace(tab[1]) != NULL && supprimeEspace(tab[3]) != NULL)
                {
                    variable *tab0 = find_var(supprimeEspace(tab[0]), liste_nombre);
                    variable *tab1 = find_var(supprimeEspace(tab[1]), liste_nombre);
                    variable *tab3 = find_var(supprimeEspace(tab[3]), liste_nombre);
                    if (atoi(supprimeEspace(tab[1])) && tab3 != NULL)
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            unbounded_int res = unbounded_int_somme(string2unbounded_int(supprimeEspace(tab[1])), tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "-") == 0)
                        {
                            unbounded_int res = unbounded_int_difference(string2unbounded_int(supprimeEspace(tab[1])), tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            unbounded_int res = unbounded_int_produit(string2unbounded_int(supprimeEspace(tab[1])), tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                    }

                    else if (tab1 != NULL && atoi(supprimeEspace(tab[3])))
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            unbounded_int res = unbounded_int_somme(tab1->nombre, string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "-") == 0)
                        {
                            unbounded_int res = unbounded_int_difference(tab1->nombre, string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            unbounded_int res = unbounded_int_produit(tab1->nombre, string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                    }
                    else if (atoi(supprimeEspace(tab[1])) && atoi(supprimeEspace(tab[3])))
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            unbounded_int res = unbounded_int_somme(string2unbounded_int(supprimeEspace(tab[1])), string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "-") == 0)
                        {
                            unbounded_int res = unbounded_int_difference(string2unbounded_int(supprimeEspace(tab[1])), string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        else if (strcmp(tab[2], "*") == 0)
                        {
                            unbounded_int res = unbounded_int_produit(string2unbounded_int(supprimeEspace(tab[1])), string2unbounded_int(supprimeEspace(tab[3])));
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                    }
                    else if (tab1 != NULL && tab3 != NULL)
                    {
                        if (strcmp(tab[2], "+") == 0)
                        {
                            unbounded_int res = unbounded_int_somme(tab1->nombre, tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        if (strcmp(tab[2], "-") == 0)
                        {
                            unbounded_int res = unbounded_int_difference(tab1->nombre, tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                        if (strcmp(tab[2], "*") == 0)
                        {
                            unbounded_int res = unbounded_int_produit(tab1->nombre, tab3->nombre);
                            if (tab0 == NULL)
                            {
                                variable *v = new_var(supprimeEspace(tab[0]), res, liste_nombre->premier);
                                liste_nombre->premier = v;
                            }
                            else
                                tab0->nombre = res;
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc > 5)
    {
        printf("Nombre d'arguments invalide\n");
        exit(1);
    }
    FILE *f1;
    FILE *f2;
    if (argc > 1 && strcmp(argv[1], "-i") == 0)
    {
        f1 = fopen(argv[2], "r");
        if (f1 == NULL)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
        if (argc > 3 && strcmp(argv[3], "-o") == 0)
        {
            f2 = fopen(argv[4], "w");
            interpreter(f1, f2);
            fclose(f2);
        }
        else
        {
            interpreter(f1, stdout);
        }
        fclose(f1);
    }
    else if (argc > 1 && strcmp(argv[1], "-o") == 0)
    {
        f2 = fopen(argv[2], "w");
        interpreter(stdin, f2);
        fclose(f2);
    }
    else
    {
        interpreter(stdin, stdout);
    }
}
