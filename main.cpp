#include <iostream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <math.h>
#include <time.h>
#include <windows.h>

#define T_KONC 0.01

using namespace std;

inline void swap(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
};

void wypiszElementy(int **tablicaZadan, int iloscZadan, int iloscMaszyn)
{
    cout << "******************" << endl;
    for (int i = 0; i < iloscZadan; i++)
    {
        for (int j = 0; j < iloscMaszyn; j++)
        {
            cout << tablicaZadan[i][j] << " ";
        }
        cout << endl;
    }
    cout << "******************" << endl;
}

int obliczCMax(int **tablicaTymczasowa, int iloscZadan, int iloscMaszyn)
{
    int **harmonogram = new int *[iloscZadan];
    for(int i = 0; i < iloscZadan; i++)
        harmonogram[i] = new int[iloscMaszyn];

    int suma = 0;

    for (int i = 0; i < iloscZadan; i++)
        for (int j = 0; j < iloscMaszyn; j++)
        {
            if (i == 0 && j == 0)
            {
                harmonogram[i][j] = tablicaTymczasowa[i][j];
                suma += harmonogram[i][j];
            }
            else if (i == 0 && j != 0)
            {
                harmonogram[i][j] = harmonogram[i][j-1] + tablicaTymczasowa[i][j];
                suma += harmonogram[i][j];
            }
            else if (i != 0 && j == 0)
            {
                harmonogram[i][j] = harmonogram[i-1][j] + tablicaTymczasowa[i][j];
                suma += harmonogram[i][j];
            }
            else
            {
                harmonogram[i][j] = max(harmonogram[i][j-1], harmonogram[i-1][j]) + tablicaTymczasowa[i][j];
                suma += harmonogram[i][j];
            }
        }
        int cMax = harmonogram[iloscZadan-1][iloscMaszyn-1];
        delete harmonogram;
        return cMax;
}

/** Wersja podstawowa **/

double czyZamienicKolejnosc (int aktualny, int nowy, double temperatura)
{
    if (nowy < aktualny)
    {
        return 1;
        //return exp((aktualny - nowy)/temperatura);
    };

    if (nowy >= aktualny)
    {
        return exp((aktualny - nowy)/temperatura);
    }
}

/** Wersja inna

double czyZamienicKolejnosc (int aktualny, int nowy, double temperatura)
{
    if (nowy == aktualny)
    {
        return 0;
    };

    if (nowy != aktualny)
    {
        //return exp((aktualny - nowy)/temperatura);
        return 1;
    }
}

**/

int symulowaneWyzarzanie(int **tablicaZadan, double temperatura, int iloscZadan, int iloscMaszyn)
{
    // Krok 1 - Inicjalizacja

    double mi = 0.99;

    int **tablicaPodstawowa = new int *[iloscZadan];
    for (int k = 0; k < iloscZadan; k++)
        tablicaPodstawowa[k] = new int[iloscMaszyn];

    int **tablicaZamieniona = new int *[iloscZadan];
    for (int k = 0; k < iloscZadan; k++)
        tablicaZamieniona[k] = new int[iloscMaszyn];

    for (int i = 0; i < iloscZadan; i++)
        for (int j = 0; j < iloscMaszyn; j++)
                tablicaPodstawowa[i][j] = tablicaZadan[i][j];

    for (int i = 0; i < iloscZadan; i++)
        for (int j = 0; j < iloscMaszyn; j++)
                tablicaZamieniona[i][j] = tablicaZadan[i][j];

    // Krok 2 - generowanie ruchu

    int n1, n2;
    int wynikKoncowy = 0;
    srand(time(NULL));

    while (temperatura > T_KONC)
    {
        //Sleep(100);

        int cmax1 = obliczCMax(tablicaPodstawowa, iloscZadan, iloscMaszyn);
        ////cout << "cmax1 = " << cmax1 << endl;

        n1 = (rand() % iloscZadan );
        n2 = (rand() % iloscZadan );

        while (n1 == n2)
        {
            n2 = (rand() % iloscZadan );
        }

        swap(tablicaZamieniona[n1],tablicaZamieniona[n2]);

        int cmax2 = obliczCMax(tablicaZamieniona, iloscZadan, iloscMaszyn);
        ////cout << "cmax2 = " << cmax2 << endl;

        // Krok 3 - Decyzja: wykonanie

        double pp = czyZamienicKolejnosc(cmax1, cmax2, temperatura);

        double sprawdzaniePrawdopodobienstwa = (rand() % 101); // generuje losowy numer miedzy 0 a 100
        sprawdzaniePrawdopodobienstwa = sprawdzaniePrawdopodobienstwa/100; // dzieli go przez 100 aby uzyskac zakres <0,1>

        if (pp < sprawdzaniePrawdopodobienstwa)
        {
            for (int i = 0; i < iloscZadan; i++)
                for (int j = 0; j < iloscMaszyn; j++)
                    tablicaZamieniona[i][j] = tablicaPodstawowa[i][j];

            temperatura = temperatura*mi;
            ///cout << "Prawdop. zwraca: " << pp << " przeciwko " << sprawdzaniePrawdopodobienstwa << endl;
            ///cout << "Aktualna temperatura: " << temperatura << endl;
            ///cout << "Nie zamieniam tablic." << endl;
            wynikKoncowy = 0;
        }
        else
        {
            for (int i = 0; i < iloscZadan; i++)
                for (int j = 0; j < iloscMaszyn; j++)
                    tablicaPodstawowa[i][j] = tablicaZamieniona[i][j];

            temperatura = temperatura*mi;
            ///cout << "Prawdop. zwraca: " << pp << " przeciwko " << sprawdzaniePrawdopodobienstwa << endl;
            //cout << "Aktualna temperatura: " << temperatura << endl;
           ///cout << "Zamieniam tablice." << endl;
            wynikKoncowy = 1;
        }

        //wypiszElementy(tablicaPodstawowa, iloscZadan, iloscMaszyn);
        //wypiszElementy(tablicaZamieniona, iloscZadan, iloscMaszyn);
        //system("pause");
    }

    int cMaxKoncowe;

    if (wynikKoncowy == 0)
        cMaxKoncowe = obliczCMax(tablicaPodstawowa, iloscZadan, iloscMaszyn);
    if (wynikKoncowy == 1)
        cMaxKoncowe = obliczCMax(tablicaZamieniona, iloscZadan, iloscMaszyn);

    cout << "Cmax = " << cMaxKoncowe << endl;
    fstream output;
    output.open("outputData.txt", ios::out|ios::app);
    output << cMaxKoncowe << endl;
    output.close();

    delete tablicaPodstawowa;
    delete tablicaZamieniona;
}

int obliczNEH(int **tablicaKolejnosci, int iloscZadan, int iloscMaszyn)
{
    int czasAktualny = 0;
    int czasMinimalny = 99999;

    int **tablicaTymczasowa = new int *[iloscZadan];
    for (int k = 0; k < iloscZadan; k++)
        tablicaTymczasowa[k] = new int[iloscMaszyn];

    for (int i = 0; i < iloscZadan; i++)
        for (int j = 0; j < iloscMaszyn; j++)
            tablicaTymczasowa[i][j] = tablicaKolejnosci[i][j];

    int **harmonogram = new int *[iloscZadan];
    for(int i = 0; i < iloscZadan; i++)
        harmonogram[i] = new int[iloscMaszyn];

    if (iloscZadan == 1)
    {
        for (int j = 0; j < iloscMaszyn; j++)
        {
            harmonogram[0][j] = tablicaTymczasowa[0][j];
            czasAktualny += harmonogram[0][j];
            czasMinimalny = czasAktualny;
        }

        for (int x = 0; x < iloscZadan; x++)
            for (int z = 0; z < iloscMaszyn; z++)
                tablicaKolejnosci[x][z] = tablicaTymczasowa[x][z];
    }
    else
    {
        for (int i = 1; i < iloscZadan+1; i++)
        {
            if  (i != iloscZadan)
            {
                for (int n = 0; n < iloscZadan; n++)
                    for (int m = 0; m < iloscMaszyn; m++)
                    {
                        czasAktualny = 0;
                        if (n == 0 && m == 0)
                        {
                            harmonogram[n][m] = tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else if (n == 0 && m != 0)
                        {
                            harmonogram[n][m] = harmonogram[n][m-1] + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else if (n != 0 && m == 0)
                        {
                            harmonogram[n][m] = harmonogram[n-1][m] + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else
                        {
                            harmonogram[n][m] = max(harmonogram[n][m-1], harmonogram[n-1][m]) + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                    }

                if (czasAktualny <= czasMinimalny)
                {
                    czasMinimalny = czasAktualny;

                    for (int x = 0; x < iloscZadan; x++)
                        for (int z = 0; z < iloscMaszyn; z++)
                            tablicaKolejnosci[x][z] = tablicaTymczasowa[x][z];
                }

                swap(tablicaTymczasowa[iloscZadan-i-1],tablicaTymczasowa[iloscZadan-i]);

            }
            else
            {
                for (int n = 0; n < iloscZadan; n++)
                    for (int m = 0; m < iloscMaszyn; m++)
                    {
                        czasAktualny = 0;
                        if (n == 0 && m == 0)
                        {
                            harmonogram[n][m] = tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else if (n == 0 && m != 0)
                        {
                            harmonogram[n][m] = harmonogram[n][m-1] + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else if (n != 0 && m == 0)
                        {
                            harmonogram[n][m] = harmonogram[n-1][m] + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                        else
                        {
                            harmonogram[n][m] = max(harmonogram[n][m-1], harmonogram[n-1][m]) + tablicaTymczasowa[n][m];
                            czasAktualny += harmonogram[n][m];
                        }
                    }

                if (czasAktualny <= czasMinimalny)
                {
                    czasMinimalny = czasAktualny;

                    for (int x = 0; x < iloscZadan; x++)
                        for (int z = 0; z < iloscMaszyn; z++)
                            tablicaKolejnosci[x][z] = tablicaTymczasowa[x][z];
                }
            }
        }
    }
    delete harmonogram;
    return czasMinimalny;
}

int algorytmNEH(int **tablicaZadan, int iloscZadan, int iloscMaszyn)
{
    int wagaAktualna = 0;
    int indeksMaksymalnego = 0;
    int wagaMaksymalna = 0;

    int Cmax = 0;

    int **tablicaKolejnosci = new int *[iloscZadan];
    for (int k = 0; k < iloscZadan; k++)
        tablicaKolejnosci[k] = new int[iloscMaszyn];

    for (int k = 0; k < iloscZadan; k++)
    {
        wagaMaksymalna = 0;
        for (int i = 0; i < iloscZadan; i++)
        {
            wagaAktualna = 0;

            for (int j = 0; j < iloscMaszyn; j++)
            {
                wagaAktualna += tablicaZadan[i][j];
            }
            if (wagaAktualna > wagaMaksymalna)
            {
                wagaMaksymalna = wagaAktualna;
                indeksMaksymalnego = i;
            }
        }

        for (int j = 0; j < iloscMaszyn; j++)
            tablicaKolejnosci[k][j] = tablicaZadan[indeksMaksymalnego][j];

        for (int j = 0; j < iloscMaszyn; j++)
            tablicaZadan[indeksMaksymalnego][j] = 0;

        int czas = obliczNEH(tablicaKolejnosci, k+1, iloscMaszyn);

        Cmax = czas;

    }
    cout << "Cmax = " << Cmax << endl;

    for (int i = 0; i < 10; i++)
    {
        symulowaneWyzarzanie(tablicaKolejnosci, 30, iloscZadan, iloscMaszyn);
        Sleep(1000);
    }
}
int main()
{
    int **tablicaZadan, iloscZadan, iloscMaszyn;

    double temperatura = 30;
    int ileBadan = 10;

    fstream input;
    input.open("inputData.txt");
    if(input)
    {
        input >> iloscZadan;
        input >> iloscMaszyn;

        tablicaZadan = new int *[iloscZadan];
        for(int i = 0; i < iloscZadan; i++)
            tablicaZadan[i] = new int[iloscMaszyn];

        for (int i = 0; i < iloscZadan; i++)
            for (int j = 0; j < iloscMaszyn; j++)
                input >> tablicaZadan[i][j];
    }
    input.close();
    cout << "Zadania: " << iloscZadan << ", Maszyny: " << iloscMaszyn << endl << endl;

    algorytmNEH(tablicaZadan, iloscZadan, iloscMaszyn);
/**
    for (int i = 0; i < ileBadan; i++)
    {
        symulowaneWyzarzanie(tablicaZadan, temperatura, iloscZadan, iloscMaszyn);
        Sleep(1000);
    }
**/
    delete tablicaZadan;
    system("pause");
}
