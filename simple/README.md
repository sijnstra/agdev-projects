# Simple Journey
Simple console game. \ O /

This is a simple console game that draws the generated landscape using ASCII characters. 

  -> To move, click: A | D.
  -> To exit: Esc.
  
This game can be started on Linux and Windows.

# Manual
Link to the repo: https://github.com/Laynholt/simple_journey.git

For Linux:
  1) git clone https://github.com/Laynholt/simple_journey.git
  2) cd simple_journey/src
  3) gcc -o simple.out simple.c config.c functions.c kbhit.c
  4) clear
  5) ./simple.out

For Window you can clone this project on the Visual Studio project.

# Linux version
![Screen-466](https://user-images.githubusercontent.com/41357381/130327842-42a8541e-9abb-4fd6-abdc-8bcbc9d29482.png)


# Windows version
![Screen-467](https://user-images.githubusercontent.com/41357381/130327852-937d6a2b-9f82-4d77-b2fe-c46530cc2397.png)

# Special thanks
Used repos:
  1) https://github.com/lihexali/linux-kbhit  //  implements GetAsyncKeyState() on Linux.
