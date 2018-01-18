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

    Ime fajla:      server.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write

#include "config.h"
#include "room.h"
#include "rank.h"
#include "score.h"

int main(int argc, char *argv[])
{
    int serverSocket;
    struct sockaddr_in server;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    // Make address reusable
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed. Error");
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    // Bind
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }

    // Listen
    listen(serverSocket, 10);

    // Initialize mutex
    pthread_mutex_init(&rank_mutex, NULL);

    int mode = PLAY_GAME;

    // Wait for new players and take them to new rooms
    while (1)
    {

        // LOG
        printf("[LOG] %s\n", "Waiting for players");
        int c = sizeof(struct sockaddr_in);
        int client1Sock, client2Sock;
        struct sockaddr_in client1Address, client2Address;

        // Accept connection from first client
        if ((client1Sock = accept(serverSocket, (struct sockaddr *)&client1Address, (socklen_t *)&c)) < 0)
        {
            perror("accept failed. Error");
            return 1;
        }

        if (recv(client1Sock, &mode, sizeof(int), 0) == -1)
        {
            perror("recv failed. Error");
            return 1;
        }

        if (mode == GET_SCORE)
        {
            sendScore(client1Sock);
            // LOG
            printf("[LOG] %s\n", "Sending score");
            continue;
        }
        
        // LOG
        printf("[LOG] %s\n", "Player 1 connected");

        int player = 1;
        // Send waiting message
        if (send(client1Sock, &player, sizeof(int), 0) < 0)
        {
            perror("send failed. Error");
            return 1;
        }
        do
        {
            // Accept connection from second client
            if ((client2Sock = accept(serverSocket, (struct sockaddr *)&client2Address, (socklen_t *)&c)) < 0)
            {
                perror("accept failed. Error");
                return 1;
            }

            if (recv(client2Sock, &mode, sizeof(int), 0) == -1)
            {
                perror("recv failed. Error");
                return 1;
            }

            if (mode == GET_SCORE)
            {
                // LOG
                printf("[LOG] %s\n", "Sending score");
                sendScore(client2Sock);
                continue;
            }
        } while (mode == GET_SCORE);
        
        // LOG
        printf("[LOG] %s\n", "Player 2 connected");

        player++;
        // Send number
        if (send(client2Sock, &player, sizeof(int), 0) < 0)
        {
            perror("send failed. Error");
            return 1;
        }

        createNewRoom(client1Sock, client1Address, client2Sock, client2Address);
    }

    close(serverSocket);
    pthread_mutex_destroy(&rank_mutex);

    return 0;
}
