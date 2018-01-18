/*
    ********************************************************************
    Autori:         Kosta Svrdlan, Mario Peric, Igor Ilic
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2017/2018
    Semestar:       Zimski (V)

    Ime fajla:      interface.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <stdio.h>  //printf
#include <stdlib.h> //system

#include "config.h"
#include "table.h"
#include "move.h"
#include "getch.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define WHT "\x1B[37m"
#define WHT_BACKGROUND "\x1B[47m"
#define RED_BACKGROUND "\x1B[41m"
#define GRN_BACKGROUND "\x1B[42m"

void clearScreen();
void disableCursor();
void enableCursor();
void draw(Table t, int xSelected, int ySelected, char *user1, char *user2);
Move showInterface(Table t, int player, char *user1, char *user2);

#endif
