
#include <stdio.h>
#include <stdlib.h>

typedef struct element {
    double broj;
    struct element* sljedeci;
} ELEMENT;

// Dodavanje elementa na stog
int Push(ELEMENT** top, double broj)
{
    ELEMENT* novi = (ELEMENT*)malloc(sizeof(ELEMENT));

    if (novi == NULL)
        return 0;

    novi->broj = broj;
    novi->sljedeci = *top;
    *top = novi;

    return 1;
}

// Uklanjanje elementa sa stoga
int Pop(ELEMENT** top, double* broj)
{
    ELEMENT* pom;

    if (*top == NULL)
        return 0;

    pom = *top;
    *broj = pom->broj;

    *top = pom->sljedeci;
    free(pom);

    return 1;
}

// Provjera je li stog prazan
int IsEmpty(ELEMENT* top)
{
    return top == NULL;
}

// Brisanje svih elemenata stoga
int BrisiStog(ELEMENT** top)
{
    double broj;

    while (!IsEmpty(*top)) {
        if (!Pop(top, &broj))
            return 0;
    }

    return 1;
}

// Racunanje postfiksnog izraza
int Izracunaj(FILE* dat, double* rezultat)
{
    ELEMENT* top = NULL;
    double a, b, broj;
    char znak;
    int status;

    while (1) {

        status = fscanf(dat, "%lf", &broj);

        if (status == EOF)
            break;

        if (status == 1) {
            if (!Push(&top, broj)) {
                BrisiStog(&top);
                return 0;
            }
        }
        else {
            znak = fgetc(dat);

            if (znak == EOF)
                break;

            if (znak == '+' || znak == '-' ||
                znak == '*' || znak == '/') {

                if (!Pop(&top, &b) || !Pop(&top, &a)) {
                    BrisiStog(&top);
                    return 0;
                }

                switch (znak) {
                case '+':
                    broj = a + b;
                    break;

                case '-':
                    broj = a - b;
                    break;

                case '*':
                    broj = a * b;
                    break;

                case '/':
                    if (b == 0) {
                        BrisiStog(&top);
                        return 0;
                    }
                    broj = a / b;
                    break;
                }

                if (!Push(&top, broj)) {
                    BrisiStog(&top);
                    return 0;
                }
            }
            else if (znak != ' ' && znak != '\n' && znak != '\t') {
                BrisiStog(&top);
                return 0;
            }
        }
    }

    // Na stogu mora ostati samo jedan element
    if (top == NULL || top->sljedeci != NULL) {
        BrisiStog(&top);
        return 0;
    }

    if (!Pop(&top, rezultat))
        return 0;

    return 1;
}

int main()
{
    FILE* dat;
    double rezultat;

    dat = fopen("izraz.txt", "r");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    if (!Izracunaj(dat, &rezultat)) {
        printf("Greska pri racunanju izraza!\n");
        fclose(dat);
        return 1;
    }

    fclose(dat);

    printf("Rezultat izraza je: %.2lf\n", rezultat);

    return 0;
}
