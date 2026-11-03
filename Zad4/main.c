#include <stdio.h>
#include <stdlib.h>

typedef struct Polinom {
    int koef;
    int exp;
    struct Polinom* sljedeci;
} Polinom;

// Kreiranje novog elementa
Polinom* kreiraj(int koef, int exp) {
    Polinom* novi = (Polinom*)malloc(sizeof(Polinom));

    if (novi == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }

    novi->koef = koef;
    novi->exp = exp;
    novi->sljedeci = NULL;

    return novi;
}

// Dodavanje clana u polinom
void dodaj(Polinom** glava, int koef, int exp) {
    if (koef == 0)
        return;

    Polinom* p = *glava;

    // Ako eksponent vec postoji, zbrajaju se koeficijenti
    while (p != NULL) {
        if (p->exp == exp) {
            p->koef += koef;
            return;
        }
        p = p->sljedeci;
    }

    Polinom* novi = kreiraj(koef, exp);

    novi->sljedeci = *glava;
    *glava = novi;
}

// Ucitavanje polinoma iz datoteke
void ucitaj(FILE* dat, Polinom** p) {
    int n, koef, exp;

    fscanf(dat, "%d", &n);

    for (int i = 0; i < n; i++) {
        fscanf(dat, "%d %d", &koef, &exp);
        dodaj(p, koef, exp);
    }
}

// Zbrajanje polinoma
Polinom* zbroji(Polinom* p1, Polinom* p2) {
    Polinom* rez = NULL;

    while (p1 != NULL) {
        dodaj(&rez, p1->koef, p1->exp);
        p1 = p1->sljedeci;
    }

    while (p2 != NULL) {
        dodaj(&rez, p2->koef, p2->exp);
        p2 = p2->sljedeci;
    }

    return rez;
}

// Mnozenje polinoma
Polinom* pomnozi(Polinom* p1, Polinom* p2) {
    Polinom* rez = NULL;

    Polinom* a = p1;

    while (a != NULL) {
        Polinom* b = p2;

        while (b != NULL) {
            dodaj(&rez,
                a->koef * b->koef,
                a->exp + b->exp);

            b = b->sljedeci;
        }

        a = a->sljedeci;
    }

    return rez;
}

// Ispis polinoma
void ispisi(Polinom* p) {
    if (p == NULL) {
        printf("0\n");
        return;
    }

    while (p != NULL) {
        if (p->koef > 0 && p != NULL)
            printf("+");

        printf("%dx^%d ", p->koef, p->exp);

        p = p->sljedeci;
    }

    printf("\n");
}

// Oslobadanje memorije
void oslobodi(Polinom* p) {
    Polinom* tmp;

    while (p != NULL) {
        tmp = p;
        p = p->sljedeci;
        free(tmp);
    }
}

int main() {
    FILE* dat;
    Polinom* p1 = NULL;
    Polinom* p2 = NULL;
    Polinom* zbroj = NULL;
    Polinom* produkt = NULL;

    dat = fopen("polinomi.txt", "r");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    ucitaj(dat, &p1);
    ucitaj(dat, &p2);

    fclose(dat);

    zbroj = zbroji(p1, p2);
    produkt = pomnozi(p1, p2);

    printf("Prvi polinom: ");
    ispisi(p1);

    printf("Drugi polinom: ");
    ispisi(p2);

    printf("Zbroj polinoma: ");
    ispisi(zbroj);

    printf("Produkt polinoma: ");
    ispisi(produkt);

    oslobodi(p1);
    oslobodi(p2);
    oslobodi(zbroj);
    oslobodi(produkt);

    return 0;
}
