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

    Ime fajla:      client.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <fcntl.h>      //for open
#include <unistd.h>     //for close
#include <stdlib.h>     //atoi

#include "config.h"
#include "move.h"
#include "table.h"
#include "interface.h"
#include "score.h"

int main(int argc, char *argv[])
{
    int sock;
    int gameState = 0;
    struct sockaddr_in server;
    char username[USERNAME_CHARS], opponent[USERNAME_CHARS];
    char *user1, *user2;
    int number;
    Move move;
    Table matrix;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    // Check if player wants to play or get score
    int mode = argc > 1 ? GET_SCORE : PLAY_GAME;

    // Send mode to server
    if (send(sock, &mode, sizeof(int), 0) < 0)
    {
        perror("send failed. Error");
        return 1;
    }

    // If player wants to play
    if (mode == PLAY_GAME)
    {
        // Receive player number
        if (recv(sock, &number, sizeof(int), 0) == -1)
        {
            perror("recv failed. Error");
            return 1;
        }

        // If first player, display waiting message
        if (number == 1)
        {
            puts(WAITING_MESSAGE);
        }

        printf("You are player %d!\n", number);

        // Get player username
        printf("Enter username: ");
        scanf("%s", username);

        disableCursor();

        // Send username
        if (send(sock, &username, USERNAME_CHARS, 0) < 0)
        {
            perror("send failed. Error");
            return 1;
        }

        // Receive opponent name
        if (recv(sock, &opponent, USERNAME_CHARS, 0) == -1)
        {
            perror("recv failed. Error");
            return 1;
        }

        if (number == 1)
        {
            user1 = username;
            user2 = opponent;
        }
        else
        {
            user1 = opponent;
            user2 = username;
        }

        move.player = number;

        // Play the game
        while (gameState == PLAYING)
        {
            // Get table from server
            if (recv(sock, &matrix, sizeof(Table), 0) == -1)
            {
                perror("recv failed. Error");
                return 1;
            }

            // Check for 'game over'
            gameState = matrix.status;
            if (matrix.status != PLAYING)
            {
                break;
            }

            // Get next move from input
            move = showInterface(matrix, number, user1, user2);
            puts(WAITING_MESSAGE);

            while ((validMove(move, matrix)) == 0)
            {
                printf("Error, move not allowed.\n");
                move = showInterface(matrix, number, user1, user2);
            }

            if (send(sock, &move, sizeof(Move), 0) < 0)
            {
                perror("send failed. Error");
                return 1;
            }
        }

        // Display final result
        showInterface(matrix, number, user1, user2);
        if (gameState == number || gameState == -((number) % 2 + 1))
        {
            printf("\n You won! \n");
        }
        else if (gameState == -number)
        {
            printf("\n You have been disqualified %d\n", gameState);
        }
        else if (gameState == DRAW)
        {
            printf("\n Draw! \n");
        }
        else
        {
            printf("\n You lost! \n");
        }
    }
    else // get score
    {
        score s = receiveScore(sock);
        printScore(s);
    }

    close(sock);
    enableCursor();

    return 0;
}
