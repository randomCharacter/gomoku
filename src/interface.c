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

    Ime fajla:      interface.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/
#include "interface.h"
#include "nunchuk.h"

char getKey()
{
    FILE *f;
    if ((f = fopen(DEVICE_NAME, "rb")) == NULL)
    {
        printf("fopen failed.\n");
        return -1;
    }

    nunchuk n;

    if (fread(&n, sizeof(nunchuk), 1, f) <= 0) {
        exit(1);
    }

    fclose(f);

    if (n.c)
    {
        return 'q';
    }
    if (n.z)
    {
        return 'y';
    }
    if (n.x < 10)
    {
        return 'a';
    }
    if (n.x > 240)
    {
        return 'd';
    }
    if (n.y < 10)
    {
        return 's';
    }
    if (n.y > 240)
    {
        return 'w';
    }
    return ' ';
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void disableCursor()
{
#ifndef _WIN32
    system("setterm -cursor off");
#endif
}

void enableCursor()
{
#ifndef _WIN32
    system("setterm -cursor on");
#endif
}

/* Draws table with field xSelected, ySelected selected */
void draw(Table t, int xSelected, int ySelected, char *user1, char *user2)
{
    int x, y, i;
    char reset[] = "\033[m";

    clearScreen();

    printf("%s", GRN);
    printf("   %s",user1);
    int lenght1=0;
    while(user2[lenght1]!='\0')
    {
        lenght1++;
    }
    int lenght2=0;
    while(user2[lenght2]!='\0')
    {
        lenght2++;
    }
    for(i=0;i<WIDTH*5-lenght1-lenght2;i++)
    {
        printf(" ");
    }
    printf("%s", RED);
    printf("%s\n",user2);
    printf("%s", reset);

    printf("  ");
    for (i = 0; i < WIDTH; i++)
    {
        if (i == 0)
        {
            printf("╔");
        }
        printf("═════");
    }
    printf("╗\n");

    int firstTime = 0;
    for (x = 0; x < HEIGHT; x++)
    {
        if (firstTime)
        {
            printf("  ║");
            for (i = 0; i < WIDTH; i++)
            {
                printf("     ");
            }
            printf("║\n");
        }
        firstTime = 1;
        for (y = 0; y < WIDTH; y++)
        {

            if (y == 0)
            {
                printf("  ║");
            }

            if (t.matrix[x][y] == NONE)
            {
                if (x == xSelected && y == ySelected)
                {
                    printf("%s", WHT_BACKGROUND);
                    printf("  %c  ", CHAR_EMPTY);
                }
                else
                {
                    printf("%s", WHT);
                    printf("  %c  ", CHAR_EMPTY);
                }
            }
            else if (t.matrix[x][y] == PLAYER1)
            {
                if (x == xSelected && y == ySelected)
                {
                    printf("%s", GRN_BACKGROUND);
                    printf("  %c  ", CHAR_PLAYER1);
                }
                else
                {
                    printf("%s", GRN);
                    printf("  %c  ", CHAR_PLAYER1);
                }
            }
            else if (t.matrix[x][y] == PLAYER2)
            {
                if (x == xSelected && y == ySelected)
                {
                    printf("%s", RED_BACKGROUND);
                    printf("  %c  ", CHAR_PLAYER2);
                }
                else
                {
                    printf("%s", RED);
                    printf("  %c  ", CHAR_PLAYER2);
                }
            }
            else
            {
                return;
            }

            printf("%s", reset);
            if (y == WIDTH - 1)
            {
                printf("║");
            }
        }
        printf("\n");
    }
    printf("  ");
    for (i = 0; i < WIDTH; i++)
    {
        if (i == 0)
        {
            printf("╚");
        }
        printf("═════");
    }
    printf("╝\n");
}

/* Draws table and allows player to select field to play */
Move showInterface(Table t, int player, char *user1, char *user2)
{
    static int x = 0;
    static int y = 0;
    static int pressed = 0;
    Move m;
    m.player = player;
    char playerIn = 0;

    while (1)
    {
        draw(t, x, y, user1, user2);

        playerIn = getKey();
        switch (playerIn)
        {
        case 'q':
            if (!pressed)
            {
                pressed = 1;
            }
            else
            {
                m.x = x;
                m.y = y;
                return m;
            }
            break;
        case 'w':
            // key up
            if (x > 0)
            {
                x--;
                break;
            }
            break;
        case 's':
            // key down
            if (x < HEIGHT - 1)
            {
                x++;
                break;
            }
            break;
        case 'd':
            // key right
            if (y < WIDTH - 1)
            {
                y++;
                break;
            }
            break;
        case 'a':
            // key left
            if (y > 0)
            {
                y--;
                break;
            }
            break;
        }
    }
}
