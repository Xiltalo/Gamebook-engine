#include <Scripts.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// lit une ligne d'un fichier, de longueur définie
char *getLine(FILE *f, int linesize)
{
    char c;
    int i = 0;
    char *string = malloc(sizeof(char) * linesize);
    while (c = fgetc(f) != '\n' && i < linesize - 1 && c != EOF)
    {
        *(string + i) = c;
        i++;
    }
    return string;
}

// lit des caractères d'un fichier cible jusqu'à arriver à un certain caractère
void skipUntilChar(FILE *filestream, char c)
{
    char u = ' ';
    while (u != c && u != EOF)
    {
        u = fgetc(filestream);
    }
}

// demande à l'utilisateur d'appuyer sur entrée (n'accepte pas d'autre touche)
void pressEnter()
{
    char enter = 0;
    char c;
    fflush(stdin);
    fflush(stdout);
    //while ((c = getchar()) != '\n' && c != EOF){} // to flush stdin

    while (enter != '\r' && enter != '\n' && c != EOF)
    {
        enter = getchar();
    }
}

// lit un texte depuis le ficher indiqué, avec un certain rythme
void LirePassage(int scene_id, char *filename)
{
    FILE *f;
    f = fopen(filename, "r");

    if (f != NULL)
    {
        int id;

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != scene_id && id != ID_END)
        {
            skipUntilChar(f, FINPASSAGE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        int speed = READING_SPEED;
        char c = fgetc(f);
        while (c != FINPASSAGE && c != EOF)
        {
            if (c == DOUBLESPEED)
                speed *= 2;
            else if (c == HALFSPEED)
                speed /= 2;
            else if (c == WAITCHAR)
                SLEEP(speed);
            else if (c == LINEJUMP)
            {
                SLEEP(speed);
                printf("%c", '\n');
                fflush(stdout);
            }
            else
            {
                SLEEP(speed);
                printf("%c", c);
                fflush(stdout);
            }
            c = fgetc(f);
        }
        SLEEP(speed);
        // printf("\n");
        fclose(f);
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    
}

// affiche instntanément un texte depuis le fichier indiqué. ignore les indicateurs de vitesse.
void LirePassageInstantane(int scene_id, char *filename)
{
    FILE *f;
    f = fopen(filename, "r");

    if (f != NULL)
    {
        int id;

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != scene_id && id != ID_END)
        {
            skipUntilChar(f, FINPASSAGE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        int speed = READING_SPEED;
        char c = fgetc(f);
        while (c != FINPASSAGE && c != EOF)
        {
            if (c == DOUBLESPEED)
                speed = 0;
            else if (c == HALFSPEED)
                speed = 0;
            else if (c == WAITCHAR)
                speed = 0;
            else if (c == LINEJUMP)
            {
                // SLEEP(speed);
                printf("%c", '\n');
            }
            else if (c== REFRESHCHAR)
            {
                
            }
            else
            {
                // SLEEP(speed);
                printf("%c", c);
            }
            c = fgetc(f);
        }
        SLEEP(speed);
        // printf("\n");
        fclose(f);
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    
}

// lit une scène avec une image statique, comme un dialogue de jeu vidéo. finit par ---> et pressenter à modifier selon choix esthétiques
void LireScene2(int id_image, int scene_id, char *file_image, char *file_text)
{

    FILE *f;
    f = fopen(file_text, "r");

    if (f != NULL)
    {
        int id;

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != scene_id && id != ID_END) // récupération du passage dans le fichier texte
        {
            skipUntilChar(f, FINPASSAGE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        int speed = READING_SPEED;
        system(CLEAR);
        LireImage(id_image, file_image);
        // printf("\n");
        char c = fgetc(f);
        while (c != FINPASSAGE && c != EOF)
        {
            if (c == DOUBLESPEED)
                speed *= 2;
            else if (c == HALFSPEED)
                speed /= 2;
            else if (c == WAITCHAR)
                SLEEP(speed);
            else if (c == REFRESHCHAR)
            {
                //skipUntilChar(f, '\n');
                printf("\n\n--->");
                fflush(stdout);
                pressEnter();
                system(CLEAR);
                LireImage(id_image, file_image);
                // printf("\n");
                // skipUntilChar(f,'\n');
            }
            else if (c == LINEJUMP)
            {
                SLEEP(speed);
                printf("%c", '\n');
                fflush(stdout);
            }
            else if (c == CHANGEIMAGE)
            {
                fscanf(f, "%d", &id_image);
            }
            else
            {
                SLEEP(speed);
                printf("%c", c);
                fflush(stdout);
            }
            c = fgetc(f);
        }
        SLEEP(speed);
        printf("\n--->");
        // fflush(stdout);
        pressEnter();
        // printf("\n");
        fclose(f);
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", file_text);
        printf("\n");
    }
    
}

// affiche une image, puis lit un texte. finit par ---> et pressenter
void LireScene(int id_image, int id_passage, char *imagefile, char *textfile)
{
    system(CLEAR);
    LireImage(id_image, imagefile);
    printf("\n");
    LirePassage(id_passage, textfile);
    printf("\n--->");
    pressEnter();
}
