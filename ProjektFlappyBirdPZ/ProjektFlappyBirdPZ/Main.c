#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "header.h"

int main() {
    srand((unsigned int)time(NULL)); // Inicijalizacija generatora slučajnih brojeva

    IzbornikOpcije izbor;
    char ime[MAX_IME];
    int id, po_rezultatu;

    do {
        printf("\nIzbornik:\n");
        printf("%d - Ispis rezultata\n", ISPIS_REZULTATA);
        printf("%d - Ispis pravila igre\n", ISPIS_PRAVILA);
        printf("%d - Pokretanje igre\n", POKRETANJE_IGRE);
        printf("%d - Pretrazivanje rezultata po imenu\n", PRETRAZIVANJE_REZULTATA);
        printf("%d - Brisanje rezultata po ID-u\n", BRISANJE_REZULTATA);
        printf("%d - Promjena imena po ID-u\n", PROMJENA_IMENA);
        printf("%d - Brisanje datoteke.\n", BRISANJE_DATOTEKE);
        printf("%d - Gasenje programa.\n", GASI_PROGRAM);
        printf("Odaberite opciju: ");

        int temp_izbor;
        if (scanf("%d", &temp_izbor) != 1) {
            prikazi_gresku("Neispravan unos");
        }
        izbor = (IzbornikOpcije)temp_izbor;

        switch (izbor) {
        case ISPIS_PRAVILA:
            printf("\nPravila igre:\n");
            printf("Igra se pokrece pritiskom na slovo 's'.\n");
            printf("Ptica skace pritiskom na razmaknicu(space).\n");
            printf("Cilj igre je izbjeci sto je vise prepreka moguce.\n");
            break;
        case POKRETANJE_IGRE:
            postavi();
            while (1) {
                ažuriraj();
                crtanje();
                if (stanjeIgre == IGRA_ZAVRŠENA) {
                    printf("\nKraj igre!\n");
                    zapis_u_datoteku("rezultati.txt", brojačProđenihPrepreka);
                    break;
                }
            }
            break;
        case PRETRAZIVANJE_REZULTATA:
            printf("\nUnesite ime igraca: ");
            if (scanf("%s", ime) != 1) {
                prikazi_gresku("Neispravan unos");
            }
            pretraži_rezultate_po_imenu_bsearch(ime, usporedi_imena);
            break;
        case ISPIS_REZULTATA:
            printf("\nSortirati rezultate po:\n1 - Rezultat (QuickSort)\n2 - Ime (MergeSort)\n3 - Ispis rezultata s ID-evima\nOdaberite opciju: ");
            if (scanf("%d", &po_rezultatu) != 1) {
                prikazi_gresku("Neispravan unos");
            }
            if (po_rezultatu == 3) {
                učitaj_rezultate_s_id("rezultati.txt");
            }
            else if (po_rezultatu == 1) {
                učitaj_rezultate_sortirano("rezultati.txt", 1);
            }
            else if (po_rezultatu == 2) {
                učitaj_rezultate_sortirano("rezultati.txt", 0);
            }
            break;
        case BRISANJE_REZULTATA:
            printf("\nUnesite ID rezultata za brisanje: ");
            if (scanf("%d", &id) != 1) {
                prikazi_gresku("Neispravan unos");
            }
            brisanje_rezultata(id);
            break;
        case PROMJENA_IMENA:
            printf("\nUnesite ID rezultata za promjenu imena: ");
            if (scanf("%d", &id) != 1) {
                prikazi_gresku("Neispravan unos");
            }
            printf("Unesite novo ime: ");
            if (scanf("%s", ime) != 1) {
                prikazi_gresku("Neispravan unos");
            }
            promjena_imena(id, ime);
            break;
        case BRISANJE_DATOTEKE:
            brisanje_datoteke();
            break;
        case GASI_PROGRAM:
            exit(0);
            break;
        default:
            printf("\nNepostojeci izbor!\n");
        }
    } while (1);

    return 0;
}