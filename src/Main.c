#include <Modules.h>
#include <Scripts.h>
#include <Images.h>
#include <Combats.h>
#include <Menus.h>

#include <stdlib.h>
#include <stdio.h>

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

    free(events);
    free(inventaire);

    // system(".txt");

    return 0;
}