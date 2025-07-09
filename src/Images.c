#include <Images.h>
#include <Scripts.h>

#include <stdio.h>


// affiche une 'image' depuis le fichier cible
void LireImage(int scene_id, char *filename)
{    
    FILE *f;
    f = fopen(filename, "r");

    if (f != NULL)
    {
        int id;

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != scene_id && id != ENDIMAGE)
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
        fclose(f);
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", filename);
        printf("\n");
    }
    
}
