# amines - Minefield in C
This is an implementation of the classic Minefield games, written in C. I started with Brian Raiter's [cmines](https://github.com/BR903/cgames), using [Agdev](https://github.com/pcawte/AgDev) to compile. This very quickly became an exercise in writing a `getopt.h` library, as well as a substitute for `curses`.

You can compile yourself using AgDev. Once in your AgDev environment, you can use `make` from the same directory as the `makefile`.

# gameplay and features
Put the `amines.bin` file in your `\bin` directory to run from there. When you run, it will automatically create a configuration file if you don't have one. You can see the command line options using `acurses -h`.

Within the game you can use `?` to get help.

The game provides some examples of using the Agon native C libary featuers as well as abstracting them via the acurses library.

## configuration file
You can specify a config file using `-s filename` at the prompt. The default file is .aminesrc in the current directory. The file allows you to add additional levels, or change the currently available levels.

Each line of this file should be in the following format:
<li> "NAME: WIDTH HEIGHT MINES SCORE"
<li> NAME -
The name of the configuration. Every configuration must begin with a unique letter. (It is also inadvisable to begin a configuration with the letters n or q.)
<li> WIDTH - The width of the playing field.
<li> HEIGHT - The height of the playing field.
<li> MINES - The number of mines to locate.
<li> At the end of the line, your best time for that configuration is stored.

# Agon curses - acurses.h
The curses library is a commonly found POSIX/UNIX library (with multiple variations) to use as a common way to take advantage of advanced terminal capabilities without needing to heavily tailor each program.

I've created from scratch `acurses.h` to cover a large enough amount of the curses library (it's enomous!) to mines to work. The intent here is to keep developing and make it more integrated with the AgDev compiler once it's more complete and stabilised.

Bottom line: `acurses.h` is very much BETA and needs work, including getting more to work as well as heavily optimizing (in pre-defined assembler).

# Agon getopt.h
I've adapted another `getopt.h` to work well enough that it allows you to get options on both real hardware and the emulator. This again is an early version, and similarly will need a lot more testing before integrating properly.

# Contributing and Joining in the fun
Help is certainly welcome in developing the libraries as well as developing with the libraries.

Feel free to jump on the AgDev link above or help out here with the extra libraries.

Also join in the fun on the Discord [Agon & Console 8 Community](https://discord.gg/2EqfBmWUyz) server.

# Other learnings and to do items
The AgDev C libraries have a lot of quirks that need levelling out to make things more standard, and make these conversions easier. For example:
<li> file operations seems to ignore EOF on real hardware (behaviour is different on the emulator).
<li> time functions appear to not work, or work differently. This needs fixing in this game.
<li> the characters used for framing need updating to custom characters or I need to switch to Terminal mode.
<li> one consideration is that Terminal mode will make it a lot easier to apply more attributes to characters.

# Thanks
<li>cmines was originally is copyright (C) 2000 by Brian Raiter <breadbox@muppetlabs.com> and shared under GPL.
<li>Paul Cawte and everyone who has contributed to AgDev.
<li>Steve Sims for leading the charge on making a lot of improvements to the VDP on Agon Light and Console 8.
