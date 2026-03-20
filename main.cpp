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
    }
}