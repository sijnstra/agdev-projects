# Flappy Agon Bird - FlAgon!
This is the [ASCII curses library Flappy Bird game](../abird/) which has been adapted to make the most of the console 8 graphics features allowing characters to be mapped to bitmaps.

This updated version takes advantage of replacing byte characters with bitmaps. It also uses a double-buffered screen mode to reduce flicker.

The double buffered screen mode reduced the available colours from 64 down to 16, which then required remapping the colours to make the right palette available. To fully support this, you'll need to have your Agon updated to at least VDP 2.10.1 for the colours to function as intended.

The result of all this bitmap mapped to characters is that under the hood, the game that makes this:
![Screenshot](./ASCII_bird_Capture.PNG)

is the same game engine as the game that makes this:
![Screenshot](./Flagon_Capture.PNG)

# Compiling
Use AgDev C compiler and simply take `make` from the directory with the makefile.

# Known Issues
Flicker has been resolved although smoother scrolling and sound effects would be nice.
