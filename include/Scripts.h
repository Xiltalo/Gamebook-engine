#ifndef SCRIPTS_H
#define SCRIPTS_H

#include <Images.h>
#include <stdio.h>


#ifdef _WIN32
#include <Windows.h>
#define SLEEP(a) Sleep(a)
#define SET_CONSOLE_UTF_8 SetConsoleOutputCP(CP_UTF8);
#define CLEAR "cls"
#define READING_SPEED 35 // le temps de latence entre chaque caractère affiché
#define GETCH getch

#else
#include <unistd.h>
#define GETCH getchar // won't be used anyways
#define SET_CONSOLE_UTF_8
#define SLEEP(a) usleep(a)
#define CLEAR "clear"
#define READING_SPEED 35000 // le temps de latence entre chaque caractère affiché // idée : ces paramètres devraient être modifiables dans un fichier de texte

#endif

// special characters for script reading

#define DOUBLESPEED '/' // multiplies delay by 2
#define HALFSPEED '^'   // divides delay by 2
#define LINEJUMP '|'    // does a line jump
#define WAITCHAR '`'    // sleep(delay). pauses for the duration of the delay
#define REFRESHCHAR '#' // Prompts the user to press enter before displaying the rest. Refreshes the image if there is one
#define CHANGEIMAGE '_' // Only if there is an image, changes the image. syntax : _14 to change the displayed image to image 14. the change will appear when the image is refreshed.
#define FINPASSAGE '~'  // the engine stops reading and closes the file.
#define ID_END -1 // the the engine reads a segment of id "-1" it will display an error message.

// lit une ligne d'un fichier, de longueur définie
char* getLine(FILE *f, int linesize);
// lit des caractères d'un fichier cible jusqu'à arriver à un certain caractère
void skipUntilChar(FILE *filestream, char c);
// demande à l'utilisateur d'appuyer sur entrée (n'accepte pas d'autre touche)
void pressEnter();
// lit un texte depuis le ficher indiqué, avec un certain rythme
void LirePassage(int scene_id, char *filename);
// affiche instntanément un texte depuis le fichier indiqué. ignore les indicateurs de vitesse.
void LirePassageInstantane(int scene_id, char *filename);
// lit une scène avec une image statique, comme un dialogue de jeu vidéo. finit par ---> et pressenter à modifier selon choix esthétiques
void LireScene2(int id_image, int scene_id, char *file_image, char *file_text);
// affiche une image, puis lit un texte. finit par ---> et pressenter
void LireScene(int id_image, int id_passage, char *imagefile, char *textfile);



#endif