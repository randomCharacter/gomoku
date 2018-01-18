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

    Ime fajla:      score.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include "score.h"

/* Sends score to client */
void sendScore(int socket)
{
    int size = readRanks();
    score s;

    int i;
    s.size = size < RANKS_TO_SEND ? size : RANKS_TO_SEND;
    for (i = 0; i < s.size; i++)
    {
        s.ranks[i] = players[i];
    }

    /* Send structure */
    if (send(socket, &s, sizeof(score), 0) < 0)
    {
        perror("send failed. Error");
        exit(1);
    }

    /* Send each of top ranks */
    for (i = 0; i < s.size; i++)
    {
        if (send(socket, &s.ranks[i], sizeof(rank), 0) < 0)
        {
            perror("send failed. Error");
            exit(1);
        }
    }
}

/* Receives score from server */
score receiveScore(int socket)
{
    score s;
    // Receive structure
    if (recv(socket, &s, sizeof(score), 0) < 0)
    {
        perror("recv failed. Error");
        exit(1);
    }

    int i;
    rank k;
    // Receive top ranks
    for (i = 0; i < s.size; i++)
    {
        if (recv(socket, &k, sizeof(rank), 0) < 0)
        {
            perror("recv failed. Error");
            exit(1);
        }
        s.ranks[i] = k;
    }

    return s;
}

/* Prints score */
void printScore(score s)
{
    int i;
    for (i = 0; i < s.size; i++)
    {
        printf("%2d. %10s %d\n", i + 1, s.ranks[i].player, s.ranks[i].rank);
    }
}
