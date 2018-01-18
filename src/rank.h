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

    Ime fajla:      getch.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef RANK_H_INCLUDED
#define RANK_H_INCLUDED

#include <pthread.h> //mutex
#include <stdio.h>   //perror
#include <string.h>  //strcpy, strcmp
#include <math.h>    //pow
#include <stdlib.h>  //exit

#include "config.h"

typedef struct rankStruct
{
    char player[USERNAME_CHARS];
    int rank;
} rank;

extern rank players[MAX_PLAYERS];
extern int players_number;
extern pthread_mutex_t rank_mutex;

int readRanks();
void updateRank(rank r);
void saveRanks();
void sortRanks();
rank *getRankByName(char *name);
void eloRank(char *player1, char *player2, int result);
void sendRank(int clientSock);

#endif
