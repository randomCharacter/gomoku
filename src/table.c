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

    Ime fajla:      table.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include "table.h"
#include "move.h"

/* Checks if given move is valid on the table */
int validMove(Move next, Table t)
{
    if (next.x < 0 || next.x > WIDTH - 1 || next.y < 0 || next.y > HEIGHT - 1)
    {
        return 0;
    }

    if (next.player != PLAYER1 && next.player != PLAYER2)
    {
        return 0;
    }

    if (t.matrix[next.x][next.y] == NONE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Initializes table to default values */
void initTable(Table *t)
{
    int i, j;
    for (i = 0; i < WIDTH; i++)
    {

        for (j = 0; j < HEIGHT; j++)
        {

            t->matrix[i][j] = 0;
        }
    }
    t->status = PLAYING;
    t->fields_taken = 0;
}

/* Checks if game is over */
int CheckWinner(Table t, Move next)
{
    int i;
    int counter = 0;

    int xLeftBorder, xRightBorder;
    int yLeftBorder, yRightBorder;

    xLeftBorder = next.x > 5 ? -5 : -next.x;
    yLeftBorder = next.y > 5 ? -5 : -next.y;

    xRightBorder = (next.x < WIDTH - 5) ? 5 : WIDTH - next.x;
    yRightBorder = (next.y < WIDTH - 5) ? 5 : WIDTH - next.y;

    for (i = xLeftBorder; i < xRightBorder; i++)
    {
        if (t.matrix[next.x + i][next.y] == next.player)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        if (counter >= 5)
            return next.player;
    }
    counter = 0;

    for (i = yLeftBorder; i < yRightBorder; i++)
    {
        if (t.matrix[next.x][next.y + i] == next.player)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        if (counter >= 5)
            return next.player;
    }
    counter = 0;

    for (i = max(xLeftBorder, yLeftBorder); i < min(xRightBorder, yRightBorder); i++)
    {
        if (t.matrix[next.x + i][next.y + i] == next.player)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        if (counter >= 5)
            return next.player;
    }
    counter = 0;

    for (i = max(max(xLeftBorder, yLeftBorder), min(-xRightBorder, -yRightBorder)); i < min(max(-xLeftBorder, -yLeftBorder), min(xRightBorder, yRightBorder)); i++)
    {
        if (t.matrix[next.x + i][next.y - i] == next.player)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }
        if (counter >= 5)
            return next.player;
    }
    counter = 0;

    if (t.fields_taken == WIDTH * HEIGHT)
    {
        return DRAW;
    }

    return 0;
}

/* Plays next move */
Table nextMove(Move next, Table t)
{
    if (next.x < 0 || next.x > WIDTH - 1 || next.y < 0 || next.y > HEIGHT - 1)
    {
        if (next.player == PLAYER1)
        {
            t.status = PLAYER1_DISQ;
            return t;
        }
        else if (next.player == PLAYER2)
        {
            t.status = PLAYER2_DISQ;
            return t;
        }
        else
        {
            t.status = INVALID_PLAYER;
            return t;
        }
    }

    if (next.player != PLAYER1 && next.player != PLAYER2)
    {
        t.status = INVALID_PLAYER;
        return t;
    }

    if (t.matrix[next.x][next.y] == NONE)
    {
        t.matrix[next.x][next.y] = next.player;
    }
    else
    {
        if (next.player == PLAYER1)
        {
            t.status = PLAYER1_DISQ;
            return t;
        }
        else if (next.player == PLAYER2)
        {
            t.status = PLAYER2_DISQ;
            return t;
        }
        else
        {
            t.status = INVALID_PLAYER;
            return t;
        }
    }

    t.fields_taken++;
    int win = CheckWinner(t, next);

    t.status = win;

    return t;
}
