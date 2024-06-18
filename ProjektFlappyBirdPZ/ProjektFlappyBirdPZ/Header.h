#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define VISINA 20
#define ŠIRINA 40
#define BRZINA_PTICE 100
#define MIN_RAZMAK 9
#define MAX_IME 50
#define MAX_IGRACI 100

typedef enum {
    ČEKA_POČETAK = 1,
    IGRA_POKRENUTA = 2,
    IGRA_ZAVRŠENA = 0
} StanjeIgre;

typedef enum {
    ISPIS_REZULTATA = 0,
    ISPIS_PRAVILA = 1,
    POKRETANJE_IGRE = 2,
    PRETRAZIVANJE_REZULTATA = 3,
    BRISANJE_REZULTATA = 4,
    PROMJENA_IMENA = 5,
    BRISANJE_DATOTEKE = 6,
    GASI_PROGRAM = 9
} IzbornikOpcije;

typedef struct {
    int x, y;
} Prepreka;

typedef struct {
    char ime[MAX_IME];
    int rezultat;
} Igrac;

extern int pticaY, brzinaPtice;
extern StanjeIgre stanjeIgre;
extern int brojačProđenihPrepreka;
extern char ekran[VISINA][ŠIRINA];
extern Prepreka preprekaGore, preprekaDolje, preprekaGore2, preprekaDolje2;

void prikazi_gresku(const char* msg);
void postavi(void);
void crtanje(void);
void ažuriraj(void);
void zapis_u_datoteku(const char* ime, int rezultat);
void učitaj_rezultate(const char* ime);
void pretraži_rezultate_po_imenu(const char* ime);
void pretraži_rezultate_po_imenu_bsearch(const char* ime, int (*usporedba)(const void*, const void*));
void brisanje_rezultata(int id);
void promjena_imena(int id, const char* novo_ime);
void učitaj_rezultate_sortirano(const char* ime, int po_rezultatu);
void učitaj_rezultate_s_id(const char* ime);
void quicksort(Igrac* igraci, int n, int (*usporedba)(const void*, const void*));
void mergesort(Igrac* igraci, int n, int (*usporedba)(const void*, const void*));
Igrac* binary_search(Igrac* igraci, int n, const char* ime, int (*usporedba)(const void*, const void*));
int usporedi_igrace(const void* a, const void* b);
int usporedi_imena(const void* a, const void* b);
void kopiraj_datoteku(const char* izvor, const char* odredište);
void brisanje_datoteke();

#endif // HEADER_H_INCLUDED
