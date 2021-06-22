#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

//Definicja slowa
typedef struct slowo
{
    wint_t s;
    struct slowo* nastepny;
}slowo;
//Definicja slownika
typedef struct slownik
{
    slowo* jezyka;
    slowo* jezykb;
    struct slownik* nastepny;
}slownik;


/*
void Wypisz_slowo(slowo* pierwszy)
{
    slowo* tymczasowy= pierwszy;
    while(tymczasowy!=NULL)
    {
        wprintf(L"%lc",tymczasowy->s);
        tymczasowy=tymczasowy->nastepny;
    }

}
void Wypisz_slownik(slownik* glowa)
{
    slownik* tymczasowy =glowa;
    while(tymczasowy!=NULL)
    {
        Wypisz_slowo(tymczasowy->jezyka);
        printf("\t");
        Wypisz_slowo(tymczasowy->jezykb);
        printf("\n");
        tymczasowy=tymczasowy->nastepny;
    }
}
*/


//-----------------------Funkcje do wiersza polecen-------------

//Funkcja sprawdzająca przelaczniki dla s i o
char* Sprawdz_poprawnosc_komendy(char stan,int lim, char* polecenia [])
{
    char* tymczasowy=NULL;
    for(int i=1;i<lim;i=i+2)
    {
      if(polecenia[i][1]==stan)
        {
            tymczasowy=malloc(sizeof(polecenia[i+1]));
            if(tymczasowy==NULL)
            {
                printf("Nie udalo sie zaalokowac pamieci programu.\n");
                exit(0);
            }
            strcpy(tymczasowy,polecenia[i+1]);
            if(strlen(tymczasowy)>4 && ( tymczasowy[strlen(tymczasowy)-1] == 't' && tymczasowy[strlen(tymczasowy)-2] == 'x' && tymczasowy[strlen(tymczasowy)-3] == 't' && tymczasowy[strlen(tymczasowy)-4] =='.'))
                return tymczasowy;
            else
            {
                printf("Podany plik nie jest plikiem tekstowym lub jest niepoprawny %s.\n",tymczasowy);
                exit(0);
            }
        }
    }
        printf("Wpisana komenda, nie zawiera przelacznika -%c.\n",stan);
        exit(0);
        return NULL;
};


//Funkcja sprawdzająca kierunek tłumaczenia
bool Sprawdz_kierunek(int lim,char* polecenia[])
{
    for(int i=1;i<lim;i=i+2)
    {
       if(polecenia[i][1]=='k')
        {
            if(strcmp(polecenia[i+1],"L2-L1")==0)
                return true;
            else if(strcmp(polecenia[i+1],"L1-L2")==0)
                return false;
        }
    }
    printf("Wpisana komenda (kierunek tlumaczenia), badz przelacznik sa niepoprawne. Poprawny kierunek to L1-L2, lub L2-L1.\n");
    exit(0);
};
//------------------------Dynamiczne_struktury-------------------------

//Funkcje obsługujace strukture slowo

//Funkcja tworząca nowy element typu slowo
slowo * Stworz_literke(wint_t znak)
{
    slowo *nowe=malloc(sizeof(slowo));
    if(nowe==NULL)
    {
        printf("Nie udalo sie zaalokowac pamieci programu.\n");
        exit(0);
    }
    nowe->s=znak;
    nowe->nastepny=NULL;
    return nowe;
}


//Funkcja inicjalizująca listę jednokierunkową krok po kroku (dodając podany znak na jej koniec)
slowo* Zbuduj_slowo(slowo* glowa,wint_t znak)
{
    slowo* szukacz=glowa;
    if(glowa==NULL)
    {
        szukacz=Stworz_literke(znak);
        return szukacz;
    }
    while(szukacz->nastepny!=NULL)
    {
        szukacz=szukacz->nastepny;
    }
    slowo* nowy=Stworz_literke(znak);
    szukacz->nastepny=nowy;
    return glowa;
};


//Funkcja sprawdzająca czy znaki podanych list typu slowo są takie same
bool Porownaj(slowo* pierwsze,slowo* drugie)
{
    slowo * tymczasowya=pierwsze;
    slowo * tymczasowyb=drugie;
    while(tymczasowya!=NULL)
    {
        if(tymczasowyb==NULL)
            return false;
        if( (tymczasowya->s  ==  tymczasowyb->s) ||  (tymczasowya->s-32 == tymczasowyb->s ) ||  (tymczasowya->s == tymczasowyb->s-32)   )
        {
            tymczasowya=tymczasowya->nastepny;
            tymczasowyb=tymczasowyb->nastepny;
        }
        else
            return false;
    }
    if(tymczasowyb!=NULL)
        return false;
    return true;
}


//Usuwanie słowa rekurencyjnie
slowo* Usun_slowo(slowo* glowa)
{
   slowo* przeszukiwacz = glowa;
   if(przeszukiwacz->nastepny!=NULL)
   {
       Usun_slowo(przeszukiwacz->nastepny);
   }
   free(przeszukiwacz);
   return NULL;
}


//Funkcja znajdująca daną listę typu slowo, podwieszaną pod listę typu slownik
slowo* Znajdz_slowo(slownik* glowa,slowo* slowo_s)
{
    slownik* przeszukiwacz=glowa;
    while(przeszukiwacz!=NULL)
    {
        if(Porownaj(przeszukiwacz->jezyka,slowo_s)==true)
        {
            return przeszukiwacz->jezykb;
        }
        przeszukiwacz=przeszukiwacz->nastepny;
    }
    return NULL;
}


//Funkcje operujace na strukturze slownik

//Funkcja tworząca nowy element typu slownik
struct slownik* Stworz_nowy_el_slownika(bool rev,slowo * a,slowo * b)
{
    struct slownik* tymczasowy= malloc(sizeof(slownik));
    if(tymczasowy!=NULL)
    {
        if(a==NULL || b==NULL)
        {
            printf("Plik slownika zawiera niepoprawne dane.\n");
            exit(0);
        }
        if(rev)
        {
            tymczasowy->jezyka=b;
            tymczasowy->jezykb=a;
        }
        else
        {
            tymczasowy->jezykb=b;
            tymczasowy->jezyka=a;
        }
        tymczasowy->nastepny=NULL;
    }
    else{
        printf("Nie udalo sie zaalokowac pamieci programu.\n");
        exit(0);
    }
    return tymczasowy;
};


//Funkcja która tworzy listę z podanego elementu, jeśli lista już istnieje nowy element dodawany jest na jej koniec
slownik * Dodaj_do_listy(slownik* glowa,slownik* do_dodania)
{
    if(glowa==NULL)
    {
        return do_dodania;
    }
    else
    {
        slownik * przeszukiwacz=glowa;
        while(przeszukiwacz->nastepny!=NULL)
        {
            przeszukiwacz=przeszukiwacz->nastepny;
        }
        przeszukiwacz->nastepny=do_dodania;
        return glowa;
    }
}


//Główna funkcja wczytująca dane z pliku i przekazująca wczytane dane do odpowiednich funkcji i zwracająca listę jednokierunkową typu slownik
slownik * Wczytaj_slownik(char nazwa[],bool rev)
{
    FILE *plik= fopen(nazwa,"r, ccs=UTF-8");
    slownik* glowa=NULL;
    if(plik!=NULL)
    {
        slowo * jezyka=NULL;
        slowo * jezykb=NULL;
        wint_t znak;
        bool spacja= false;
        while((znak = fgetwc(plik)) != WEOF)
        {
            if( ( znak>64 && znak<91 )  ||  ( znak>96 && znak<123 ) || znak>191)
            {
                if(spacja)
                    jezykb= Zbuduj_slowo(jezykb,znak);
                else
                    jezyka= Zbuduj_slowo(jezyka,znak);
            }
            else if(znak==32 && spacja==false)
                spacja=true;
            else if(znak==10 && spacja==true)
            {
                slownik* nowy=Stworz_nowy_el_slownika(rev,jezyka,jezykb);
                glowa=Dodaj_do_listy(glowa,nowy);
                jezyka=NULL;
                jezykb=NULL;
                spacja=false;
            }
            else
            {
               printf("Plik slownika zawiera niepoprawne dane.\n");
               exit(0);
            }
        }
        fclose( plik );
        return glowa;
    }
    else
    {
       printf("Nie udalo sie otworzyc pliku slownika.\n");
       exit(0);
    }
};


//Funkcja zapisujaca do otwartego pliku odpowiednie dane
void Zapisz_do_pliku(FILE * fwyjsciowy, slowo* do_przep,slowo* element)
{
    if(element!=NULL)
    {
        slowo* przeszukiwacz=element;
        while(przeszukiwacz!=NULL)
        {
            fwprintf(fwyjsciowy,L"%lc",przeszukiwacz->s);
            przeszukiwacz=przeszukiwacz->nastepny;
        }
    }
    else
    {
        slowo* przeszukiwacz=do_przep;
        fwprintf(fwyjsciowy,L"[");
        while(przeszukiwacz!=NULL)
        {
            fwprintf(fwyjsciowy,L"%lc",przeszukiwacz->s);
            przeszukiwacz=przeszukiwacz->nastepny;
        }
        fwprintf(fwyjsciowy,L"]");
    }
}


//Druga z głownych funkcji która służy do odczytania pliku wejściowego, przetłumaczeniu go z wykorzystaniem funkcji i wpisaniu wyniku do pliku wyjściowego rowniez z uzyciem funkcji.
void Przetlumacz(slownik* glowa,char we[],char wy[])
{
    FILE *fwejsciowy=fopen(we,"r, ccs=UTF-8");
    FILE *fwyjsciowy=fopen(wy,"w, ccs=UTF-8");
    if(fwejsciowy!=NULL && fwyjsciowy!=NULL)
    {
        wint_t znak;
        slowo* nowy=NULL;
        while((znak = fgetwc(fwejsciowy)) != WEOF)
        {
            if( ( znak>64 && znak<91 )  ||  ( znak>96 && znak<123 ) || znak>160)
                    nowy=Zbuduj_slowo(nowy,znak);
            else
            {
                if(nowy!=NULL)
                {
                    slowo* tymczasowy=Znajdz_slowo(glowa,nowy);
                    Zapisz_do_pliku(fwyjsciowy,nowy,tymczasowy);
                    nowy=Usun_slowo(nowy);
                }
                    fwprintf(fwyjsciowy,L"%lc",znak);
            }
        }
        if(nowy!=NULL)
        {
            slowo* tymczasowy=Znajdz_slowo(glowa,nowy);
            Zapisz_do_pliku(fwyjsciowy,nowy,tymczasowy);
            nowy=Usun_slowo(nowy);
            free(nowy);
        }
    }
    else
    {
        printf("Nie udalo otworzyc sie pliku: %s\n",fwejsciowy==NULL? we : wy);
        exit(0);
    }
    fclose(fwejsciowy);
    fclose(fwyjsciowy);
}


//Funkcja służąca do usunięcia listy typu slownik i zawartych w niej list typu slowo
void Usun_liste(slownik ** glowa)
{
    slownik* przeszukiwacz=*glowa;
    slownik* nast;
    while(przeszukiwacz!=NULL)
    {
        nast=przeszukiwacz->nastepny;
        przeszukiwacz->jezyka=Usun_slowo(przeszukiwacz->jezyka);
        przeszukiwacz->jezykb=Usun_slowo(przeszukiwacz->jezykb);
        free(przeszukiwacz);
        przeszukiwacz=nast;
    }
    *glowa=NULL;
}
