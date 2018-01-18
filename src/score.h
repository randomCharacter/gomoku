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

    Ime fajla:      score.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <sys/socket.h> //socket

#include "config.h"
#include "rank.h"

typedef struct scoreStruct
{
    rank ranks[RANKS_TO_SEND];
    int size;
} score;

void sendScore(int socket);
score receiveScore(int socket);
void printScore(score s);

#endif
