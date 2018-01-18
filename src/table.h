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

    Ime fajla:      table.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <stdio.h> //printf

#include "move.h"

#define WIDTH 15
#define HEIGHT 15
#define PLAYING 0
#define PLAYER1_WIN 1
#define PLAYER2_WIN 2
#define DRAW 3
#define PLAYER1_DISQ -1
#define PLAYER2_DISQ -2
#define INVALID_PLAYER -3

#define NONE 0
#define PLAYER1 1
#define PLAYER2 2

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))

typedef struct TableStruct
{
    char matrix[WIDTH][HEIGHT];
    int status;
    int fields_taken;
} Table;

void initTable(Table *t);
int validMove(Move next, Table t);
void printTable(Table t);
int CheckWinner(Table t, Move next);
Table nextMove(Move next, Table t);

#endif
