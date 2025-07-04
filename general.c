#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// changements pour la compilation sous windows. fonctions alternatives pour windows, etc.
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#define SLEEP(a) Sleep(a)
#define SET_CONSOLE_UTF_8 SetConsoleOutputCP(CP_UTF8);
#define CLEAR "cls"
#define READING_SPEED 35 // le temps de latence entre chaque caractère affiché
#define MENUCHOIX(a, b, c, d, e, f) MenuChoix(a, b, c, d, e, f)
#define MENUFOURCHE(a, b, c, d, e, f, g, h) MenuFourche(a, b, c, d, e, f, g, h)
#define MENUQUATRE(a, b, c, d, e, f, g, h, i) MenuQuatre(a, b, c, d, e, f, g, h, i)
#define GETCH getch

#else
#include <unistd.h>
#define GETCH getchar // won't be used anyways
#define SET_CONSOLE_UTF_8
#define SLEEP(a) usleep(a)
#define CLEAR "clear"
#define READING_SPEED 35000 // le temps de latence entre chaque caractère affiché // idée : ces paramètres devraient être modifiables dans un fichier de texte
#define MENUCHOIX(a, b, c, d, e, f) MenuChoixLinux(a, b, c, d, e, f)
#define MENUFOURCHE(a, b, c, d, e, f, g, h) MenuFourcheLinux(a, b, c, d, e, f, g, h)
#define MENUQUATRE(a, b, c, d, e, f, g, h, i) MenuQuatreLinux(a, b, c, d, e, f, g, h, i)


// #define KEY_ENTER 10

#endif

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13

#define SCRIPT "script.txt"
#define IMAGES "images.txt"
#define MODULES "modules.txt"
#define SAVE "save.txt"
#define OBJETS "objets.txt"
#define COMBATS "combats.txt"

// #define LINESIZE 200
// caractères spéciaux pour lecture passages

#define DOUBLESPEED '/' // multiplie speed par deux
#define HALFSPEED '^'   // divise speed par deux
#define LINEJUMP '|'    // se traduit par un saut de ligne
#define WAITCHAR '`'    // sleep(speed)
#define REFRESHCHAR '#' // ce caractère recharge l'écran avec la même image
#define CHANGEIMAGE '_' // seulement pour lecturescène 2, change l'image affichée par l'image indiquée ex: _101
#define FINPASSAGE '~'  // le lecteur détecte la fin du passage et ferme le fichier
#define ID_END -1

// caractère spécial pour lecture image
#define ENDIMAGE '$'

// caractères spéciaux pour lecture module
#define LIRESCENE 'S'   // une image, puis un texte, finit par --> | format : S i%id_image p%id_passage
#define LIRESCENE2 'Z'  // type "dialogue jeu vidéo", finit par --> | format : Z i%id_image p%id_passage
#define LIREIMAGE 'I'   // lit une image | format : I %id_image
#define LIREPASSAGE 'P' // lit du texte uniquement depuis le fichier de texte | format : P %id_passage
#define CHOIX1 'C'      // ce choix fait lire une variation ou une autre d'une même scène selon le choix
#define CHOIX2 'D'      // ce choix amène à un module différent selon le choix
#define CHOIX3 'Y'      // comme choix 2, mais avec 3 options à la place de 2
#define CHOIX3INSTANT 'X' // comme choix 3 mais affiche l'image instantanément
#define CHOIX4INSTANT 'W' // comme choix 3 mais affiche l'image instantanément
#define CHOIX4 'A' // comme choix 3 mais affiche l'image instantanément
#define EVENT 'E'       // change le statut de l'évènement en '1'. cela permet de savoir si quelque chose s'est produit.
#define OBJET 'O'       // change le statut de l'objet en '0' ou '1' cela permet de retirer ou donner un objet au joueur.
#define CHECKEVENT2 'Q' // verifie si un évenement s'est produit. si oui, envoie vers un module donné
#define CHECKEVENT 'U'  // lit un passage ou un autre selon si un évènement se vérifie
#define CHECKINVENT 'T' // vérifie si un objet est dans l'inventaire. si oui, envoie vers un module donné, sinon renvoie vers un autre.
#define CHECKINVENT2 'H'// vérifie si un objet est dans l'inventaire. si oui, envoie vers un module donné.
#define INVENTAIRE 'K'  // renvoie vers un menu permettant de consulter l'inventaire.
#define RETOUR 'J'      // revient au module précédent. cette fonction n'existe que pour pouvoir consulter l'inventaire lol
#define SAVING 'V'      // sauvegarde la progression
#define GOTOMODULE 'M'  // rembobine le fichier, cherche le module indiqué et commence la lecture | format : M %id_module
#define FINMODULE '$'   // ce symbole ne devrait pas être lu : on passe à un autre module avant. si il est atteint, faire apparaître un message d'erreur, puis quitter le programme
#define RANDOMSCENE 'R' // joue une scène ou une autre selon un tirage aléatoire (à implémenter)
#define FIGHT 'F'       // démarre le combat de numéro donné | format : F 30
#define COMMENTAIRE '#' // le lecteur de module ignorera la ligne à partir de ce caractère
#define EVENTANDEVENT 'N' // vérifie la condition event1 ET event2, et envoie vers un module si la condition est remplie

// caractères spéciaux pour lecture combat
#define PATHOS 'P'
#define LOGOS 'L'
#define STATS 'S'
#define CREDIBILITE 100
#define TAILLEINVENTAIRE 40

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
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    fclose(f);
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
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    fclose(f);
}

// affiche une 'image' depuis le fichier cible
void LireImage(int scene_id, char *filename)
{
    srand(time(NULL)); // initiaise le générateur de nombre aléatoire

    FILE *f;
    f = fopen(filename, "r");

    if (f != NULL)
    {
        int id;

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != scene_id && id != ID_END)
        {
            skipUntilChar(f, ENDIMAGE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        char c = fgetc(f);
        while (c != ENDIMAGE && c != EOF)
        {

            printf("%c", c);
            c = fgetc(f);
        }
        printf("\n--------------------------------------------------------\n\n");
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    fclose(f);
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
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", file_text);
        printf("\n");
    }
    fclose(f);
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

// faire une fonction menu choix pour linux
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

// affiche une scene puis donne un choix.
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
    }

    fclose(f);
}

// si cette fonction est utilisée avec l'inventaire comme paramètre, alors elle vérifie la présence d'un objet dans l'inventaire. renvoie 1 si l'évènement vaut 1.
int CheckForEvent(int id_event, char *events) // renvoie 0(true), si l'event indiqué est dans a sauvegarde, renvoie 1(false) sinon (à implémenter)
{
    int ret;
    if (events[id_event] != '0')
        ret = 1;
    else
        ret = 0;

    return ret;
}

// si cette fonction est utilisée avec "inventaire" à la place de "events" alors cela revient à mettre quelque chose dans l'inventaire (ou retirer, selon la valeur (1 = présent, 0 = absent)
void EventTrigger(int id_event, char *events, char value)
{
    if (value == '0')
    {
        events[id_event] = '0';
    }
    else
        events[id_event] = '1';
}

// lit un fichier et enregistre les valeurs dans des variables données à l'avance
void Load(char *savefile, int *id, char *events, char *inventaire) //, char events[40], char inventaire[40]) // events[40] et inventaire[40]
{
    FILE *f;
    f = fopen(savefile, "r");
    if (f == NULL)
    {
        NouvelleSave(savefile);
    }

    fscanf(f, "%d\n%s\n%s", id, events, inventaire); //, events, inventaire);

    fclose(f);
}

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
    }
    else printf("error, file not found : %s", objets_file);

    fclose(f);
}

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

// renvoie 0 si le combat est perdu, 1 si le combat est gagné
int LireCombat(char *combatfile, int id_combat, char * inventaire)
{
    //printf("start lecturecombat");
    int cred = 0;
    FILE *fc;
    fc = fopen(combatfile, "r");
    if (fc != NULL)
    {
        int b;
        int id;
        int reponse = -1;
        // variables générales du combat
        int damage; // points de vie de l'ennemi, et du joueur
        float ethos ;
        damage = 0;
        // variables pour pathos
        int bonne_reponse = 0;
        char str1[50], str2[50], str3[50], str4[50]; // réponses possibles
        int p0, i0;                                   // passage et image à lire avant de proposer le choix
        int p1, p2, p3, p4;                           // passages à lire pour chaque réponse
        int i1, i2, i3, i4;                           // images à afficher avec le passage pour chaque réponse
        int d1, d2, d3, d4;                           // dégats associés à chaque réponse (pour la bonne réponse, le dégat est infligé à l'ennemi)

        //transformer l'inventaire en un tableau de valeurs.
        int size_invent = TAILLEINVENTAIRE - 1;//sizeof(inventaire)/sizeof(inventaire[0]);
        int taillerelle = 0;
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

        // aller au bon module de comabt
        fscanf(fc, ":%d:", &id);
        skipUntilChar(fc, '\n');
        while (id != id_combat && id != ID_END)
        {
            skipUntilChar(fc, FINMODULE);
            skipUntilChar(fc, '\n');
            fscanf(fc, ":%d:", &id);
            skipUntilChar(fc, '\n');
        }
        //printf("combat trouvé");

        fscanf(fc,"ethos %f", &ethos); // récupérer les stats du combat
        // si ethos est inférieur à 1, cela diminue les dégats. sinon, cela les augmente

        cred = CREDIBILITE;

        char c = fgetc(fc);
        while (c != FINMODULE && c != EOF)
        {
            if (c == PATHOS && cred > 0)
            {
                // fscanf toutes les variables
                fscanf(fc, " i%d p%d\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\nr%d\n", &i0, &p0, str1, &i1, &p1, &d1, str2, &i2, &p2, &d2, str3, &i3, &p3, &d3, str4, &i4, &p4, &d4, &bonne_reponse);
                system(CLEAR);
                LireImage(i0, IMAGES);
                LirePassage(p0, SCRIPT);
                reponse = -1;
                while (reponse != bonne_reponse && cred > 0) // le tour continue tant que le joueur n'a pas trouvé la bonne réponse, ou échoué suffisamment
                {
                    system(CLEAR);
                    LireImage(i0, IMAGES);
                    LirePassageInstantane(p0, SCRIPT);
                    printf("\n--------------------------------------------------------\n");
                    printf("               Credibility : %d%%", cred);
                    printf("\n--------------------------------------------------------\n");
                    printf("\n   1 : | %s |\n   2 : | %s |    \n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
                    char verif = 0;
                    while (!verif)
                    {
                        c = 0;
                        switch ((c = getchar()))
                        {
                        case '1':
                            reponse = 1;
                            verif = 1;
                            damage = d1;
                            i0 = i1;
                            p0 = p1;
                            break;
                        case '2':
                            reponse = 2;
                            verif = 1;
                            damage = d2;
                            i0 = i2;
                            p0 = p2;
                            break;
                        case '3':
                            reponse = 3;
                            verif = 1;
                            damage = d3;
                            i0 = i3;
                            p0 = p3;
                            break;
                        case '4':
                            reponse = 4;
                            verif = 1;
                            damage = d4;
                            
                            i0 = i4;
                            p0 = p4;
                            break;
                        default:
                            puts("\n");
                            system(CLEAR);
                            LireImage(i0, IMAGES);
                            LirePassageInstantane(p0, SCRIPT);
                            printf("\n--------------------------------------------------------\n");
                            printf("               Credibility : %d%%", cred);
                            printf("\n--------------------------------------------------------\n");
                            printf("\n   1 : | %s |\n   2 : | %s |    \n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
                            fflush(stdin);
                        }
                    }
                    while ((c = getchar()) != '\n' && c != EOF){}
                    if (reponse == bonne_reponse)
                    {
                        LireScene2(i0,p0,IMAGES,SCRIPT);
                    }
                    else
                    {
                        cred = cred - (int)(damage*ethos);
                        if (cred < 0)
                            cred = 0;
                        
                        // affiche le résultat
                        system(CLEAR);
                        LireImage(i0, IMAGES);
                        LirePassage(p0, SCRIPT);
                        printf("\n--------------------------------------------------------\n");
                        printf("               Credibility : %d%%", cred);
                        printf("\n--------------------------------------------------------\n");
                    }

                }
            }
            else if (c == LOGOS && cred > 0)
            {
                // scanf les variables : i0, p0, bonne_reponse, i1, p1, d1 (image/passage/degats en cas d'erreur), i2, p2, d2 (image/passage/degats en cas de reussite) et image/ passage en cas d'abandon
                fscanf(fc, " i%d p%d\nx[i%d][p%d][d%d]\no[i%d][p%d][d%d]\n[i%d][p%d]\nr%d\n", &i0,&p0, &i1,&p1,&d1,&i2,&p2,&d2,&i4,&p4,&bonne_reponse);
                system(CLEAR);
                LireImage(i0, IMAGES);
                LirePassage(p0, SCRIPT);
                reponse = -1;
                i = 0;
                
                while (reponse != bonne_reponse && cred > 0) // le tour continue tant que le joueur n'a pas trouvé la bonne réponse, ou qu'il lui reste de la credibilité
                {
                    system(CLEAR);
                    LireImage(i0, IMAGES);
                    LirePassageInstantane(p0, SCRIPT);
                    printf("\n--------------------------------------------------------\n");
                    printf("               Credibility : %d%%", cred);
                    printf("\n--------------------------------------------------------\n                     OBJECT %d :\n",i+1);                    
                    AfficherObjet(true_inventaire[i], OBJETS);
                    printf("\n--------------------------------------------------------\n1 : | Next Object |           2 : | Previous Object |\n3 : | Choose |                   4 : | Yield |\n\nChoix: ");

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
                            while ((c = getchar()) != '\n' && c != EOF){}
                            if (true_inventaire[i] == bonne_reponse)
                            {
                                
                                reponse = bonne_reponse; 
                                LireScene2(i2,p2, IMAGES,SCRIPT);
                            }
                            else
                            {
                                cred = cred - (int)(d1*ethos);;
                                if (cred < 0)
                                    cred = 0;
                                LireScene2(i1,p1, IMAGES,SCRIPT);
                            }
                            //pressEnter();


                            verif = 1;
                            break;

                        case '4':
                            
                            while ((b = getchar()) != '\n' && c != EOF){}
                            verif = 1;
                            cred = 0;
                            //fflush(stdin);
                            //c = getchar();

                            LireScene2(i4,p4, IMAGES,SCRIPT);
                            break;
                            
                        default:
                            puts("\n");
                            system(CLEAR);
                            LireImage(i0, IMAGES);
                            LirePassageInstantane(p0, SCRIPT);
                            printf("\n--------------------------------------------------------\n");
                            printf("               Credibility : %d%%", cred);
                            printf("\n--------------------------------------------------------\n                     OBJECT %d :\n",i+1);                    
                            AfficherObjet(true_inventaire[i], OBJETS);
                            printf("\n--------------------------------------------------------\n1 : | Next Object |           2 : | Previous Object |\n3 : | Choose |                   4 : | Yield |\n\nChoix: ");

                            fflush(stdin);
                        }
                    }
                    
                }
                // à la fin du choix, afficher le résultat
            } else c = fgetc(fc);
        }
        //printf("fin du combat");
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", COMBATS);
        printf("\n");
    }

    fclose(fc);
    //pressEnter();
    if(cred == 0) 
    {
        pressEnter();
        return 0;
    }
    else return 1;
}

// parcourt un fichier texte et éxecute des actions en fonction des caractères rencontrés.
void lireModule(int id_module, char *modules_file, char *images_file, char *text_file, char *events, char *inventaire)
{
    FILE *f;
    f = fopen(modules_file, "r");

    if (f != NULL)
    {
        int id, random, choix;
        int id_event, value_event; // il n'y a probablement pas besoin de déclarer autant de variables, certaines peuvent être réutilisées. à optimiser
        char value;
        int id_module1, id_module2, id_module3, id_module4, id_combat, rez;

        int id_module_precedent = 0; // sauvegarde en mémoire le module précédent.

        char str1[50], str2[50], str3[50], str4[50];
        char chr1,chr2;
        int id_image;
        int id_passage;
        int id_passage1, id_image1, id_passage2, id_image2; // id1 et id2 servent pour les scènes alternatives

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != id_module && id != ID_END)
        {
            skipUntilChar(f, FINMODULE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        char c = fgetc(f);
        while (c != FINMODULE && c != EOF)
        {
            if (c == COMMENTAIRE)
                skipUntilChar(f, '\n');
            else if (c == SAVING)
                Save(SAVE, id_module, events, inventaire);
            else if (c == LIRESCENE)
            {
                fscanf(f, " i%d p%d", &id_image, &id_passage);
                LireScene(id_image, id_passage, IMAGES, SCRIPT);
            }
            else if (c == LIRESCENE2)
            {
                fscanf(f, " i%d p%d", &id_image, &id_passage);
                LireScene2(id_image, id_passage, IMAGES, SCRIPT);
            }
            else if (c == LIREIMAGE)
            {
                fscanf(f, " %d", &id_image);
                LireImage(id_image, IMAGES);
            }
            else if (c == LIREPASSAGE)
            {
                fscanf(f, " %d", &id_passage);
                LirePassage(id_passage, SCRIPT);
            }
            else if (c == CHOIX1)
            {
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] 1 p%d i%d 2 p%d i%d", &id_passage, &id_image, str1, str2, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (MENUCHOIX(str1, str2, SCRIPT, id_passage, IMAGES, id_image) == 1)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == GOTOMODULE)
            {
                id_module_precedent = id_module;
                fscanf(f, " %d", &id_module);

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n');
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX2)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] m%d m%d", &id_passage, &id_image, str1, str2, &id_module1, &id_module2);
                
                if (MENUCHOIX(str1, str2, SCRIPT, id_passage, IMAGES, id_image) == 1)
                    id_module = id_module1;
                else 
                    id_module = id_module2;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX3 || c == CHOIX3INSTANT)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] [%49[^]]] m%d m%d m%d", &id_passage, &id_image, str1, str2, str3, &id_module1, &id_module2, &id_module3);
                if (c == CHOIX3) 
                    choix = MENUFOURCHE(str1, str2, str3, SCRIPT, id_passage, IMAGES, id_image, 0);
                else choix = MENUFOURCHE(str1, str2, str3, SCRIPT, id_passage, IMAGES, id_image, 1);
                
                if ( choix == 1)
                    id_module = id_module1;
                else if (choix == 2)
                    id_module = id_module2;
                else 
                    id_module = id_module3;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX4 || c == CHOIX4INSTANT)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] [%49[^]]] [%49[^]]] m%d m%d m%d m%d", &id_passage, &id_image, str1, str2, str3, str4, &id_module1, &id_module2, &id_module3, &id_module4);
                if (c == CHOIX3) 
                    choix = MENUQUATRE(str1, str2, str3, str4, SCRIPT, id_passage, IMAGES, id_image, 0);
                else choix = MENUQUATRE(str1, str2, str3, str4, SCRIPT, id_passage, IMAGES, id_image, 1);
                
                if ( choix == 1)
                    id_module = id_module1;
                else if (choix == 2)
                    id_module = id_module2;
                else  if (choix == 3)
                    id_module = id_module3;
                else
                    id_module = id_module4;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == EVENT)
            {
                fscanf(f, " e%d v%c", &id_event, &value); // la value est lue comme "zéro ou un". n'importe quoi d'autre que zéro sera lu comme un "1"
                EventTrigger(id_event, events, value);
            }
            else if (c == OBJET)
            {
                fscanf(f, " o%d v%c", &id_event, &value); // la value est lue comme "zéro ou un". n'importe quoi d'autre que zéro sera lu comme un "1"
                EventTrigger(id_event, inventaire, value);
            }
            else if (c == CHECKEVENT)
            {
                fscanf(f, " e%d 0 p%d i%d 1 p%d i%d", &id_event, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (CheckForEvent(id_event, events) == 0)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == CHECKEVENT2)
            {
                id_module_precedent = id_module;
                fscanf(f, " e%d v%d m%d", &id_event,&value_event, &id_module1);
                if (CheckForEvent(id_event, events) == value_event)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c == CHECKINVENT)
            {
                id_module_precedent = id_module;
                fscanf(f, " o%d m%d m%d", &id_event, &id_module1, &id_module2);
                if (CheckForEvent(id_event, inventaire) == 1)
                    id_module = id_module1;
                else
                    id_module = id_module2;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHECKINVENT2)
            {
                id_module_precedent = id_module;
                fscanf(f, " o%d v%d m%d", &id_event,&value_event, &id_module1);
                if (CheckForEvent(id_event, inventaire) == value_event)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c== FIGHT)
            {
                id_module_precedent = id_module;
                fscanf(f, " c%d d%d v%d", &id_combat, &id_module1, &id_module2);
                //printf("\n%d %d %d",id_combat, id_module1, id_module2);
                //pressEnter();
                if(LireCombat(COMBATS, id_combat, inventaire) == 1)
                    id_module = id_module2;
                else 
                    id_module = id_module1;
                
                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == RANDOMSCENE)
            {
                fscanf(f, " %d p%d i%d p%d i%d", &random, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (rand()%100 <= random)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == EVENTANDEVENT)
            {
                fscanf(f, " %c%d AND %c%d m%d", &chr1, &id_event, &chr2, &id_passage1, &id_module1);

                if (chr1 == 'e') id_image1 = CheckForEvent(id_event, events); 
                else if (chr1 == 'o') id_image1 = CheckForEvent(id_event, inventaire);
                if (chr2 == 'e') id_image2 = CheckForEvent(id_passage1, events);
                else if (chr2 == 'o') id_image2 = CheckForEvent(id_passage1, inventaire);

                if(id_image1 && id_image2)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c == INVENTAIRE)
            {
                ConsulterInventaire(inventaire);
            }
            else if (c == RETOUR)
            {
                id_module = id_module_precedent;
                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
                
            }
            // rewind(f) //permet de revenir au début du fichier

            c = fgetc(f);
        }

        printf("\n\nExit program\n");
        pressEnter();
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", modules_file);
        printf("\n");
    }

    fclose(f); // fermeture du fichier modules
}

int main()
{

    SET_CONSOLE_UTF_8

    FILE *file;

    if (file = fopen(SAVE, "r"))
    {
        fclose(file);
        printf("save file exists, loading save");
    }
    else
    {
        printf("save file doesn't exist, creating new save");
        NouvelleSave(SAVE);
    }

    int id_module = 0;
    char *events = malloc(TAILLEINVENTAIRE);
    char *inventaire = malloc(TAILLEINVENTAIRE);

    Load(SAVE, &id_module, events, inventaire);
    printf("|%d|%s|%s\n", id_module, events, inventaire);

    lireModule(id_module, MODULES, IMAGES, SCRIPT, events, inventaire);

    free(events); // possible fuite mémoire si le programme est interrompu, mais négligeable dans ce contexte.
    free(inventaire);

    // system(".txt");

    return 0;
}
