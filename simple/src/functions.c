#include "functions.h"

char** memory_allocate()
{
    uint32_t worldMapSize = worldMap.iMapHeight * sizeof(char*) + worldMap.iMapSize * sizeof(char);
    uint32_t mapOfHeightsSize = worldMap.iMapWidth * sizeof(uint16_t);
    uint32_t mapOfCloudsSize = 0;

    worldMap.iMapFullSize = worldMapSize + mapOfHeightsSize + mapOfCloudsSize;

    char** ptr = (char**)malloc(worldMap.iMapFullSize);
    return ptr;
}

void create_map()
{
    worldMap.iMapSize = worldMap.iMapWidth * worldMap.iMapHeight;

    char** ptr = memory_allocate();
    uint16_t* ptrHeight;

    // Операции с массивом map
    char* start = (char*)ptr + worldMap.iMapHeight * sizeof(char*);
    
    for(uint16_t i = 0; i < worldMap.iMapHeight; i++)
    {
        ptr[i] = start + i * worldMap.iMapWidth;
    }
    clear_map(ptr, 0);

    // Операции с массивом высот
    ptrHeight = (uint16_t*)((char*)ptr + worldMap.iMapHeight * sizeof(char*) + worldMap.iMapSize * sizeof(char));


    // Карта уже существует, делаем ресайз
    if(worldMap.map != NULL)
    {
        // Перенос данных из старой в новую Map
        for(uint16_t i = 0; i < worldMap.iMapOldHeight; i++)
        {
            memcpy(ptr[i], worldMap.map[i], worldMap.iMapOldWidth);
        }

        // Копируем карту высот
        memcpy(ptrHeight, worldMap.mapOfHeights, sizeof(uint16_t) * worldMap.iMapOldWidth);

        destruct_map();
   }
    
   worldMap.map = ptr;
   worldMap.mapOfHeights = ptrHeight;

   worldMap.iMapOfHeightsSize = worldMap.iMapWidth;

   // Вызываем функции генерации окружения
   generate();

   worldMap.iMapOldHeight = worldMap.iMapHeight;
   worldMap.iMapOldWidth = worldMap.iMapWidth;
}

void destruct_map()
{
    free(worldMap.map);
}

void clear_map(char** map, int16_t numOfFrame)
{
    memset(map[numOfFrame], ' ', worldMap.iMapSize);
//    memset(map[numOfFrame], '.', worldMap.iMapSize / 2);
    memset(map[worldMap.iMapHeight / 2], '=', worldMap.iMapWidth);
}

void show_map()
{
    char tmp[SCREEN_WIDTH + 1];

    printf("\n");
    for (uint16_t i = CameraPosition.y; i < SCREEN_HEIGHT + CameraPosition.y; i++)
    {
        uint16_t k = 0;
        for(uint16_t j = CameraPosition.x; j < SCREEN_WIDTH + CameraPosition.x; j++)
        {
            tmp[k++] = worldMap.map[i][j];
           // putchar(worldMap.map[i][j]);
        }
        tmp[k] = '\0';
        puts(tmp);
    }
}

void show_info()
{
    static int16_t clicks = 0;

    printf("Current size of map: [%d] | Current row/column pointers: [%d/%d] | Frames: [%d] | Clicks: [%d]     \n",
            worldMap.iMapSize, CameraPosition.y, CameraPosition.x, worldMap.iMapFrameCount, clicks+= 1);
    printf("Move with right and left arrows | ESC to exit | any other key to stop moving");
}

void move(int8_t direction)
{
    // Сохраняем позицию курсора
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    printf("\x1b%d", 7);        // Сохраняем позицию курсора

#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    COORD pos;
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
        pos = cbsi.dwCursorPosition;
    else
    {
        pos.X = 0; pos.Y = 0;
    }
#else
    printf("\x1e");  
#endif

    

    if (direction == RIGHT) 
    {
        if (CameraPosition.x + SCREEN_WIDTH + SPEED >= worldMap.iMapWidth)
        {
            worldMap.iMapWidth += (SCREEN_WIDTH * NUM_MALLOC_FRAME);
            create_map();
            worldMap.iMapFrameCount += NUM_MALLOC_FRAME;
        }

        CameraPosition.x += SPEED;
    }

    else if (direction == LEFT)
    {
        if ((int32_t)CameraPosition.x - SPEED >= 0)
            CameraPosition.x -= SPEED;
        else if (CameraPosition.x != 0)
            CameraPosition.x--;
    }

    show_map();
    show_info();

    // Получаем сохраненную позицию курсора
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    // https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
    printf("\x1b%d", 8);        // Получаем сохраненную позицию курсора

#elif defined(WIN32)|| defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    SetConsoleCursorPosition(hConsoleOutput, pos);
#endif
}

void generate()
{
    static int16_t frame = 0;

    if (frame == 0)
    {
        srand(SEED);
        
        // Облака
        generate_clouds(0, worldMap.iMapWidth);
        // Горы
        generate_landscape(0, SCREEN_HEIGHT / 8, worldMap.iMapWidth, SCREEN_HEIGHT / 2, 0.3f);
        // Облака
        //generate_clouds(0, worldMap.iMapWidth);
        // Ландшафт     
        generate_landscape(0, SCREEN_HEIGHT / 4, worldMap.iMapWidth, SCREEN_HEIGHT, 0.3f);
        // Деревья
        generate_trees(0, worldMap.iMapWidth);
    }

    else
    {    // Облака
        generate_clouds(worldMap.iMapOldWidth, worldMap.iMapWidth);
        // Горы
        generate_landscape(worldMap.iMapOldWidth, SCREEN_HEIGHT / 8, worldMap.iMapWidth, SCREEN_HEIGHT / 2, 0.3f);
        // Облака
        //generate_clouds(worldMap.iMapOldWidth, worldMap.iMapWidth);
        // Ландшафт
        generate_landscape(worldMap.iMapOldWidth, SCREEN_HEIGHT / 4, worldMap.iMapWidth, SCREEN_HEIGHT, 0.3f);
        // Деревья
        generate_trees(worldMap.iMapOldWidth, worldMap.iMapWidth);
    }

    frame++;
}

void generate_landscape(uint32_t min_w, uint16_t min_h, uint32_t max_w, uint16_t max_h, float roughness)
{
   uint16_t start_y = max_h - (rand() % (min_h) + rand() % 3) - 1;
   uint16_t end_y = max_h - (rand() % (min_h) + rand() % 5) - 1;

   // Отмечаем границы отрезков
   worldMap.map[start_y][min_w] = palette_ground[4];
   worldMap.map[end_y][max_w - 1] = palette_ground[4];

   filling_landscape(min_w, start_y);
   filling_landscape(max_w - 1, end_y);

   // Заносим их в массив высот
   worldMap.mapOfHeights[min_w] = start_y;
   worldMap.mapOfHeights[max_w - 1] = end_y;
 
   midpoint_displacement(min_w, start_y, max_w - 1, end_y, roughness);
}

void midpoint_displacement(uint32_t leftX, uint16_t leftY, uint32_t rightX, uint16_t rightY, float roughness)
{
    uint32_t length = rightX - leftX;
    if (length == 1) return;
    
    uint16_t randomVal = (rand() % (int32_t)(roughness * (length) * 2)) - length * roughness;
    uint16_t h = (leftY + rightY) / 2 + randomVal;

    // Генерируем случайную высоту
    while(h >= worldMap.iMapHeight || h < 0)
    {
        randomVal = (rand() % (int32_t)(roughness * (length) * 2)) - length * roughness;
        h = (leftY + rightY) / 2 + randomVal;
    }
    worldMap.mapOfHeights[leftX + length / 2] = h;

    worldMap.map[h][leftX + length / 2] = palette_ground[4];
    filling_landscape(leftX + length / 2, h); 
    
    // Вызываем для левого и правого отрезков по рекурсии
    midpoint_displacement(leftX, leftY, leftX + length / 2, h, roughness);
    midpoint_displacement(leftX + length / 2, h, rightX, rightY, roughness);
}

void filling_landscape(uint32_t x, uint16_t start_y)
{ 
    // Плавно зарисовываем грунт ландшафта
    for(uint16_t y = start_y; y < worldMap.iMapHeight; y++)
    {
        uint16_t deltaY = worldMap.iMapHeight - start_y;
        char color;

        if (y > start_y && y <= start_y + deltaY / 4) color = palette_ground[3];
        else if (y > start_y + deltaY / 4 && y <= start_y + deltaY / 2) color = palette_ground[2];
        else if (y > start_y + deltaY / 2 && y <= start_y + 3 * deltaY / 4) color = palette_ground[1];
        else color = palette_ground[0];

        worldMap.map[y][x] = color;
    }
}

void generate_trees(uint32_t min_w, uint32_t max_w)
{
    char* trees[] = {
        " ^ /|\\/|\\",
        " # ### | ",
        "~#_#x# | "
    };


    uint32_t x, y;
    int16_t randomTree;

    x = y = 0;

    if (TREES_ON)
    {
        for(int16_t k = 0; k < TREES_NUM; k++)
        {
            // Генерируем случайный Х и получаем соответствующую ему высоту из массива высот
            x = min_w + rand() % (max_w - min_w - 15) + 5;
            y = worldMap.mapOfHeights[x] - 2;

            randomTree = rand() % 3;

            // Деревья имеют размер 3х3
            for(int16_t i = 0; i < 3; i++)
            {
                for(int16_t j = 0; j < 3; j++)
                {
                    if (y + i > 0 && y + i < worldMap.iMapHeight && x + j > min_w && x + j < max_w)
                        worldMap.map[y + i][x + j] = trees[randomTree][i * 3 + j]; 
                }
            }
        }
    }
}

void generate_clouds(uint32_t min_w, uint32_t max_w)
{
    char* clouds[] = {
        "   .--.   .-(    ). (__.__)__)",
        "    .-.      (   ).   (___(__)",
        " _-.__     ( __ )._ (___(__)_)"
    };

    uint32_t x, y;
    int16_t randomCloud = 0;

    if (CLOUDS_ON)
    {
        for(int16_t k = 0; k < CLOUDS_NUM; k++)
        {
            // Первые 4 облака рисуем у границы, чтобы убрать эффект рваности
            if (k < 4)  x = min_w + rand() % 2;
            else        x = min_w + rand() % (max_w - min_w - k) + k;
            y = rand() % 10;

            randomCloud = rand() % 3;

            for(int16_t i = 0; i < 3; i++)
            {
                for(int16_t j = 0; j < 10; j++)
                {
                    if (x + j < worldMap.iMapWidth)
                        worldMap.map[CameraPosition.y + i + y][x + j] = clouds[randomCloud][i * 10 + j];
                }
            }
        }
    }
}
