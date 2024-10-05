# Shawn Sijnstra's Agon AgDev C development projects
This is a collection of games that have been ported to the Agon Light platform, built with [AgDev C](https://github.com/pcawte/AgDev) and keeping code changes to a minimum. This meant the creation of 2 libraries so far, `getopt.h` and a minimalist version of curses which I've called `acurses.h`.

If you have an interest in z80 assembler, check out my [Agon z80 repo](https://github.com/sijnstra/agon-projects/).

These are the games that have been ported so far:
<li>ASCII Bird</li>
<li>Apple War</li>
<li>Flappy Bird</li>
<li>Galaxis</li>
<li>Minefield</li>
<li>Simple Journey</li>
<li>UE14500</li>

# ASCII Bird
[abird.bin](https://github.com/sijnstra/agdev-projects/tree/main/abird) - A different implementation of that classic one button game made using plain ASCII characters.

# Apple War
[awar.bin](https://github.com/sijnstra/agdev-projects/tree/main/apple) - A programming exercise that made a pleasant game to test the curses library, using plain ASCII characters with colour.

# Flagon - Flappy Agon Bird
[flagon.bin](https://github.com/sijnstra/agdev-projects/tree/main/flagon) - the ASCII bird game modifieid to use bitmaps that have been mapped to text characters, so still using the curses libraru and the same game engine.

# Flappy Bird
[flappy.bin](https://github.com/sijnstra/agdev-projects/tree/main/flappy) - The classic one-button game using only plain ASCII characters.

# Galaxis
[galaxis.bin](https://github.com/sijnstra/agdev-projects/tree/main/galaxis) - Find interstellar lifeboats in a starfield in this curses-based clone of the nifty little Macintosh freeware game Galaxis.

# Langton ant
[ant.bin](https://github.com/sijnstra/agdev-projects/tree/main/Langton) - simple implementation of Langton's ant in C using ncurses, optimised for Agon.

# Minefield
[amines.bin](https://github.com/sijnstra/agdev-projects/tree/main/mines) Classic minefield game. Modifies the font for flags and mines, using bitmaps and font replacement.

# Simple Journey
[journey.bin](https://github.com/sijnstra/agdev-projects/tree/main/simple) is a 2D terrain simulation in text format.

# UE14500
[UE14500](https://github.com/sijnstra/agdev-projects/tree/main/ue14500) is a tube computer emulator, along with an assembler also provided. There's a lot to explain on this one, so there are mainly links to what it is from the original authors.

# UE14500-asm
[UE14500-asm](https://github.com/sijnstra/agdev-projects/tree/main/ue14500) is the matching source and makefile to compile the assembler for UE14500 yourself. This is provided courtesy of Turboss.

# Agon curses - acurses.h
The curses library is a commonly found POSIX/UNIX library (with multiple variations) to use as a common way to take advantage of advanced terminal capabilities without needing to heavily tailor each program.

I've created from scratch `acurses.h` to cover a large enough amount of the curses library (it's enomous!) to mines to work. The intent here is to keep developing and make it more integrated with the AgDev compiler once it's more complete and stabilised.

New features since initial release include being able to use the `box()` function, non-blocking `wgetch()` and moving the font tweaks into the library. Additional text manipulation features, more correct use of types, and other bonus features are now in version 0.3. The current version is in the UE14500 src directory.

Bottom line: `acurses.h` is very much BETA and needs work, including getting more to work as well as heavily optimizing (in pre-defined assembler).

# Agon getopt.h
I've adapted another `getopt.h` to work well enough that it allows you to get options on both real hardware and the emulator. This again is an early version, and similarly will need a lot more testing before integrating properly.

# Contributing and Joining in the fun
Help is certainly welcome in developing the libraries as well as developing with the libraries.

Feel free to jump on the AgDev link above or help out here with the extra libraries.

Also join in the fun on the Discord [Agon & Console 8 Community](https://discord.gg/2EqfBmWUyz) server.
