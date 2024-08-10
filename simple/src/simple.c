#include "libraries.h"
#include "functions.h"


// Для линукса
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)

// Подключаем библиотеку для обработки клавиш в линукс
#include "kbhit.h"

static sig_atomic_t end = 0;
static void sighandler(int signo)   { end = 1;}

int main()
{
    bool exit = false;

    // Обработчик сигналов для клавиш
    term_setup(sighandler);

    // Создание и генерация карты
    create_map();
    move(STAND);

    while (!exit)
    {
        // Обработчик клавиш в Линукс
        if(kbhit())
        {
            if(keydown(KEY_ESC))
                exit = true;
            else if (keydown(KEY_D))
                move(RIGHT);
            else if (keydown(KEY_A))
                move(LEFT);            
        }

        usleep(15000);
    }

    // Обработчик сигналов для клавиш (Выкл)
    term_restore();

    destruct_map();
    printf("\x1b[%dJ", 2);
    
    return 0;
}

// Для Виндовса
#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

void create_console()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
        printf("Handle error.");

    SMALL_RECT rectWindow = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

    // Set the size of the screen buffer
    COORD coord = { SCREEN_WIDTH, SCREEN_HEIGHT + 5 };
    if (!SetConsoleScreenBufferSize(hConsole, coord))
        printf("SetConsoleScreenBufferSize");

    // Assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(hConsole))
        printf("SetConsoleActiveScreenBuffer");

    // Get screen buffer info and check the maximum allowed window size. Return
    // error if exceeded, so user knows their dimensions/fontsize are too large
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        printf("GetConsoleScreenBufferInfo");
    if (SCREEN_HEIGHT + 5 > csbi.dwMaximumWindowSize.Y)
        printf("Screen Height");
    if (SCREEN_WIDTH > csbi.dwMaximumWindowSize.X)
        printf("Screen Width");

    // Set Physical Console Window Size
    rectWindow.Left = rectWindow.Top = 0;
    rectWindow.Right = SCREEN_WIDTH - 1;
    rectWindow.Bottom = SCREEN_HEIGHT + 4;

	if (!SetConsoleWindowInfo(hConsole, TRUE, &rectWindow))
        printf("SetConsoleWindowInfo");

    SetConsoleTitle(L"Simple Journey");
}

int main()
{
    bool exit = false;

    // Создание консоли опеределенных размеров
    create_console();

    // Создание и генерация карты
    create_map();
    move(STAND);

    while (!exit)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            exit = true;
        else if (GetAsyncKeyState('D') & 0x8000)
            move(RIGHT);
        else if (GetAsyncKeyState('A') & 0x8000)
            move(LEFT);

        Sleep(50);
    }

    destruct_map();
    system("cls");

    return 0;
}

#else
//Agon version

int main()
{
    bool exit = false;
    uint8_t vk;

    // Создание и генерация карты
    vdp_mode( 18 );
    vdp_clear_screen();
    vdp_logical_scr_dims( false );
    vdp_cursor_enable( false );
    create_map();
    move(STAND);

    while (!exit)
    {
        vk = getsysvar_vkeycode();
        switch (vk){
        case 0x7d:
            exit = true; break;
        case 0x9c:
            move(RIGHT); break;
        case 0x9a:
            move(LEFT); break;
        }

    }

    destruct_map();
    vdp_cursor_enable( true );

    return 0;
}

#endif
