#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godinaRodjenja;
    struct Osoba* sljedeci;
} Osoba;

// Unos podataka i dinamicka alokacija nove osobe
Osoba* kreirajOsobu(void) {
    Osoba* nova = malloc(sizeof(Osoba));

    if (nova == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    printf("Unesite ime: ");
    if (scanf("%49s", nova->ime) != 1) {
        free(nova);
        return NULL;
    }

    printf("Unesite prezime: ");
    if (scanf("%49s", nova->prezime) != 1) {
        free(nova);
        return NULL;
    }

    printf("Unesite godinu rodenja: ");
    if (scanf("%d", &nova->godinaRodjenja) != 1) {
        free(nova);
        return NULL;
    }

    nova->sljedeci = NULL;
    return nova;
}

// A. Dodavanje na pocetak
Osoba* dodajNaPocetak(Osoba* lista) {
    Osoba* nova = kreirajOsobu();

    if (nova == NULL)
        return lista;

    nova->sljedeci = lista;
    return nova;
}

// B. Ispis liste
void ispisiListu(Osoba* lista) {
    if (lista == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    while (lista != NULL) {
        printf("%s %s, godina rodenja: %d\n",
            lista->ime,
            lista->prezime,
            lista->godinaRodjenja);

        lista = lista->sljedeci;
    }
}

// C. Dodavanje na kraj
Osoba* dodajNaKraj(Osoba* lista) {
    Osoba* nova = kreirajOsobu();

    if (nova == NULL)
        return lista;

    if (lista == NULL)
        return nova;

    Osoba* tekuci = lista;

    while (tekuci->sljedeci != NULL)
        tekuci = tekuci->sljedeci;

    tekuci->sljedeci = nova;
    return lista;
}

// D. Pronalaženje po prezimenu
Osoba* pronadjiOsobu(Osoba* lista, const char* prezime) {
    while (lista != NULL) {
        if (strcmp(lista->prezime, prezime) == 0)
            return lista;

        lista = lista->sljedeci;
    }

    return NULL;
}

// E. Brisanje po prezimenu
Osoba* obrisiOsobu(Osoba* lista, const char* prezime) {
    Osoba* tekuci = lista;
    Osoba* prethodni = NULL;

    while (tekuci != NULL &&
        strcmp(tekuci->prezime, prezime) != 0) {
        prethodni = tekuci;
        tekuci = tekuci->sljedeci;
    }

    if (tekuci == NULL) {
        printf("Osoba nije pronadena.\n");
        return lista;
    }

    if (prethodni == NULL)
        lista = tekuci->sljedeci;
    else
        prethodni->sljedeci = tekuci->sljedeci;

    free(tekuci);
    printf("Osoba je obrisana.\n");

    return lista;
}

// F. Dodavanje iza osobe s odredenim prezimenom
Osoba* dodajIza(Osoba* lista, const char* prezime) {
    Osoba* tekuci = pronadjiOsobu(lista, prezime);

    if (tekuci == NULL) {
        printf("Osoba nije pronadena.\n");
        return lista;
    }

    Osoba* nova = kreirajOsobu();

    if (nova == NULL)
        return lista;

    nova->sljedeci = tekuci->sljedeci;
    tekuci->sljedeci = nova;

    return lista;
}

// G. Dodavanje ispred osobe s odredenim prezimenom
Osoba* dodajIspred(Osoba* lista, const char* prezime) {
    Osoba* tekuci = lista;
    Osoba* prethodni = NULL;

    while (tekuci != NULL &&
        strcmp(tekuci->prezime, prezime) != 0) {
        prethodni = tekuci;
        tekuci = tekuci->sljedeci;
    }

    if (tekuci == NULL) {
        printf("Osoba nije pronadena.\n");
        return lista;
    }

    Osoba* nova = kreirajOsobu();

    if (nova == NULL)
        return lista;

    nova->sljedeci = tekuci;

    if (prethodni == NULL)
        return nova;

    prethodni->sljedeci = nova;
    return lista;
}

// H. Sortiranje po prezimenu (abecedno)
void sortirajListu(Osoba* lista) {
    if (lista == NULL)
        return;

    for (Osoba* i = lista; i->sljedeci != NULL; i = i->sljedeci) {
        for (Osoba* j = i->sljedeci; j != NULL; j = j->sljedeci) {
            if (strcmp(i->prezime, j->prezime) > 0) {
                char ime[50];
                char prezime[50];
                int godina;

                strcpy(ime, i->ime);
                strcpy(i->ime, j->ime);
                strcpy(j->ime, ime);

                strcpy(prezime, i->prezime);
                strcpy(i->prezime, j->prezime);
                strcpy(j->prezime, prezime);

                godina = i->godinaRodjenja;
                i->godinaRodjenja = j->godinaRodjenja;
                j->godinaRodjenja = godina;
            }
        }
    }

    printf("Lista je sortirana.\n");
}

// I. Zapisivanje liste u datoteku
void zapisiUDatoteku(Osoba* lista, const char* naziv) {
    FILE* dat = fopen(naziv, "w");

    if (dat == NULL) {
        printf("Datoteka se ne moze otvoriti za pisanje.\n");
        return;
    }

    while (lista != NULL) {
        if (fprintf(dat, "%s %s %d\n",
            lista->ime,
            lista->prezime,
            lista->godinaRodjenja) < 0) {
            printf("Greska pri zapisivanju.\n");
            fclose(dat);
            return;
        }

        lista = lista->sljedeci;
    }

    if (fclose(dat) != 0) {
        printf("Greska pri zatvaranju datoteke.\n");
        return;
    }

    printf("Lista je zapisana u datoteku.\n");
}

// J. Oslobadanje svih elemenata
void oslobodiListu(Osoba* lista) {
    while (lista != NULL) {
        Osoba* zaBrisanje = lista;
        lista = lista->sljedeci;
        free(zaBrisanje);
    }
}

// K. Ucitavanje liste iz datoteke
Osoba* ucitajIzDatoteke(Osoba* lista, const char* naziv) {
    FILE* dat = fopen(naziv, "r");

    if (dat == NULL) {
        printf("Datoteka se ne moze otvoriti za citanje.\n");
        return lista;
    }

    Osoba* novaLista = NULL;
    Osoba* zadnji = NULL;
    char ime[50];
    char prezime[50];
    int godina;
    int rezultat;

    while ((rezultat = fscanf(dat, "%49s %49s %d",
        ime, prezime, &godina)) == 3) {
        Osoba* nova = malloc(sizeof(Osoba));

        if (nova == NULL) {
            printf("Greska pri alokaciji memorije.\n");
            oslobodiListu(novaLista);
            fclose(dat);
            return lista;
        }

        strcpy(nova->ime, ime);
        strcpy(nova->prezime, prezime);
        nova->godinaRodjenja = godina;
        nova->sljedeci = NULL;

        if (novaLista == NULL)
            novaLista = nova;
        else
            zadnji->sljedeci = nova;

        zadnji = nova;
    }

    if (ferror(dat) || rezultat != EOF) {
        printf("Greska: neispravan format datoteke.\n");
        oslobodiListu(novaLista);
        fclose(dat);
        return lista;
    }

    if (fclose(dat) != 0) {
        printf("Greska pri zatvaranju datoteke.\n");
        oslobodiListu(novaLista);
        return lista;
    }

    oslobodiListu(lista);
    printf("Lista je ucitana iz datoteke.\n");

    return novaLista;
}

int main(void) {
    Osoba* lista = NULL;
    Osoba* pronadena;
    char prezime[50];
    int izbor;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj na pocetak\n");
        printf("2. Ispisi listu\n");
        printf("3. Dodaj na kraj\n");
        printf("4. Pronadi osobu\n");
        printf("5. Obrisi osobu\n");
        printf("6. Dodaj iza odredene osobe\n");
        printf("7. Dodaj ispred odredene osobe\n");
        printf("8. Sortiraj po prezimenu\n");
        printf("9. Zapisi listu u datoteku\n");
        printf("10. Ucitaj listu iz datoteke\n");
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

            if (pronadena != NULL)
                printf("%s %s, godina rodenja: %d\n",
                    pronadena->ime, pronadena->prezime,
                    pronadena->godinaRodjenja);
            else
                printf("Osoba nije pronadena.\n");
            break;

        case 5:
            printf("Unesite prezime za brisanje: ");
            scanf("%49s", prezime);
            lista = obrisiOsobu(lista, prezime);
            break;

        case 6:
            printf("Iza osobe s kojim prezimenom dodati novu osobu? ");
            scanf("%49s", prezime);
            lista = dodajIza(lista, prezime);
            break;

        case 7:
            printf("Ispred osobe s kojim prezimenom dodati novu osobu? ");
            scanf("%49s", prezime);
            lista = dodajIspred(lista, prezime);
            break;

        case 8:
            sortirajListu(lista);
            break;

        case 9:
            zapisiUDatoteku(lista, "osobe.txt");
            break;

        case 10:
            lista = ucitajIzDatoteke(lista, "osobe.txt");
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
