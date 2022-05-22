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



int main(void)
{
   

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
