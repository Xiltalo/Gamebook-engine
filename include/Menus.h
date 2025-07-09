#ifndef MENUS_H
#define MENUS_H

#include <Scripts.h>

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

// contantes des menus
#define TAILLEINVENTAIRE 40

//menus pour windows

// renvoie 1 ou 2 selon le choix. affiche une image et un texte avant le choix
int MenuChoix(char * str1, char * str2, char *textfile, int id_text, char *imagefile, int id_image);
// comme MenuChoix, mais propose 3 choix
int MenuFourche(char * str1, char * str2, char * str3, char *textfile, int id_text, char *imagefile, int id_image, int instantane);
//Menu choix qui propose 4 options
int MenuQuatre(char * str1, char * str2, char * str3, char * str4, char *textfile, int id_text, char *imagefile, int id_image, int instantane);


// Menus pour Linux

//choix 2 options pour linux
int MenuChoixLinux(char * str1, char * str2, char *textfile, int id_text, char *imagefile, int id_image);
// affiche une scene puis donne un choix avec 3 options.
int MenuFourcheLinux(char * str1, char * str2, char * str3, char *textfile, int id_text, char *imagefile, int id_image, int instantane);
// menu 4 options pour linux
int MenuQuatreLinux(char * str1, char * str2, char * str3, char * str4, char *textfile, int id_text, char *imagefile, int id_image, int instantane);


// Gestion de la sauvegarde

// crée une sauvegarde complètement vierge (système de sauvegarde à implémenter et à préciser)
void NouvelleSave(char *savefile);
// sauvegarde une certaine valeur dans le fichier de sauvegarde (système de sauvegarde à implémenter et à préciser)
void Save(char *savefile, int id_module, char *events, char *inventaire);
// lit un fichier de sauvegarde et charge les informations appropriées en mémoire
void Load(char *savefile, int *id, char *events, char *inventaire);


// Menu Inventaire

//Lit une entrée du fichier objet et affiche une représentation formatée de l'information
void AfficherObjet( int id_objet, char* objets_file);
// Un menu pour consulter l'inventaire
void ConsulterInventaire(char * inventaire);


#endif
