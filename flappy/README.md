# flappy-curses
Ported from Scrooge2020's Flappy Bird game using Ncurses, written in C for Linux. Original Repo [https://github.com/Scrooge2020/flappy-curses]

# Changes required
Minimal changes were required for this onem which means the `acurses.h` library is getting some reusability already.

The main changes were:
<li>replacing `getch()` with `wgetch()` to use the non-blocking version, otherwise the screen is only updated when you press a key.</li>
<li>adding a small delay before erasing the screen using `napms(10)` to emphasize the time the screen is available.</li>

Don't forget you can edit constants.h to your liking.

## Screenshot
![screenshot](https://github.com/sijnstra/agdev-projects/blob/main/flappy/cursed_flappy_Capture.PNG)
