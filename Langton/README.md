# Langton-ant
simple implementation of Langton's ant in C using ncurses, optimised here for Agon AgDev and my acurses library.

from [Wikipedia](https://en.wikipedia.org/wiki/Langton%27s_ant):

Squares on a plane are colored variously either black or white. We arbitrarily identify one square as the "ant". The ant can  travel in any of the four cardinal directions at each step it takes. The "ant" moves according to the rules below:

* At a white square, turn 90° clockwise, flip the color of the square, move forward one unit
* At a black square, turn 90° counter-clockwise, flip the color of the square, move forward one unit

![screenshot1](/screenshots/ant_screenshot.png)
