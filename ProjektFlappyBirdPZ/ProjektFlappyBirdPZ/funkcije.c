#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include "header.h"

const char* ime_datoteke = "rezultati.txt";
pticaY, brzinaPtice;
StanjeIgre stanjeIgre;
int brojačProđenihPrepreka = 0;
static char ekran[VISINA][ŠIRINA];
static Prepreka preprekaGore, preprekaDolje, preprekaGore2, preprekaDolje2;

void prikazi_gresku(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void postavi() {
    pticaY = VISINA / 2;
    brzinaPtice = 0;
    stanjeIgre = ČEKA_POČETAK; // Postavljeno stanje igre na 1 (čeka početak)

    for (int y = 0; y < VISINA; y++) {
        for (int x = 0; x < ŠIRINA; x++) {
            ekran[y][x] = ' ';
        }
    }

    preprekaGore.x = ŠIRINA - 1;
    preprekaDolje.x = ŠIRINA - 1;
    preprekaGore2.x = ŠIRINA + ŠIRINA / 2 - 1; // Pojavljuje se kasnije
    preprekaDolje2.x = ŠIRINA + ŠIRINA / 2 - 1;
    preprekaGore.y = rand() % (VISINA - MIN_RAZMAK);
    preprekaDolje.y = preprekaGore.y + MIN_RAZMAK;
    preprekaGore2.y = rand() % (VISINA - MIN_RAZMAK);
    preprekaDolje2.y = preprekaGore2.y + MIN_RAZMAK;
    brojačProđenihPrepreka = 0;
}

void crtanje() {
    system("cls");

    for (int y = 0; y < VISINA; y++) {
        for (int x = 0; x < ŠIRINA; x++) {
            if ((y == pticaY || y == pticaY + 1) && (x == ŠIRINA / 3 || x == ŠIRINA / 3 + 1)) {
                printf(">");
            }
            else if (x == preprekaGore.x && y <= preprekaGore.y) {
                printf("|");
            }
            else if (x == preprekaDolje.x && y >= preprekaDolje.y) {
                printf("|");
            }
            else if (x == preprekaGore2.x && y <= preprekaGore2.y) {
                printf("|");
            }
            else if (x == preprekaDolje2.x && y >= preprekaDolje2.y) {
                printf("|");
            }
            else {
                printf("%c", ekran[y][x]);
            }
        }
        if (y == 0) { // Iscrtavanje brojača na vrhu ekrana
            printf("  Score: %d", brojačProđenihPrepreka);
        }
        printf("\n");
    }

    for (int x = 0; x < ŠIRINA; x++) {
        printf("-");
    }
    printf("\n");

    Sleep(BRZINA_PTICE);
}

void ažuriraj() {
    if (stanjeIgre == ČEKA_POČETAK) { // Čeka početak igre
        if (_kbhit()) {
            char tipka = _getch();
            if (tipka == 's' || tipka == 'S') {
                stanjeIgre = IGRA_POKRENUTA; // Promijeni stanje igre na pokrenuto
                brzinaPtice = -2; // Početna brzina ptice
            }
        }
    }
    else if (stanjeIgre == IGRA_POKRENUTA) { // Igra je pokrenuta
        if (_kbhit()) {
            char tipka = _getch();
            if (tipka == ' ') { // Ako je pritisnuta razmaknica ili lijevi klik miša
                brzinaPtice = -2; // Postavi brzinu ptice prema gore
            }
        }

        brzinaPtice += 1;
        pticaY += brzinaPtice;

        if (pticaY < 0 || pticaY + 1 >= VISINA) {
            stanjeIgre = IGRA_ZAVRŠENA;
            return;
        }

        // Provjeri koliziju s preprekom
        if (((preprekaGore.x == ŠIRINA / 3 || preprekaGore.x == ŠIRINA / 3 + 1) &&
            (pticaY <= preprekaGore.y || pticaY + 1 >= preprekaDolje.y)) ||
            ((preprekaGore2.x == ŠIRINA / 3 || preprekaGore2.x == ŠIRINA / 3 + 1) &&
                (pticaY <= preprekaGore2.y || pticaY + 1 >= preprekaDolje2.y))) {
            stanjeIgre = IGRA_ZAVRŠENA;
            return;
        }

        // Ažuriranje pozicije prepreka
        preprekaGore.x--;
        preprekaDolje.x--;
        preprekaGore2.x--;
        preprekaDolje2.x--;

        if (preprekaGore.x < 0 || preprekaDolje.x < 0) {
            preprekaGore.x = preprekaDolje.x = ŠIRINA - 1;

            // Odredi visinu gornje prepreke
            preprekaGore.y = rand() % (VISINA - MIN_RAZMAK);

            // Visina donje prepreke je određena na način da se osigura minimalni razmak
            preprekaDolje.y = preprekaGore.y + MIN_RAZMAK;
        }

        if (preprekaGore2.x < 0 || preprekaDolje2.x < 0) {
            preprekaGore2.x = preprekaDolje2.x = ŠIRINA - 1;

            // Odredi visinu gornje prepreke
            preprekaGore2.y = rand() % (VISINA - MIN_RAZMAK);

            // Visina donje prepreke je određena na način da se osigura minimalni razmak
            preprekaDolje2.y = preprekaGore2.y + MIN_RAZMAK;
        }

        // Provjeri prolazak između prepreka koristeći samo lijevu stranu ptice
        if (((preprekaGore.x == ŠIRINA / 3) && (pticaY > preprekaGore.y && pticaY + 1 < preprekaDolje.y)) ||
            ((preprekaGore2.x == ŠIRINA / 3) && (pticaY > preprekaGore2.y && pticaY + 1 < preprekaDolje2.y))) {
            brojačProđenihPrepreka++;
        }
    }
}

void zapis_u_datoteku(const char* ime, int rezultat) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne moze biti NULL\n");
        return;
    }

    FILE* datoteka = fopen(ime, "a");
    if (datoteka == NULL) {
        prikazi_gresku("Greska pri otvaranju datoteke");
        return;  // Prekid funkcije ako nije uspjelo otvaranje datoteke
    }

    char* ime_igrača=(char*)calloc(100,sizeof(char));
    printf("\nUnesite ime igraca: ");
    if (scanf("%s", ime_igrača) != 1) {
        prikazi_gresku("Neispravan unos");
        fclose(datoteka); // Zatvori datoteku prije nego što se vratiš
        return;
    }

    fprintf(datoteka, "%s %d\n", ime_igrača, rezultat);

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greska pri zatvaranju datoteke");
    }
    free(ime_igrača);
}


int usporedi_igrace(const void* a, const void* b) {
    const Igrac* igracA = (const Igrac*)a;
    const Igrac* igracB = (const Igrac*)b;
    return igracB->rezultat - igracA->rezultat;
}

inline int usporedi_imena(const void* a, const void* b) {
    return strcmp(((const Igrac*)a)->ime, ((const Igrac*)b)->ime);
}

void učitaj_rezultate(const char* ime) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne može biti NULL\n");
        return;
    }

    FILE* datoteka = fopen(ime, "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greska pri otvaranju datoteke");
    }

    // Koristimo fseek za pomicanje na početak datoteke
    if (fseek(datoteka, 0, SEEK_SET) != 0) {
        prikazi_gresku("Greska pri pomicanju na pocetak datoteke");
    }

    static Igrac igraci[MAX_IGRACI];
    static int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greska pri zatvaranju datoteke");
    }

    quicksort(igraci, brojIgraca, usporedi_igrace);

    printf("\nRezultati:\n");
    for (int i = 0; i < brojIgraca; i++) {
        printf("%d. %s: %d\n", i + 1, igraci[i].ime, igraci[i].rezultat);
    }
}

void pretraži_rezultate_po_imenu(const char* ime) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne moze biti NULL\n");
        return;
    }

    FILE* datoteka = fopen("rezultati.txt", "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greska pri otvaranju datoteke");
    }

    Igrac igrac;
    while (fscanf(datoteka, "%s %d", igrac.ime, &igrac.rezultat) != EOF) {
        if (strcmp(igrac.ime, ime) == 0) {
            printf("%s: %d\n", igrac.ime, igrac.rezultat);
        }
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greska pri zatvaranju datoteke");
    }
}



void kopiraj_datoteku(const char* izvor, const char* odredište) {
    FILE* src = fopen(izvor, "r");
    if (src == NULL) {
        prikazi_gresku("Greska pri otvaranju izvorne datoteke");
    }

    FILE* dst = fopen(odredište, "w");
    if (dst == NULL) {
        fclose(src);
        prikazi_gresku("Greska pri otvaranju odredišne datoteke");
    }

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    if (fclose(src) != 0 || fclose(dst) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }
}

void brisanje_rezultata(int id) {
    FILE* datoteka = fopen("rezultati.txt", "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    Igrac igraci[MAX_IGRACI];
    int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }

    if (id < 0 || id >= brojIgraca) {
        printf("Nevažeći ID\n");
        return;
    }

    FILE* temp_datoteka = fopen("rezultati_temp.txt", "w");
    if (temp_datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju privremene datoteke");
    }

    for (int i = 0; i < brojIgraca; i++) {
        if (i != id) {
            fprintf(temp_datoteka, "%s %d\n", igraci[i].ime, igraci[i].rezultat);
        }
    }

    if (fclose(temp_datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju privremene datoteke");
    }

    if (remove("rezultati.txt") != 0) {
        prikazi_gresku("Greška pri brisanju originalne datoteke");
    }

    if (rename("rezultati_temp.txt", "rezultati.txt") != 0) {
        prikazi_gresku("Greška pri preimenovanju privremene datoteke");
    }
}

void promjena_imena(int id, const char* novo_ime) {
    FILE* datoteka = fopen("rezultati.txt", "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    Igrac igraci[MAX_IGRACI];
    int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }

    if (id < 0 || id >= brojIgraca) {
        printf("Nevažeći ID\n");
        return;
    }

    strncpy(igraci[id].ime, novo_ime, MAX_IME);
    igraci[id].ime[MAX_IME - 1] = '\0'; // Osiguraj null-terminator

    datoteka = fopen("rezultati.txt", "w");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    for (int i = 0; i < brojIgraca; i++) {
        fprintf(datoteka, "%s %d\n", igraci[i].ime, igraci[i].rezultat);
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }
}

void učitaj_rezultate_sortirano(const char* ime, int po_rezultatu) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne može biti NULL\n");
        return;
    }

    FILE* datoteka = fopen(ime, "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    Igrac igraci[MAX_IGRACI];
    int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }

    if (po_rezultatu) {
        quicksort(igraci, brojIgraca, usporedi_igrace);
    }
    else {
        quicksort(igraci, brojIgraca, usporedi_imena);
    }

    printf("\nRezultati:\n");
    for (int i = 0; i < brojIgraca; i++) {
        printf("%d. %s: %d\n", i + 1, igraci[i].ime, igraci[i].rezultat);
    }
}



void učitaj_rezultate_s_id(const char* ime) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne može biti NULL\n");
        return;
    }

    FILE* datoteka = fopen(ime, "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    Igrac igraci[MAX_IGRACI];
    int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }

    printf("\nRezultati s ID-evima:\n");
    for (int i = 0; i < brojIgraca; i++) {
        printf("ID: %d - %s: %d\n", i, igraci[i].ime, igraci[i].rezultat);
    }
}



void pretraži_rezultate_po_imenu_bsearch(const char* ime, int (*usporedba)(const void*, const void*)) {
    if (ime == NULL) {
        fprintf(stderr, "Ime ne može biti NULL\n");
        return;
    }

    FILE* datoteka = fopen("rezultati.txt", "r");
    if (datoteka == NULL) {
        prikazi_gresku("Greška pri otvaranju datoteke");
    }

    static Igrac igraci[MAX_IGRACI];
    static int brojIgraca = 0;

    while (fscanf(datoteka, "%s %d", igraci[brojIgraca].ime, &igraci[brojIgraca].rezultat) != EOF && brojIgraca < MAX_IGRACI) {
        brojIgraca++;
    }

    if (fclose(datoteka) != 0) {
        prikazi_gresku("Greška pri zatvaranju datoteke");
    }

    quicksort(igraci, brojIgraca, usporedba);

    Igrac ključ;
    strncpy(ključ.ime, ime, MAX_IME);
    ključ.ime[MAX_IME - 1] = '\0';

    Igrac* rezultat = (Igrac*)bsearch(&ključ, igraci, brojIgraca, sizeof(Igrac), usporedba);

    if (rezultat != NULL) {
        int index = rezultat - igraci;
        printf("Pronađeni rezultati za ime %s:\n", ime);

        for (int i = index; i >= 0 && strcmp(igraci[i].ime, ime) == 0; i--) {
            printf("%s: %d\n", igraci[i].ime, igraci[i].rezultat);
        }
        for (int i = index + 1; i < brojIgraca && strcmp(igraci[i].ime, ime) == 0; i++) {
            printf("%s: %d\n", igraci[i].ime, igraci[i].rezultat);
        }
    }
    else {
        printf("Rezultat za ime %s nije pronađen.\n", ime);
    }
}

void quicksort(Igrac* igraci, int n, int (*usporedba)(const void*, const void*)) {
    if (n <= 1) return;

    Igrac pivot = igraci[n / 2];
    int i, j;
    for (i = 0, j = n - 1;; i++, j--) {
        while (usporedba(&igraci[i], &pivot) < 0) i++;
        while (usporedba(&igraci[j], &pivot) > 0) j--;

        if (i >= j) break;

        Igrac temp = igraci[i];
        igraci[i] = igraci[j];
        igraci[j] = temp;
    }

    quicksort(igraci, i, usporedba);
    quicksort(igraci + i, n - i, usporedba);
}

void brisanje_datoteke() {
    char* ime_datoteke = "rezultati.txt";
    
    int status =0;
    status = remove(ime_datoteke);
    if (status == 0) {
        printf("Uspjesno obrisana datoteka!\n");
    }
    else {
        printf("Nemogucnost brisanja datoteke!\n");
    }
}
