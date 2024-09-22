
#ifndef PIECES_H
#define PIECES_H

enum COLOR     {WHITE = 1,BLACK,RED_W,RED_B};
enum DIRECTION {UP,RIGHT,DOWN,LEFT};

struct PIECE {
    int x;
    int y;
    enum COLOR color;
    char icon;
    enum DIRECTION dir;
};

typedef enum   COLOR     COLOR;
typedef struct PIECE     PIECE;
typedef enum   DIRECTION DIRECTION;

#endif

