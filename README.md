Graphics Assignment-1 Pacman-Killer
===================================

Pacman-Killer in OpenGL 3.

Prathyakshun Rajashankar - 20161107
===================================

This program contains code for the game "Pacman-Killer"

For running the game:
* cd into the build directory
* cmake ../
* make
* run the executable file

Controls:
=========

A – Move player left
D – Move player right
SPACE – Jump
LeftArrow – Pan the screen left
RightArrow – Pan the screen right
UpArrow – Pan the screen up
DownArrow – Pan the screen down
Mouse Scroll – Zoom in and Zoom out
Mouse Drag left – Move player left
Mouse Drag right – Move player right


Rules:
======

The player ball gets points on bouncing on top of the flying balls. Scoring is done based on the size of the ball, presence of plank as stated in the scoring section.

The player loses points on hitting the porcupines and jumps upwards. The porcupine disappears on attacking the player.  There are 5 porcupines initially. Once all have disappeared, 5 more get spawned. 

When the player jumps on the trampoline, he gets a higher boost in the air.

The pond slows down the player and player follows circular motion inside the pond. Jumping is slower.

Magnets attract the player ball towards itself along with some acceleration.


Levels:
=======
Porcupines and 1 magnet come from Level 2 onwards.
2 magnets come from level 3 onwards.

Each level i requires i*300 points in 60 secs to be successfully completed. Following completion, the player is directed towards the next level. Failing to complete the level results in GAME OVER.

Timer in title bar and scoring done on the screen


Scoring:
========
Bouncing on the flying balls gets points.

The player ball gets an extra boost and bounces upwards upon hitting a ball

There are 3 types of balls:
smallest size = +10
medium size = +20
largest size = +30

Flying balls may have planks on them. The player ball bounces following the laws of reflection upon hitting the balls with plank.
Smallest size + plank = +20
medium size + plank = +40
largest size + plank = +60

Porcupines keep spawning and moving. Hitting a porcupine causes the player ball to bounce and gives penalty in the score of (level*50) points
Porcupine = -(level*50)


Features
--------

* Moving porcupines
* 2 magnets in opposite directions attracting when player within its range
* Porcupines which induct penalty in score
* Environment following physics rules


Extra Features
--------------

* Rainbow colored player ball
* Unlimited levels, unlimited gameplay (infinite boundary)
* Display score on the screen
* Time and Target on title bar


License
-------

Copyright &copy; 2018 Prathyakshun Rajashankar <prathraja@gmail.com>
