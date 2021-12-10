#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <windows.h>
#include "ai.h"

using namespace std;

int dimenzije = 15;
bool dead;
int dir;
int score;
int moves;
pair<int, int> directions[4] = {
    make_pair(1, 0), // desno
    make_pair(0, 1), // dole
    make_pair(-1, 0),// levo
    make_pair(0, -1) // gore
};
vector<pair<int, int>> zmija; /// zmija je velicine 4 i pocinje levo na sredini
pair<int, int> nestaliRep;
pair<int, int> jabuka;

void input_x(values* v);

void init_game()
{
    zmija = {
        make_pair(4, dimenzije/2 + 1),
        make_pair(3, dimenzije/2 + 1),
        make_pair(2, dimenzije/2 + 1),
        make_pair(1, dimenzije/2 + 1),
    };
    dead = false;
    dir = 0;
    score = 0;
    nestaliRep = make_pair(-1, -1);
    jabuka = make_pair(dimenzije - 3, dimenzije/2 + 1);
    moves = 0;
}

void sleep_funkcija(int milliseconds)
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end);
}

void printSomething(int s)
{
    COORD c;
    c.X = 0;
    c.Y = 17;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout << s << endl;
}

void nacrtaj()
{
    for(int i = 0; i < dimenzije+2; i++)
    {
        for(int j = 0; j < dimenzije+2; j++)
        {
            if(i==0 || i==dimenzije+1 || j==0 || j==dimenzije+1)
                cout << "##";
            else
                cout << "  ";
        }
        cout << endl;
    }
    COORD c;
    pair<int, int> temp;
    for(int i = 0; i < (int)zmija.size(); i++)
    {
        c.X = zmija[i].first * 2;
        c.Y = zmija[i].second;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
        if(i == 0)
            cout  << "&&";
        else
            cout  << "<>";
    }

    c.X = jabuka.first * 2;
    c.Y = jabuka.second;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout << "()";
}

void input(bool ai, genes* g, values* v)
{
    if(ai == true)
    {
        input_x(v);
        int rez = feed_forward(g, v); // 1 - levo, 2 - pravo, 3 - desno
        if(rez == 1)
        {
            dir++;
            dir = dir % 4;
        }
        else if(rez == 3)
        {
            dir += 3;
            dir = dir % 4;
        }
        return;
    }


    if (_kbhit())
	{
		switch (_getch())
		{
			case 'w': dir = (dir==1)?1:3; break;
			case 'd': dir = (dir==2)?2:0; break;
			case 's': dir = (dir==3)?3:1; break;
			case 'a': dir = (dir==0)?0:2; break;
		}
	}

}

bool contains(pair<int, int> c)
{
    for(int i = 1; i < (int)zmija.size(); i++)
    {
        if(zmija[i] == c)
            return true;
    }
    return false;
}

void pomeri(bool crtajJabuku)
{
    moves++;
    pair<int, int> glava = zmija.front();
    glava.first = glava.first + directions[dir].first;
    glava.second = glava.second + directions[dir].second;
    zmija.insert(zmija.begin(), glava);

    nestaliRep = zmija.back();
    if(glava.first == 0 || glava.first == dimenzije + 1 || glava.second == 0 || glava.second == dimenzije + 1)
    {
        dead = true;
    }
    else if(zmija[0].first != jabuka.first || zmija[0].second != jabuka.second)
    {
        zmija.pop_back();
    }
    else
    {
        do
        {
            jabuka.first = rand() % dimenzije + 1;
            jabuka.second = rand() % dimenzije + 1;
        } while(find(zmija.begin(), zmija.end(), jabuka) != zmija.end());

        if(crtajJabuku)
        {
            COORD c;
            c.X = jabuka.first * 2;
            c.Y = jabuka.second;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
            cout << "()";

            c.X = 0;
            c.Y = 17;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
            cout << "Score: " << ++score;
        }
    }
    if(contains(glava))
    {
        dead = true;
        return;
    }
}

void nacrtaj_zmiju()
{
    pair<int, int> temp;

    COORD c;

    c.X = nestaliRep.first * 2;
    c.Y = nestaliRep.second;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout  << "  ";

    c.X = zmija[0].first * 2;
    c.Y = zmija[0].second;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout  << "&&";

    c.X = zmija[1].first * 2;
    c.Y = zmija[1].second;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout  << "<>";

    c.X = 0;
    c.Y = 18;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

///__________________________________________________________________________
/// Genetic algorithm
#define BROJ_JEDINKI 1000

int brGeneracija = 100, brJedinki = BROJ_JEDINKI;
int scores[BROJ_JEDINKI], scoreSum;
values v[BROJ_JEDINKI];
genes gCurr[BROJ_JEDINKI];
genes gPrev[BROJ_JEDINKI];
int najboljaJedinka = 0;

void mutacija(int gIndex)
{
    int r = randomf(0, scoreSum);
    int index = 0;
    while(scoreSum > 0)
    {
        r -= scores[index];
        if(scoreSum < 0) scoreSum = 0;
    }
    mutate_values(&gCurr[gIndex], gPrev[index]);
}

bool prepreka(pair<int, int> glava)
{
    if(glava.first == 0 || glava.first == dimenzije + 1 || glava.second == 0 || glava.second == dimenzije + 1)
        return true;

    for(int i = 0; i < (int)zmija.size(); i++)
        if(glava == zmija[i])
            return true;

    return false;
}

int get_x(pair<int, int> koordinate)
{
    if(koordinate == jabuka)
        return 2;
    if(prepreka(koordinate))
        return -1;

    return 0;
}

void input_x(values* v)
{
    pair<int, int> smer = make_pair(1, 0);
    pair<int, int> glava = zmija[0];
    pair<int, int> temp = glava;
    for(int i = 0; i < 8; i++)
    {
        temp = glava;
        do
        {
            temp.first += smer.first;
            temp.second += smer.second;
        } while(get_x(temp) == 0 && i != 2);
    }
    v->x[2] = 0;
}

void ucenje()
{
    int minScore;
    for(int i = 0; i < brGeneracija; i++)
    {
        scoreSum = 0;
        cout << i << endl;
        for(int j = 0; j < brJedinki; j++)
        {
            if(i == 0) init_values(&gCurr[j], &v[j]);
            else mutacija(j);
            init_game();
            while(dead == false && moves < 1000)
            {
                input_x(&v[j]);
                input(true, &gCurr[j], &v[j]);
                pomeri(false);
            }
            scores[j] = (score+1)*(score+1) - moves;
            if(moves == 1000) scores[i] -= 10000;
            if(scores[i] < minScore) minScore = scores[i];
        }
        for(int j = 0; j < brJedinki; j++)
        {
            if(minScore <= 0) scores[i] -= minScore - 1;
            gPrev[i] = gCurr[i];
        }
    }
    for(int i = 0; i < brJedinki; i++)
    {
        if(scores[i] > scores[najboljaJedinka])
            najboljaJedinka = i;
    }
}

///__________________________________________________________________________

void normal_game();
void repeating_game();

int main()
{
    srand((unsigned)time(0));

    ucenje();
    repeating_game();

	return 0;
}

void repeating_game()
{
    COORD c;
    while(true)
    {
        normal_game();
        c.X = 0;
        c.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
        for(int i = 0; i < dimenzije + 2; i++)
        {
            for(int j = 0; j < dimenzije * 2 + 4; j++)
            {
                cout << " ";
            }
            cout << endl;
        }

        c.X = 0;
        c.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
        cout << "space for play again and anything else for exit";
        if(_getch() != ' ')
        {
            system("cls");
            break;
        }
    }
}

void normal_game()
{
    /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /// TREBAM DA DODAM G I V UMESTO NULL I NULL KOD ARGUMENATA FUNKCIJI INPUT!!!
    /// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int speed = 1000; // koliko polja se pomeri u sekundi
    COORD c;

    init_game();
    system("cls");
    nacrtaj();
    c.X = 0;
    c.Y = 17;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout << "Score: " << 0;
    while(dead == false)
    {
        input(true, NULL, NULL);
        pomeri(true);
        nacrtaj_zmiju();
        sleep_funkcija(1000 / speed);
    }
    c.X = 0;
    c.Y = 17;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    cout << "Final score: " << score;
}
