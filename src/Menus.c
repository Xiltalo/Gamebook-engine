#include <Menus.h>
#include <Modules.h>

#include <stdio.h>
#include <stdlib.h>

// renvoie 1 ou 2 selon le choix. affiche une image et un texte avant le choix
int MenuChoix(char * str1, char * str2, char *textfile, int id_text, char *imagefile, int id_image)
{
    int c = 0;

    int choix = 1;
    system(CLEAR);

    LireImage(id_image, imagefile);
    LirePassage(id_text, textfile);
    printf("\n\n > | %s |    | %s |    ", str1, str2);

    while (c != KEY_ENTER)
    {
        c = 0;

        switch ((c = GETCH()))
        {
        case KEY_LEFT:
            if (choix == 2)
            {
                choix = 1;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n > | %s |    | %s |    ", str1, str2);
            }
            break;
        case KEY_RIGHT:
            if (choix == 1)
            {
                choix = 2;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |  > | %s |    ", str1, str2);
            }
            break;
        case KEY_ENTER:
            break;
        default:
            //printf("%d", c);
            fflush(stdout);
        }
    }

    return choix;
}

// comme MenuChoix, mais propose 3 choix
int MenuFourche(char * str1, char * str2, char * str3, char *textfile, int id_text, char *imagefile, int id_image, int instantane)
{
    int c = 0;

    int choix = 1;
    system(CLEAR);

    LireImage(id_image, imagefile);
    if (instantane)
        LirePassageInstantane(id_text, textfile);
    else LirePassage(id_text, textfile);    
    printf("\n\n > | %s |    | %s |    | %s |", str1, str2, str3);

    while (c != KEY_ENTER)
    {
        c = 0;

        switch ((c = GETCH()))
        {
        case KEY_LEFT:
            if (choix == 2)
            {
                choix = 1;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n > | %s |    | %s |    | %s |", str1, str2, str3);
            }
            else if (choix == 3)
            {
                choix = 2;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |  > | %s |    | %s |", str1, str2, str3);
            }
            break;
        case KEY_RIGHT:
            if (choix == 1)
            {
                choix = 2;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |  > | %s |    | %s |", str1, str2, str3);
            }
            else if (choix == 2)
            {
                choix = 3;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |    | %s |  > | %s |", str1, str2, str3);
            }
            break;
        case KEY_ENTER:
            break;
        default:
            //printf("%d", c);
            fflush(stdout);
        }
    }

    return choix;
}

//Menu choix qui propose 4 options
int MenuQuatre(char * str1, char * str2, char * str3, char * str4, char *textfile, int id_text, char *imagefile, int id_image, int instantane)
{
    int c = 0;

    int choix = 1;
    system(CLEAR);

    LireImage(id_image, imagefile);
    if (instantane)
        LirePassageInstantane(id_text, textfile);
    else LirePassage(id_text, textfile);    
    printf("\n\n > | %s |    | %s |    | %s |    | %s |", str1, str2, str3, str4);

    while (c != KEY_ENTER)
    {
        c = 0;

        switch ((c = GETCH()))
        {
        case KEY_LEFT:
            if (choix == 2)
            {
                choix = 1;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n > | %s |    | %s |    | %s |    | %s |", str1, str2, str3, str4);
            }
            else if (choix == 3)
            {
                choix = 2;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |  > | %s |    | %s |    | %s |", str1, str2, str3, str4);
            }
            else if (choix == 4)
            {
                choix = 3;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |    | %s |  > | %s |    | %s |", str1, str2, str3, str4);
            }
            break;
        case KEY_RIGHT:
            if (choix == 1)
            {
                choix = 2;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |  > | %s |    | %s |    | %s |", str1, str2, str3, str4);
            }
            else if (choix == 2)
            {
                choix = 3;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |    | %s |  > | %s |    | %s |", str1, str2, str3, str4);
            }
            else if (choix == 3)
            {
                choix = 4;
                system(CLEAR);
                LireImage(id_image, imagefile);
                LirePassageInstantane(id_text, textfile);
                printf("\n\n   | %s |    | %s |    | %s |  > | %s |", str1, str2, str3, str4);
            }
            break;
        case KEY_ENTER:
            break;
        default:
            //printf("%d", c);
            fflush(stdout);
        }
    }

    return choix;
}

//choix 2 options pour linux
int MenuChoixLinux(char * str1, char * str2, char *textfile, int id_text, char *imagefile, int id_image)
{
    int choix = 1;
    char c;
    system(CLEAR);

    LireImage(id_image, imagefile);
    LirePassage(id_text, textfile);
    printf("\n\n   1 : | %s |\n   2 : | %s |    \n\nchoice : ", str1, str2);
    char verif = 0;
    while (!verif)
    {
        c = 0;
        switch ((c = getchar()))
        {
        case '1':
            choix = 1;
            verif = 1;
            break;
        case '2':
            choix = 2;
            verif = 1;
            break;
        default:
            puts("\n");
            system(CLEAR);
            LireImage(id_image, imagefile);
            LirePassageInstantane(id_text, textfile);
            printf("\n\n   1 : | %s |\n   2 : | %s |    \n\nchoice : ", str1, str2);
            fflush(stdin);
        }
    }
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // to flush stdin

    return choix;
}

// affiche une scene puis donne un choix avec 3 options.
int MenuFourcheLinux(char * str1, char * str2, char * str3, char *textfile, int id_text, char *imagefile, int id_image, int instantane)
{
    int choix = 1;
    char c;
    system(CLEAR);

    LireImage(id_image, imagefile);
    if (instantane)
        LirePassageInstantane(id_text, textfile);
    else LirePassage(id_text, textfile);

    printf("\n\n   1 : | %s |\n   2 : | %s |\n   3 : | %s |    \n\nchoice : ", str1, str2, str3);
    char verif = 0;
    while (!verif)
    {
        c = 0;
        switch ((c = getchar()))
        {
        case '1':
            choix = 1;
            verif = 1;
            break;
        case '2':
            choix = 2;
            verif = 1;
            break;
        case '3':
            choix = 3;
            verif = 1;
            break;
        default:
            puts("\n");
            system(CLEAR);
            LireImage(id_image, imagefile);
            LirePassageInstantane(id_text, textfile);
            printf("\n\n   1 : | %s |\n   2 : | %s |\n   3 : | %s |    \n\nchoice : ", str1, str2, str3);
            fflush(stdin);
        }
    }
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // to flush stdin

    return choix;
}

// menu 4 options pour linux
int MenuQuatreLinux(char * str1, char * str2, char * str3, char * str4, char *textfile, int id_text, char *imagefile, int id_image, int instantane)
{
    int choix = 1;
    char c;
    system(CLEAR);

    LireImage(id_image, imagefile);
    if (instantane)
        LirePassageInstantane(id_text, textfile);
    else LirePassage(id_text, textfile);

    printf("\n\n   1 : | %s |\n   2 : | %s |\n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
    char verif = 0;
    while (!verif)
    {
        c = 0;
        switch ((c = getchar()))
        {
        case '1':
            choix = 1;
            verif = 1;
            break;
        case '2':
            choix = 2;
            verif = 1;
            break;
        case '3':
            choix = 3;
            verif = 1;
            break;
        case '4':
            choix = 4;
            verif = 1;
            break;
        default:
            puts("\n");
            system(CLEAR);
            LireImage(id_image, imagefile);
            LirePassageInstantane(id_text, textfile);
            printf("\n\n   1 : | %s |\n   2 : | %s |\n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
            fflush(stdin);
        }
    }
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // to flush stdin

    return choix;
}

// crée une sauvegarde complètement vierge (système de sauvegarde à implémenter et à préciser)
void NouvelleSave(char *savefile)
{
    FILE *f;
    f = fopen(savefile, "w");

    fprintf(f, "%d\n%s\n%s", 0, "000000000000000000000000000000000000000", "000000000000000000000000000000000000000"); // le 0 au début est le module de départ.

    fclose(f);
}

// sauvegarde une certaine valeur dans le fichier de sauvegarde (système de sauvegarde à implémenter et à préciser)
void Save(char *savefile, int id_module, char *events, char *inventaire)
{
    FILE *f;
    f = fopen(savefile, "w");

    if (f == NULL)
    {
        NouvelleSave(savefile);
    }
    else
    {
        fprintf(f, "%d\n%s\n%s", id_module, events, inventaire);
        fclose(f);
    }

    
}

// lit un fichier de sauvegarde et charge les informations appropriées en mémoire
void Load(char *savefile, int *id, char *events, char *inventaire) //, char events[40], char inventaire[40]) // events[40] et inventaire[40]
{
    FILE *f;
    f = fopen(savefile, "r");
    if (f == NULL)
    {
        NouvelleSave(savefile);
        f = fopen(savefile, "r"); // now that it exists for sure
        fscanf(f, "%d\n%s\n%s", id, events, inventaire);
        fclose(f);
    }
    else
    {
        fscanf(f, "%d\n%s\n%s", id, events, inventaire); //, events, inventaire);
        fclose(f);
    }
}

//Lit une entrée du fichier objet et affiche une représentation formatée de l'information
void AfficherObjet( int id_objet, char* objets_file)
{
    int id;
    char nom[100];
    char description[500];
    FILE *f;
    f = fopen(objets_file, "r");

    if(f != NULL)
    {
        fscanf(f, ":%d:", &id);
        while (id != id_objet && id != ID_END)
        {
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
        }
        if (id == id_objet)
        {
            fscanf(f,"%99[^;]", nom);
            fscanf(f,";%499[^$]", description);
            printf("\n<%s> \n\n%s\n",nom, description);
        }
        else
        {
            printf("error, object not found");
        }
        fclose(f);
    }
    else printf("error, file not found : %s", objets_file);

}

// Un menu pour consulter l'inventaire
void ConsulterInventaire(char * inventaire)
{
    //transformer l'inventaire en un tableau de valeurs.
    int size_invent = TAILLEINVENTAIRE -1;//sizeof(inventaire)/sizeof(inventaire[0]);
    int taillerelle = 0;
    char reponse,c = 0;
    int i;
    for (i = 0; i < size_invent; i++) // compter le nombre d'objets dans l'inventaire
    {
        if(inventaire[i] != '0') taillerelle ++;
    }
    int true_inventaire[taillerelle];
    taillerelle = 0;
    for (i = 0; i < size_invent; i++) // créer un tableau qui indique l'id de chaque élément de l'inventaire.
    {
        if(inventaire[i] != '0') 
        {
            true_inventaire[taillerelle] = i;
            taillerelle ++;
        }
    }

    // 2 : proposer un menu de choix qui permet de faire défiler l'inventaire, ou de sortir
    i = 0;
    puts("\n");
    while (reponse != 1) // le menu continue tant que le joueur n'a pas choisi de sortir du menu
    {
        //while ((c = getchar()) != '\n' && c != EOF){}
                system(CLEAR);
                printf("\n\n\n\n\n\n\n\n               OWNED OBJECTS               ");
                printf("\n\n\n\n\n\n\n\n--------------------------------------------------------\n                     OBJET %d :\n",i+1);                    
                AfficherObjet(true_inventaire[i], OBJETS);
                printf("\n--------------------------------------------------------\n1 : | Previous Object |           2 : | Next Object |\n3 : | Quit the menu |\n\nChoice: ");
                fflush(stdin);
        char verif = 0;
        while (!verif)
        {
            c = 0;
            switch ((c = getchar()))
            {
            case '1': // "objet précédent"
                if (i == 0) i = taillerelle - 1;
                else i--;
                verif = 1;
                break;                         
            case '2': // "objet suivant"
                if (i == taillerelle - 1) i = 0;
                else i++;
                verif = 1;
                break;
            case '3':
                reponse = 1;
                verif = 1;
                break;
                
            default:
                puts("\n");
                system(CLEAR);
                printf("\n\n\n\n\n\n\n\n--------------------------------------------------------\n                     OBJET %d :\n",i+1);                    
                AfficherObjet(true_inventaire[i], OBJETS);
                printf("\n--------------------------------------------------------\n1 : | Previous Object |           2 : | Next Object |\n3 : | Quit the menu |\n\nChoice: ");
                fflush(stdin);
            }
        }
        while ((c = getchar()) != '\n' && c != EOF){} // to flush stdin
        
    }

}
