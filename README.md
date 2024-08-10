# Shawn Sijnstra's Agon AgDev C development projects
This is a collection of games that have been ported to the Agon Light platform, keeping code changed to a minimum. This meant the creation of 2 libraries so far, `getopt.h` and a minimalist version of curses which I've called `acurses.h`.

These are the games that have been ported so far:
<li>Apple War</li>
<li>Flappy Bird</li>
<li>Minefield</li>
<li>Simple Journey</li>

# Apple War
`awar.bin` - A programming exercise that made a pleasant game to test the curses library.

# Flappy Bird
`flappy.bin` The classic one-button game.

# Minefiled
`amines.bin` Classic minefield game. Modifies the font for flags and mines.

# Simple Journey
`journey.bin` is a 2D terrain simulation in text format.

# Agon curses - acurses.h
The curses library is a commonly found POSIX/UNIX library (with multiple variations) to use as a common way to take advantage of advanced terminal capabilities without needing to heavily tailor each program.

I've created from scratch `acurses.h` to cover a large enough amount of the curses library (it's enomous!) to mines to work. The intent here is to keep developing and make it more integrated with the AgDev compiler once it's more complete and stabilised.

New features since initial release include being able to use the `box()` function, non-blocking `wgetch()` and moving the font tweaks into the library.

Bottom line: `acurses.h` is very much BETA and needs work, including getting more to work as well as heavily optimizing (in pre-defined assembler).

# Agon getopt.h
I've adapted another `getopt.h` to work well enough that it allows you to get options on both real hardware and the emulator. This again is an early version, and similarly will need a lot more testing before integrating properly.

# Contributing and Joining in the fun
Help is certainly welcome in developing the libraries as well as developing with the libraries.

Feel free to jump on the AgDev link above or help out here with the extra libraries.

Also join in the fun on the Discord [Agon & Console 8 Community](https://discord.gg/2EqfBmWUyz) server.
