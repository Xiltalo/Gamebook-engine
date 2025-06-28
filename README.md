# How to use

## 1 - let's make a basic adventure

To make an adventure, you'll need at the very least two text files:
`script.txt` and `modules.txt`

Let's explain what these files do.

### modules.txt

This project is inspired by [gamebooks](https://en.wikipedia.org/wiki/Gamebook) 
The `module.txt` file will tell the program what module to read. In a gamebook you would have something like "if you go right then go to page 14; if you decide to head to the left then go to page 32.". The module.txt does essentially the same.

### script.txt

This file contains the text of the adventure.

### Ok let's just have an example.

Let's make a simple adventure. It will have the following structure :
you are an adventurer on a road and you get to a fork in the path. If you go right you die instantly, killed by a lion. If you go left you find a cool treasure.

let's put it in practice.
first, we'll write the next needed for the adventure :

script.txt
```
:1: intro
You are an adventurer. You are in quest of glory and trasure ! You are walking on a path, waiting for adventure to meet you. You stop as you come accross a fork in your path. Will you go right or go left ?
~
:2: right - instant death
You head right, confident in your sense of direction. Suddenly, you hear something rusting. Suddenly, a lion jumps out from the side of the road and kills you instantly. You die.
~
:3: left - cool treasure
You head left. On the road you stumble upon an old-looking chest. You open it and see that it is filled with treasure. Hooray ! You can retire and leave this dangerous life behind you !
~
:4: "the end"
THE END
~
:-1: end of file marker
```

Now, you want to edit the module file so that the program executes the correct actions.

module.txt
```
:0: intro
D p0 i0 [LEFT] [RIGHT] m1 m2
$
:1: left
P 3
P 4
$
:2: right
P 2
P 4
$
:-1: end of file marker
```

Actually there is another file you need, the image.txt file. It stores ASCII images to be used. For this example, we don't have to put any image inside, but we do have to at least create it :

images.txt
```
:0: null image
$
:-1: end of file
```
