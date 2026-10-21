#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godinaRodjenja;
    struct Osoba *sljedeci;
} Osoba;

// A. Dodavanje osobe na pocetak liste
Osoba *dodajNaPocetak(Osoba *lista) {
    Osoba *nova = (Osoba *)malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return lista;
    }

    printf("Unesite ime: ");
    scanf("%49s", nova->ime);

    printf("Unesite prezime: ");
    scanf("%49s", nova->prezime);

    printf("Unesite godinu rodenja: ");
    scanf("%d", &nova->godinaRodjenja);

    nova->sljedeci = lista;

    return nova;
}

// B. Ispisivanje liste
void ispisiListu(Osoba *lista) {
    Osoba *tekuci = lista;

    if (tekuci == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    while (tekuci != NULL) {
        printf("%s %s, godina rodenja: %d\n",
               tekuci->ime,
               tekuci->prezime,
               tekuci->godinaRodjenja);

        tekuci = tekuci->sljedeci;
    }
}

// C. Dodavanje osobe na kraj liste
Osoba *dodajNaKraj(Osoba *lista) {
    Osoba *nova = (Osoba *)malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return lista;
    }

    printf("Unesite ime: ");
    scanf("%49s", nova->ime);

    printf("Unesite prezime: ");
    scanf("%49s", nova->prezime);

    printf("Unesite godinu rodenja: ");
    scanf("%d", &nova->godinaRodjenja);

    nova->sljedeci = NULL;

    if (lista == NULL) {
        return nova;
    }

    Osoba *tekuci = lista;

    while (tekuci->sljedeci != NULL) {
        tekuci = tekuci->sljedeci;
    }

    tekuci->sljedeci = nova;

    return lista;
}

// D. Pronalaženje osobe prema prezimenu
Osoba *pronadjiOsobu(Osoba *lista, const char *prezime) {
    Osoba *tekuci = lista;

    while (tekuci != NULL) {
        if (strcmp(tekuci->prezime, prezime) == 0) {
            return tekuci;
        }

        tekuci = tekuci->sljedeci;
    }

    return NULL;
}

// E. Brisanje osobe prema prezimenu
Osoba *obrisiOsobu(Osoba *lista, const char *prezime) {
    Osoba *tekuci = lista;
    Osoba *prethodni = NULL;

    while (tekuci != NULL &&
           strcmp(tekuci->prezime, prezime) != 0) {
        prethodni = tekuci;
        tekuci = tekuci->sljedeci;
    }

    if (tekuci == NULL) {
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return lista;
    }

    if (prethodni == NULL) {
        lista = tekuci->sljedeci;
    } else {
        prethodni->sljedeci = tekuci->sljedeci;
    }

    free(tekuci);

    printf("Osoba je uspjesno obrisana.\n");
    return lista;
}

// Oslobadanje memorije svih elemenata liste
void oslobodiListu(Osoba *lista) {
    while (lista != NULL) {
        Osoba *zaBrisanje = lista;
        lista = lista->sljedeci;
        free(zaBrisanje);
    }
}

int main(void) {
    Osoba *lista = NULL;
    Osoba *pronadena;
    char prezime[50];
    int izbor;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Ispisi listu\n");
        printf("3. Dodaj osobu na kraj\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos.\n");
            break;
        }

        switch (izbor) {
            case 1:
                lista = dodajNaPocetak(lista);
                break;

            case 2:
                ispisiListu(lista);
                break;

            case 3:
                lista = dodajNaKraj(lista);
                break;

            case 4:
                printf("Unesite prezime za pretragu: ");
                scanf("%49s", prezime);

                pronadena = pronadjiOsobu(lista, prezime);

                if (pronadena != NULL) {
                    printf("Pronadena osoba: %s %s, godina rodenja: %d\n",
                           pronadena->ime,
                           pronadena->prezime,
                           pronadena->godinaRodjenja);
                } else {
                    printf("Osoba nije pronadena.\n");
                }
                break;

            case 5:
                printf("Unesite prezime za brisanje: ");
                scanf("%49s", prezime);

                lista = obrisiOsobu(lista, prezime);
                break;

            case 0:
                printf("Izlaz iz programa.\n");
                break;

            default:
                printf("Neispravan odabir.\n");
        }

    } while (izbor != 0);

    oslobodiListu(lista);

    return 0;
}
