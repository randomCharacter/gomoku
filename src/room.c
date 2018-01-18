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

    Ime fajla:      room.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include "room.h"

/* Creates new room, making server able to create other rooms */
void createNewRoom(int client1Sock, struct sockaddr_in client1Address, int client2Sock, struct sockaddr_in client2Address)
{
    pthread_t gameThread;
    param *p = (param *)malloc(sizeof(param));
    p->client1Socket = client1Sock;
    p->client1Address = client1Address;
    p->client2Socket = client2Sock;
    p->client2Address = client2Address;
    pthread_create(&gameThread, NULL, createThread, (void *)p);
    pthread_detach(gameThread);
}

/* Creates new thread for room */
void *createThread(void *p)
{
    param *st = (param *)p;
    startRoom(st->client1Socket, st->client1Address, st->client2Socket, st->client2Address);
    return (void *)0;
}

/* Communicates with clients, and plays the game on server side */
void startRoom(int client1Sock, struct sockaddr_in client1Address, int client2Sock, struct sockaddr_in client2Address)
{
    Table table;
    initTable(&table);
    Move move;
    int player = 0;
    int len1 = 1, len2 = 1;
    char user1[USERNAME_CHARS], user2[USERNAME_CHARS];

    // Receive usernames from players
    if (recv(client1Sock, &user1, USERNAME_CHARS, 0) < 0)
    {
        perror("recv failed. Error");
        exit(1);
    }

    if (recv(client2Sock, &user2, USERNAME_CHARS, 0) < 0)
    {
        perror("recv failed. Error");
        exit(1);
    }

    // Send name of opponent to clients
    if (send(client1Sock, &user2, strlen(user2), 0) < 0)
    {
        perror("send failed. Error");
        exit(1);
    }

    if (send(client2Sock, &user1, strlen(user1), 0) < 0)
    {
        perror("send failed. Error");
        exit(1);
    }

    // LOG
    printf("[LOG] %s.%s vs %s!\n", "Room created", user1, user2);

    // Send table to first player
    if (send(client1Sock, &table, sizeof(Table), 0) == -1)
    {
        perror("send failed. Error");
        exit(1);
    }
    // Change to player 2
    player++;

    do
    {
        // Receive move
        if (player)
        {
            // Receive move from first player
            if ((len1 = recv(client1Sock, &move, sizeof(Move), 0)) < 0)
            {
                perror("recv failed. Error");
                exit(1);
            }
        }
        else
        {
            // Receive move from second player
            if ((len2 = recv(client2Sock, &move, sizeof(Move), 0)) < 0)
            {
                perror("recv failed. Error");
                exit(1);
            }
        }

        // Send response
        if (len1 && len2)
        {
            table = nextMove(move, table);
            player = (player + 1) % 2;
        }
        else
        {
            if (!len1)
            {
                table.status = PLAYER1_DISQ;
            }
            else if (!len2)
            {
                table.status = PLAYER2_DISQ;
            }
        }

        if (table.status == PLAYING)
        {
            if (player)
            {
                // Receive move from first player
                if (send(client1Sock, &table, sizeof(Table), 0) == -1)
                {
                    perror("send failed. Error");
                    exit(1);
                }
            }
            else
            {
                // Receive move from second player
                if (send(client2Sock, &table, sizeof(Table), 0) == -1)
                {
                    perror("send failed. Error");
                    exit(1);
                }
            }
        }
    } while (table.status == PLAYING);

    // Send result back to players
    if (send(client1Sock, &table, sizeof(Table), 0) == -1)
    {
        perror("send failed. Error");
        exit(1);
    }
    if (send(client2Sock, &table, sizeof(Table), 0) == -1)
    {
        perror("send failed. Error");
        exit(1);
    }

    // Update ranks
    eloRank(user1, user2, table.status);

    // LOG
    printf("[LOG] %s\n", "Ranks updated");
}
