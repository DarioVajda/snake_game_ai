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
pair<int, int> directions[4] = {
    make_pair(1, 0), // desno
    make_pair(0, 1), // dole
    make_pair(-1, 0),// levo
    make_pair(0, -1) // gore
};
vector<pair<int, int>> zmija; /// zmija je velicine 4 i pocinje levo na sredini
pair<int, int> nestaliRep;
pair<int, int> jabuka;

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

void input(bool ai)
{
    if(ai == true)
    {
        int rez = feed_forward(); // 1 - levo, 2 - pravo, 3 - desno
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
			case 'w':
                if(dir != 1)
				dir = 3; break;
			case 'd':
			    if(dir != 2)
				dir = 0; break;
			case 's':
			    if(dir != 3)
				dir = 1; break;
			case 'a':
			    if(dir != 0)
				dir = 2; break;
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

void pomeri()
{
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

void normal_game();

int main()
{
    normal_game();
	return 0;
}

void normal_game()
{
    srand((unsigned)time(0));

    int speed = 5; // koliko polja se pomeri u sekundi
    int acceleration = 1000;
    COORD c;

    while(true)
    {
        init_game();
        system("cls");
        nacrtaj();
        while(dead == false)
        {
            input(false);
            pomeri();
            nacrtaj_zmiju();
            sleep_funkcija(1000 / speed);
        }
        c.X = 0;
        c.Y = 17;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
        cout << "Final score: " << score;
        sleep_funkcija(1000);
        speed += acceleration;
    }
}
