#include "unbounded_int.h"

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

    unbounded_int a_multi = string2unbounded_int("14");
    unbounded_int b_multi = string2unbounded_int("257");
    unbounded_int a_fois_b = unbounded_int_produit(a_multi, b_multi);
    print_liste(&a_fois_b);
    printf("\n");

    printf("\n\n================TEST================\n\n");

    unbounded_int a_positif = string2unbounded_int("316");
    unbounded_int b_positif = string2unbounded_int("64");

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