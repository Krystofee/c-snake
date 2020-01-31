#include <simple2d.h>
#include <stdlib.h>
#include <time.h>

#define TRUE (1)
#define FALSE (0)

#define FOR(x, n) for (int x = 0; x < n; ++x)

#define TITLE ("Snake")
#define SCREEN_SIZE (600)
#define FPS_CAP (2)

#define GRID_SIZE  (60)
#define RECT_SIZE (10)

#define WHITE 255, 255, 255, 1

S2D_Window *window;

int stop = FALSE;

typedef struct coord {
    int x;
    int y;
} s_coord;

void init_coord(s_coord *c, int x, int y)
{
    // David, look how pointers in C works,
    // https://www.tutorialspoint.com/cprogramming/c_pointers.htm
    // https://www.geeksforgeeks.org/pointers-in-c-and-c-set-1-introduction-arithmetic-and-array/
    // these two sites I like
    c->x = x;
    c->y = y;
}

s_coord ZERO;
s_coord UP;
s_coord DOWN;
s_coord RIGHT;
s_coord LEFT;

s_coord snake[GRID_SIZE * GRID_SIZE]; // 0 item is head and th rest is tail
int snake_size = 0;
s_coord direction;

void init()
{
    init_coord(&ZERO, 0, 0);
    init_coord(&UP, 0, -1);
    init_coord(&DOWN, 0, 1);
    init_coord(&RIGHT, 1, 0);
    init_coord(&LEFT, -1, 0);

    direction = RIGHT;

    init_coord(&snake[0], 10, 10);
    init_coord(&snake[1], 9, 10);
    init_coord(&snake[2], 8, 10);
    init_coord(&snake[3], 7, 10);
    init_coord(&snake[4], 6, 10);
    init_coord(&snake[5], 5, 10);
    init_coord(&snake[6], 4, 10);

    snake_size ++;
    snake_size ++;
    snake_size ++;
    snake_size ++;
    snake_size ++;
    snake_size ++;
    snake_size ++;
}

int cmp_coord(s_coord a, s_coord b)
{
    return a.x == b.x && a.y == b.y;
}

s_coord add_coord(s_coord a, s_coord b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

void change_direction(s_coord new_direction)
{
    // for example [1,0] + [0,1]  = [1,1]... [1,1] != [0,0] = Valid move
    // for example [1,0] + [-1,0] = [0,0]... [0,0] != [0,0] = Invalid move
    s_coord sum = add_coord(direction, new_direction);
    int result = cmp_coord(sum, ZERO);

    if (result) {
        printf("...learn to play snake, idiot\n");
    } else {
        direction = new_direction;
    }
}

void move()  // if x = 1, and y = 0, then snake moves 1 to the right...
{
    // move tail
    for (int n = snake_size; n > 0; -- n) {
        snake[n].x = snake[n - 1].x;
        snake[n].y = snake[n - 1].y;
    }

    // move head
    snake[0].x += direction.x;
    snake[0].y += direction.y;

    // check if any part is outside the border, in that case, teleport it to
    // the opposite side
    FOR (n, snake_size) {
        s_coord snake_part = snake[n];

        if (snake_part.x < 0) snake_part.x = GRID_SIZE - 1;
        if (snake_part.y < 0) snake_part.y = GRID_SIZE - 1;
        if (snake_part.x >= GRID_SIZE) snake_part.x = 0;
        if (snake_part.y >= GRID_SIZE) snake_part.y = 0;

        snake[n] = snake_part;
    }
}

void on_key(S2D_Event e)
{
    if (e.type == S2D_KEY_DOWN)
    {
        if (strcmp(e.key, "W") == 0) change_direction(UP);
        else if (strcmp(e.key, "S") == 0) change_direction(DOWN);
        else if (strcmp(e.key, "A") == 0) change_direction(LEFT);
        else if (strcmp(e.key, "D") == 0) change_direction(RIGHT);
    }
}

void update()
{
    if (stop == TRUE)
        return;

    move();
}

void draw_rect(int x, int y)
{
    x *= RECT_SIZE;
    y *= RECT_SIZE;
    S2D_DrawLine(x, y + RECT_SIZE / 2, x + RECT_SIZE, y + RECT_SIZE / 2, RECT_SIZE, WHITE, WHITE, WHITE, WHITE);
}

void render()
{
    FOR (i, snake_size) {
        draw_rect(snake[i].x, snake[i].y);
    }
}

int main()
{
    init();

    window = S2D_CreateWindow(TITLE, SCREEN_SIZE, SCREEN_SIZE, update, render, S2D_HIGHDPI);
    window->fps_cap = FPS_CAP;
    window->on_key = on_key;

    S2D_Show(window);
    return 0;
}

