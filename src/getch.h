/*
    ********************************************************************
    Autori:         RT-RK
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2017/2018
    Semestar:       Zimski (V)

    Ime fajla:      getch.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef GETCH_H_INCLUDED
#define GETCH_H_INCLUDED

#include <termios.h> //tcgetattr
#include <stdio.h>   //getchar

void initTermios(int echo);
void resetTermios();
char getch_(int echo);
char getch();
char getche();

#endif
