#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define WIDTH 41
#define HEIGHT 21

char maze[HEIGHT][WIDTH];

int playerX, playerY;
int endX, endY;
int CELL_SIZE;

int p1Score = 0, p2Score = 0;

int dirY[4] = {-2, 2, 0, 0};
int dirX[4] = {0, 0, -2, 2};

int solveDirY[4] = {-1, 1, 0, 0};
int solveDirX[4] = {0, 0, -1, 1};

// MAZE
void initMaze()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            maze[i][j] = '#';
}

void shuffle(int arr[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
}

void carveMaze(int x, int y)
{
    maze[x][y] = ' ';
    int dirs[4] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dirX[dirs[i]];
        int ny = y + dirY[dirs[i]];

        if (nx > 0 && nx < HEIGHT - 1 && ny > 0 && ny < WIDTH - 1)
        {
            if (maze[nx][ny] == '#')
            {
                maze[x + dirX[dirs[i]] / 2][y + dirY[dirs[i]] / 2] = ' ';
                carveMaze(nx, ny);
            }
        }
    }
}

void copyMaze(char src[HEIGHT][WIDTH], char dest[HEIGHT][WIDTH])
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            dest[i][j] = src[i][j];
}
void drawCell(int x, int y)
{
    int left = y * CELL_SIZE + 50;
    int top = x * CELL_SIZE + 50;

    if (maze[x][y] == '#')
        setfillstyle(SOLID_FILL, DARKGRAY);
    else if (maze[x][y] == 'S')
        setfillstyle(SOLID_FILL, GREEN);
    else if (maze[x][y] == 'E')
        setfillstyle(SOLID_FILL, RED);
    else
        setfillstyle(SOLID_FILL, BLACK);

    bar(left, top, left + CELL_SIZE, top + CELL_SIZE);
}

void drawMaze()
{
    cleardevice();
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            drawCell(i, j);
}

void drawPlayer(int oldX, int oldY, int newX, int newY, int color)
{
    drawCell(oldX, oldY);

    int px = newY * CELL_SIZE + 50;
    int py = newX * CELL_SIZE + 50;

    setfillstyle(SOLID_FILL, color);
    bar(px + 6, py + 6, px + CELL_SIZE - 6, py + CELL_SIZE - 6);
}


void drawStep(int x, int y, int color)
{
    int px = y * CELL_SIZE + 50;
    int py = x * CELL_SIZE + 50;

    setfillstyle(SOLID_FILL, color);
    bar(px + 6, py + 6, px + CELL_SIZE - 6, py + CELL_SIZE - 6);
}

int solveMaze(int x, int y)
{
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH)
        return 0;

    if (maze[x][y] == '#' || maze[x][y] == '.')
        return 0;

    if (x == endX && y == endY)
        return 1;

    if (!(x == playerX && y == playerY))
        maze[x][y] = '.';

    drawStep(x, y, YELLOW);
    delay(15);

    for (int i = 0; i < 4; i++)
    {
        int nx = x + solveDirX[i];
        int ny = y + solveDirY[i];

        if (solveMaze(nx, ny))
        {
            drawStep(x, y, GREEN);
            return 1;
        }
    }

    drawStep(x, y, LIGHTRED);
    delay(10);

    maze[x][y] = ' ';
    return 0;
}
int playTurn(int startX, int startY, int color)
{
    int px = startX;
    int py = startY;
    int moves = 0;

    char backup[HEIGHT][WIDTH];

    drawMaze();
    drawPlayer(px, py, px, py, color);

    while (true)
    {
        int oldX = px, oldY = py;
        int nx = px, ny = py;

        char move = getch();

        if (move == 'h' || move == 'H')
        {
            copyMaze(maze, backup);

            drawMaze();
            solveMaze(px, py);
            drawPlayer(px, py, px, py, color);

            getch();

            copyMaze(backup, maze);
            drawMaze();
            drawPlayer(px, py, px, py, color);
            continue;
       
}

int main()
{
    int screenW = getmaxwidth();
    int screenH = getmaxheight();

    initwindow(screenW, screenH, "Maze Game");

    CELL_SIZE = (screenH - 200) / HEIGHT;
    if (CELL_SIZE * WIDTH > screenW - 100)
        CELL_SIZE = (screenW - 100) / WIDTH;

    srand(time(0));

    while (true)
    {
        cleardevice();

        outtextxy(50, 50, "1. Generate Maze");
        outtextxy(50, 100, "2. Single Player");
        outtextxy(50, 150, "3. Solve Maze (DFS)");
        outtextxy(50, 200, "4. Two Player");
        outtextxy(50, 250, "5. Exit");

        char ch = getch();

        if (ch == '1')
        {
            initMaze();
            carveMaze(1, 1);

            playerX = 1;
            playerY = 1;
            endX = HEIGHT - 2;
            endY = WIDTH - 2;

            maze[playerX][playerY] = 'S';
            maze[endX][endY] = 'E';

            drawMaze();
            getch();
        }
         else if (ch == '2')
        {
            playTurn(playerX, playerY, CYAN);
        }
    }
}