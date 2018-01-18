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

    Ime fajla:      config.h
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

// Server config
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015
#define SERVER_IP "127.0.0.1"

// Rank
#define USERNAME_CHARS 50
#define MAX_PLAYERS 200
#define FILE_NAME "rank.txt"
#define STARTING_RANK 1200
#define RANKS_TO_SEND 10

// Custom strings
#define WAITING_MESSAGE "Waiting for opponent..."

// Characters
#define CHAR_EMPTY ' '
#define CHAR_PLAYER1 'x'
#define CHAR_PLAYER2 'o'

// Server constants
#define GET_SCORE 0
#define PLAY_GAME 1

#endif
