# Gamebook engine
I wanted to make a game where the player gets to make a great variety of choices, but I didn't want to have to handle an infinite else-if tree...
That's how this engine was born.

This project is inspired by [gamebooks](https://en.wikipedia.org/wiki/Gamebook) 
The `module.txt` file will tell the program what module to read. In a gamebook you would have something like "if you go right then go to page 14; if you decide to head to the left then go to page 32.". The `module.txt` file does essentially the same.

I think an example is the best way to demonstrate the principle of this (modest) game engine, so I will show you the absolute basics to make your own adventure

## A basic adventure

Let's make a simple adventure. It will have the following structure :
you are an adventurer on a road and you get to a fork in the path. If you go right you die instantly, killed by a lion. If you go left you find a cool treasure.

let's put it in practice.
first, we'll write the next needed for the adventure :



### script.txt
```
:1: intro
You are an adventurer. You are in quest of glory and trasure ! You are walking on a path, waiting for adventure to meet you. You stop as you come accross a fork in your path. Will you go right or go left ?
~
:2: right - instant death
You head right, confident in your sense of direction. Suddenly, you hear something rusting in the bushes. Before you can react, a lion jumps out from the side of the road and kills you instantly. You die.
~
:3: left - cool treasure
You head left. On the road you stumble upon an old-looking chest. You open it and see that it is filled with treasure. Hooray ! You can retire and leave this dangerous life behind you !
~
:4: "the end"
THE END
~
:-1: end of file marker
```
Then, we'll want to add images to provide a visual for the player. This is not necessary for the game, but a nice touch nonetheless.

### images.txt

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
:2: chest (treasure)



        ___________________
       |\                  \
       |\\ _________________\
       | \|_________________|
       |  |       [__]      |  <------- treasure inside
        \ |                 |
         \|_________________| 
          

$
:-1: end of file
```

As you can see, the file follows a certain format. The end of the image is detected when the engine reads the '$' character. This can be easily modified in the source code.

Now, we will edit the "module" file so that the program executes the correct actions.
Let's put it all together !

### module.txt
```
:0: intro

# Gives the player a choice.
# Writes entry 1 from script and displays image 0.
# Goes to module 1 or 2 depending on the choice.

D p1 i0 [LEFT] [RIGHT] m1 m2
$
:1: left (treasure)

# Reads from the script and displays an image

Z i2 p3

# after, go to module 3

M 3
$
:2: right (death)
Z i1 p2
M 3 
$
:3: the end

# just reads from the script file, no image
P 4 
$
:-1: end of file marker
```

Alright, let's compile ! The ideal environment is Linux. I made adjustments so that the code can be compiled on Windows, and it works well but it hasn't been tested as extensively as in Linux.

To compile, simply enter the command `gcc general.c -o GAME`
If you do not have gcc, then install it or use any other tool you might prefer.

Then you can click on it, or execute it from the terminal directly.

### What it looks like 

In windows :
![alt text](./screenshots/1.png "screenshot 1")

![alt text](./screenshots/2.png "screenshot 1")




