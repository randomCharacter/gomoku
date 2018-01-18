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

    Ime fajla:      room.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <arpa/inet.h> //inet_addr
#include <stdlib.h>    //exit
#include <stdio.h>     //perror
#include <pthread.h>   //thread

#include "table.h"
#include "move.h"
#include "config.h"
#include "rank.h"

void createNewRoom(int client1Sock, struct sockaddr_in client1Address, int client2Sock, struct sockaddr_in client2Address);
void *createThread(void *p);
void startRoom(int client1Sock, struct sockaddr_in client1Address, int client2Sock, struct sockaddr_in client2Address);

typedef struct paramStruct
{
    int client1Socket;
    struct sockaddr_in client1Address;
    int client2Socket;
    struct sockaddr_in client2Address;
} param;

#endif
