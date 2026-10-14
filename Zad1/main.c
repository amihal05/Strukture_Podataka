#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char ime[50];
    char prezime[50];
    float bodovi;
} Student;

int main() {
    FILE *dat;
    Student *studenti;
    int n, i;
    int max_br_bodova;

    dat = fopen("studenti.txt", "r");

    if (dat == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    // Ucitavanje broja studenata
    fscanf(dat, "%d", &n);

    // Dinamicka alokacija memorije
    studenti = malloc(n * sizeof(Student));

    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(dat);
        return 1;
    }

    // Ucitavanje podataka o studentima
    for (i = 0; i < n; i++) {
        fscanf(dat, "%s \n %s \n %f \n\n",
               studenti[i].ime,
               studenti[i].prezime,
               &studenti[i].bodovi);
    }

    fclose(dat);

    // Ispis rezultata
    printf("%-15s %-15s %-15s %-15s\n",
           "Ime", "Prezime", "Bodovi", "Relativni bodovi");

    for (i = 0; i < n; i++) {
        float relativni_bodovi =
            studenti[i].bodovi / max_br_bodova * 100;

        printf("%-15s %-15s %-15.2f %-15.2f\n",
               studenti[i].ime,
               studenti[i].prezime,
               studenti[i].bodovi,
               relativni_bodovi);
    }

    // Oslobadanje memorije
    free(studenti);

    return 0;
}
