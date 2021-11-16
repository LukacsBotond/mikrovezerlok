#include "../include/snake.h"

void printMap()
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidht; j++)
        {
            printf("%i ", map[i][j]);
        }
        printf("\n");
    }
}

void printSnake()
{
    printf("\n snakeY\n");
    for (int i = 0; i < mapHeight; i++)
    {
        printf("%i ", snakeY[i]);
    }

    printf("\n snakeX\n");
    for (int i = 0; i < mapWidht; i++)
    {
        printf("%i ", snakeX[i]);
    }
    printf("\n");
}

enum dir SNAKE_getDir()
{
    if (gpio_get_level(PIN_NUM_UP))
        return up;
    if (gpio_get_level(PIN_NUM_DOWN))
        return down;
    if (gpio_get_level(PIN_NUM_LEFT))
        return left;
    if (gpio_get_level(PIN_NUM_RIGHT))
        return right;
    return none;
}

void SNAKE_gameStart(spi_device_handle_t spi)
{
    gpio_set_direction(PIN_NUM_UP, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_DOWN, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_LEFT, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_NUM_RIGHT, GPIO_MODE_INPUT);
    srand(1582552);
    nextHeadX = mapWidht / 2;
    nextHeadY = mapHeight / 2;
    score = 0;
    size = 1;
    ate = false;
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidht; j++)
        {
            map[i][j] = bg_color;
        }
    }

    map[mapWidht / 2][mapHeight / 2] = snake_color;
    snakeX[0] = mapWidht / 2;
    snakeY[0] = mapHeight / 2;
    map[1][5] = food_color;
    printMap();
    SNAKE_sendScreen(spi);
}

void SNAKE_mainLoop(spi_device_handle_t spi)
{
    SNAKE_gameStart(spi);
    enum dir prevDir = up;
    enum dir currDir;
    while (1)
    {
        currDir = SNAKE_getDir();
        if (currDir == none)
            currDir = prevDir;
        //printf("dir: %i\n", currDir);
        SNAKE_updateHead(currDir);

        SNAKE_IsFood();

        if (!SNAKE_Move())
            break;
        SNAKE_sendScreen(spi);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        prevDir = currDir;
    }
    SNAKE_gameEnded();
}

void SNAKE_updateHead(enum dir direction)
{
    switch (direction)
    {
    case up:
        nextHeadX--;
        break;
    case down:
        nextHeadX++;
        break;
    case left:
        nextHeadY--;
        break;
    case right:
        nextHeadY++;
        break;
    default:
        printf("not supposed to happen");
    }
}

//TODO
bool SNAKE_Move()
{
    if (SNAKE_gameOver())
        return false;
    int i;

    for (i = size; i >= 1; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] = nextHeadX;
    snakeY[0] = nextHeadY;
    map[snakeX[0]][snakeY[0]] = snake_color;

    return true;
}

void SNAKE_sendScreen(spi_device_handle_t spi)
{
    int currLine = 0;
    for (int x = 0; x < 240; x += PARALLEL_LINES)
    {
        for (int rep = 0; rep < PARALLEL_LINES; rep++)
        {
            for (int y = 0; y < 320; y++)
            {
                line[currLine][(320 * rep) + y] = map[x / PARALLEL_LINES][y / PARALLEL_LINES];
            }
        }
        //printf("i%i \n", x);
        ESP32_send_lines(spi, x, line[currLine]);
        ESP32_send_line_finish(spi);
    }
    //ESP32_send_lines(spi, 240, line[currLine]);
    //ESP32_send_line_finish(spi);
}

bool SNAKE_IsFood()
{
    //todo remove tail if not food
    printSnake();
    printf("next: %i\n", map[snakeX[0]][snakeY[0]]);
    if (map[nextHeadX][nextHeadY] == food_color)
    {
        //printf("---------------------------------ate\n");
        ate = true;
        map[snakeX[size]][snakeY[size]] = snake_color;
        //snakeX[size] =
        size++;
        score += 50;
        SNAKE_generateFood();
        return true;
    }
    map[snakeX[size]][snakeY[size]] = bg_color;
    return false;
}

void SNAKE_generateFood()
{
    int x;
    int y;
    while (1)
    {
        x = rand()%mapWidht;
        y = rand()%mapHeight;
        if(map[x][y] == 0){
            map[x][y] = food_color;
            break;
        }
    }
}

bool SNAKE_gameOver()
{

    //boundy
    if (nextHeadX < 0 || nextHeadY < 0 || nextHeadX >= mapWidht || nextHeadY >= mapHeight)
        return true;

    //yooo, that's not possible
    if (map[nextHeadX][nextHeadY] == snake_color || map[nextHeadX][nextHeadY] == wall_color)
    {
        return true;
    }

    return false;
}

void SNAKE_gameEnded()
{
    while (1)
    {
        printf("Snake color 0xff!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
