# Detailed Documentation

This file will explain the syntax and usage of each main file necessary for the engine to run.

## images.txt

```
:0: placeholder image










$
:1: death
              __________
            /            \
          /                \
         |                  |
         | ' ___      ___ ' |
         |  |___/    \___|  |
         \|       /\       |/
           \     |__|     /        <-------- you died lol
            ||          ||
            \[][][][][][]/
             \__________/
$

```

Each image starts with `:x:` with `x` being the id of the image. If multiple images have the same id, the first one in the file will be read. 
The description of the image is not necessary but recommanded for comfort.
The engine will know to stop reading the image when it encounters the `$` character.

## script.txt


```
:18: Three doors. middle door -- instant death.
You use the key to open the door and go through confidently.#Unfortunately, there is no floor and you fall to your death._1#Sorry.
~
```
Similar to the `images.txt` file, the text segments start with an id marker and an optional description. they must end with the `~` character.

There are several **special characters** that influence how the text is displayed and at what speed it is read.

The text is displayed to the user character by character. There is a set delay between each character. The shorter this delay, the faster the text displays.

from the `general.c` file :
```
#define DOUBLESPEED '/' // multiplies delay by 2
#define HALFSPEED '^'   // divides delay by 2
#define LINEJUMP '|'    // does a line jump
#define WAITCHAR '\`'    // sleep(delay). pauses for the duration of the delay
#define REFRESHCHAR '#' // Prompts the user to press enter before displaying the rest. Refreshes the image if there is one
#define CHANGEIMAGE '_' // Only if there is an image, changes the image. syntax : _14 to change the displayed image to image 14. the change will appear when the image is refreshed.
#define FINPASSAGE '~'  // the engine stops reading and closes the file.
#define ID_END -1 // the the engine reads a segment of id "-1" it will display an error message.
```

The syntax here is very simple. Any of the above characters will be read and not displayed. You can place them anywhere you like and they will produce an effect.
For example, consider the following sentence :

"Hello \`\`\`\` ...Goodbye."

The engine will read "Hello" then pause for a duration of four times the reading delay, before printing the "Goodbye".
For examples of the CHANGEIMAGE character, see the `script.txt` of the Example Adventure.

## modules.txt

This is by far the most complex one, because of the wide number of functions, and the less-than-friendly syntax
Each module starts with an id, like in `images.txt` and `script.txt`. A module ends with the "$" character. the next module :id: must be on the very next line after the "$" character.
In most places, you can add commentaries for your file. a commentary starts with the "#" character. It is useful to reread yourself and work more efficiently. Also useful if you have someone else review your modules.

### Go to a module

The first and simplest function tells the engine to go read a module.

Syntax :
```
M %id_module
```
Exemple :
```
M 3
```

### Read text

There are several functions for reading text, but only this one is really worth using because it implements the posibility to refresh the displayed image and read a text segment bit by bit with the special character "#"

Syntax : 
```
Z i%id\_image p%id\_passage
```
Example:
```
Z i1 p2
```

### Choices

These functions display a choice menu to the user.

This one displays a different set of text+image depending on the choice.
\#define CHOIX1 'C'
Syntax:
```
C p%id_text_prompt i%id_image_prompt [text choice 1] [text choice 2] 1 p%id_text_if_choice_1 i%id_image_if_choice_1 2 p%id_text_if_choice_2 i%id_image_if_choice_2
```
Example:
```C p100 i10 [YES] [NO] 1 p102 i10 2 p101 i10
```

These ones point to a different module depending on the choice.
#### Two options
Syntax:
```
D p%id_script i%id_image [%text_choice1] [%text_choice2] m%id_module_if_choice_1 m%id_module_if_choice_2
```
Example:
```
D p341 i10013 [SEE BLENDER] [SEE FRIDGE] m24 m25
```

#### Three options
Example:
```
Y p341 i10013 [SEE BLENDER] [SEE FRIDGE] [GET OUT] m24 m25 m17
```

#### Four options
```
A p341 i10013 [SEE BLENDER] [SEE FRIDGE] [GET OUT] [SEE INVENTORY] m24 m25 m17 m77
```

These ones do the same as their non-instant counterpart, but they read the text without any delay between characters.

'X' // 3 options, instant display.
'W' // 4 options, instant display.

### Random-based functions

Reads a text+image section depending on a number randomly generated between 0 and 100.
Example: 
```
R 50 p617 i30 p622 i33
```
Reads the first set if the number is below 50, the second set if it is above.

Goes to a module or another depending on a random number.
```
L 15 m13 m12
```
Goes tp the first module if the number is below 50, the second one if it is above.

The random number generator is seeded with the time at which the program starts. It isn't truly random since it is time-based, but it also doesn't really *need* to be.

### set events and object values 

#### Set the value of an event

An event can only take 1 or 0 as a value. 1 is true, 0 is false. 
Syntax:
`E e%id_event v%value_event`
Example:
`E e62 v1`

#### Set the value of an object

An object can only take 1 or 0 as a value. 1 is true (in inventory), 0 is false (not in inventory). 
Syntax:
```
E e%id_object v%value_object
```
Example:
```
O o17 v0
``` 
you remove object 17 from the inventory

### Check for event values

Goes to module m if event e has the value v.
Example:
```
Q e14 v1 m87
```

Reads a text+image section or an other depending on if an event is false or true.
Exemple: 
```
U e13 0 p200 i61 1 p201 i61
```

Sends to module m if the two events e are true. 
Example:
```
N e15 AND e17 m112
```

### Check for inventory values

Goes to module m if object o has the value v
Example:
```
Q e14 v1 m87
```

### Inventory management

#### Access the inventory menu

The inventory menu is a menu that lets the player view which objects are in the inventory.
This Menu only works if the `objets.txt` file is in the directory. For more information about this file, see the appropriate section.
The easiest method is to have a dedicated "inventory module" in the modules.txt file :


``` 
:77: inventory module
K # access the inventory menu. 
J # sends you back to the previously loaded module when you quit the inventory menu
$
```

## objets.txt

This file stores the name and description of the objects. See `objets.txt` in the repository for an example.

```
:0:Key;Found in a chest. What does it open ?.$
:-1:
```

The format for each object is: 

`:id:name;description$`

The name cannot be more than 100 characters. The description must be below 500 characters.

