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

    Ime fajla:      rank.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include "rank.h"

rank players[MAX_PLAYERS];
int players_number = 0;
pthread_mutex_t rank_mutex;

/* Read ranks from file into array */
int readRanks()
{
    // Disable access to file
    pthread_mutex_lock(&rank_mutex);
    
    players_number = 0;
    FILE *f;
    if ((f = fopen(FILE_NAME, "r")) == NULL)
    {
        perror("fopen failed. Error");
        return 0;
    }
    int rank;
    char user[USERNAME_CHARS];
    int i = 0;
    while (fscanf(f, "%s %d", user, &rank) != EOF)
    {
        strcpy(players[i].player, user);
        players[i].rank = rank;
        i++;
    }

    fclose(f);
    // Enable access to file
    pthread_mutex_unlock(&rank_mutex);

    players_number = i;
    return i;
}

/* Sorts ranks */
void sortRanks()
{
    int i, j;
    for (i = 0; i < players_number; i++)
    {
        for (j = i; j < players_number; j++)
        {
            if (players[i].rank < players[j].rank)
            {
                rank t = players[i];
                players[i] = players[j];
                players[j] = t;
            }
        }
    }
}

/* Inserts new rank into array */
rank *addRank(char *name)
{
    if (players_number < MAX_PLAYERS)
    {
        strcpy(players[players_number].player, name);
        players[players_number].rank = STARTING_RANK;
    }
    return &players[players_number++];
}

/* Saves ranks back to file */
void saveRanks()
{
    // Disable access to file
    pthread_mutex_lock(&rank_mutex);

    if (players_number == 0)
    {
        return;
    }

    FILE *f;
    if ((f = fopen(FILE_NAME, "w")) == NULL)
    {
        perror("fopen failed. Error");
        exit(1);
    }

    int i;
    for (i = 0; i < players_number; i++)
    {
        fprintf(f, "%s %d\n", players[i].player, players[i].rank);
    }

    fclose(f);

    // Enable access to file
    pthread_mutex_unlock(&rank_mutex);
}

/* Returns rank by given name */
rank *getRankByName(char *name)
{
    int i;
    for (i = 0; i < players_number; i++)
    {
        if (strcmp(players[i].player, name) == 0)
        {
            return &players[i];
        }
    }

    return NULL;
}

/* Calculates new rank by ELO algorithm, and saves them, after the game is finished */
void eloRank(char *player1, char *player2, int result)
{
    readRanks();

    rank *playerFirst = getRankByName(player1);
    rank *playerSecond = getRankByName(player2);
    if (playerFirst == NULL)
    {
        playerFirst = addRank(player1);
    }
    if (playerSecond == NULL)
    {
        playerSecond = addRank(player2);
    }

    // Expected victory outcome for the first and second player
    double exWOFirst = 0;
    double exWOSecond = 0;

    // Gain
    int K = 30;

    exWOFirst = 1 / (1 + pow(10, ((playerSecond->rank - playerFirst->rank) / 400))); //expected victory outcome for the first and second player
    exWOSecond = 1 / (1 + pow(10, ((playerFirst->rank - playerSecond->rank) / 400)));

    if (result == 1) // if the first player won
    {
        playerFirst->rank = playerFirst->rank + (K * (2 - exWOFirst));
        playerSecond->rank = playerSecond->rank - (K * (2 - exWOFirst));
    }
    else if (result == 2) // if the second player won
    {
        playerSecond->rank = playerSecond->rank + (K * (2 - exWOSecond));
        playerFirst->rank = playerFirst->rank - (K * (2 - exWOSecond));
    }
    else // if draw
    {
        playerFirst->rank = playerFirst->rank + (K * (1 / 2.0 - exWOFirst));
        playerSecond->rank = playerSecond->rank + (K * (1 / 2.0 - exWOSecond));
    }

    sortRanks();
    saveRanks();
}
