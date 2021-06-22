#include "funkcje.h"



int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"");
    struct slownik* glowa=NULL;
    char *nazwa_slownika=Sprawdz_poprawnosc_komendy('s',argc,argv);
    char *nazwa_we=Sprawdz_poprawnosc_komendy('i',argc,argv);
    char *nazwa_wy=Sprawdz_poprawnosc_komendy('o',argc,argv);
    bool odwrotne=Sprawdz_kierunek(argc,argv);
    glowa=Wczytaj_slownik(nazwa_slownika,odwrotne);
    Przetlumacz(glowa,nazwa_we,nazwa_wy);
    Usun_liste(&glowa);
    free(nazwa_slownika);
    free(nazwa_we);
    free(nazwa_wy);
    printf("Pomyslnie przetlumaczono plik wejsciowy!\n");
    return 0;
};
