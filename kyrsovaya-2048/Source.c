#define _CRT_SECURE_NO_WARNINGS
#define SIZE_BOARD 4
#define START_BLOCK 2

#include <stdio.h>
#include <locale.h>
#include <conio.h>

enum MENU
{
	EXIT,
	PLAY
};

typedef struct DISPLAY   // Структура игрового поля
{
	int score;    
	int count;       
	int outscore;
	int board[SIZE_BOARD][SIZE_BOARD];  
	int tmp[SIZE_BOARD][SIZE_BOARD]; 
	int recall[SIZE_BOARD][SIZE_BOARD];
	int flag[SIZE_BOARD][SIZE_BOARD]; 
}DISPLAY, * PDISPLAY;

typedef struct POZ
{
	int x;
	int y;
} POZ;

void menu();  //меню
void board(PDISPLAY GameBoard);  // Инициализируем игровое поле
void PrintBoard(PDISPLAY GameBoard);  // Отображение поля
int WinOrLose(PDISPLAY GameBoard); 
int IsRound(PDISPLAY GameBoard, int x, int y); // Определение, есть ли числа, которые можно складывать вокруг чисел на поле
void AddBlock(PDISPLAY GameBoard);    // Добавляем по одному блоку за шаг
void SetBlock(PDISPLAY GameBoard, int flag, int uplimit, int i, int j);    // Объединить блоки
int PlayAct(PDISPLAY GameBoard);  // Настраиваем поле
void RecallBoard(PDISPLAY GameBoard, int Command);   // Вернуться на один ход
void Game();

POZ pozXY[4][SIZE_BOARD - 1] = {
	{{-1,0}, {-2, 0}, {-3, 0}},
	{{1,0}, {2,0}, {3,0}},
	{{0,-1}, {0, -2}, {0, -3}},
	{{0,1}, {0,2}, {0,3}}
};

void menu()
{
	printf("\t*********************\n");
	printf("\t* 1. Запустить игру *\n");
	printf("\t* 0. Выйти из игры  *\n");
	printf("\t*********************\n");
}

void board(PDISPLAY GameBoard)
{
	int count = START_BLOCK;
	int i = 0;
	int j = 0;

	while (count--)
	{
		while (1)
		{
			int x = rand() % SIZE_BOARD;
			int y = rand() % SIZE_BOARD;

			if (GameBoard->board[x][y] == 0)
			{
				int tmp = rand() % 2;

				GameBoard->board[x][y] = ((tmp) ? (2) : (4));
				break;
			}
		}
	}
	for (i = 0; i < SIZE_BOARD; i++)
	{
		for (j = 0; j < SIZE_BOARD; j++)
		{
			GameBoard->tmp[i][j] = GameBoard->board[i][j];
		}
	}
}
void PrintBoard(PDISPLAY GameBoard)  // Отображение поля
{
	int i = 0;
	int j = 0;

	printf("\n \tНынешний счет:%d", GameBoard->score);
	printf("\n-----------------------------------\n\n");
	printf("\t______________________\n");
	for (i = 0; i < SIZE_BOARD; i++)
	{
		printf("\t|");
		for (j = 0; j < SIZE_BOARD; j++)
		{
			if (GameBoard->board[i][j] == 0)
			{
				printf("    |");
				continue;
			}
			printf("%4d|", GameBoard->board[i][j]);
		}
		if (i < SIZE_BOARD - 1)
		{
			printf("\n\t______________________\n");
		}
	}
	printf("\n\t______________________\n");
	printf("\n\nW A S D вверх, вниз, влево, вправо \t\n");
	printf("\n\n-----------------------------------\n");
}
int WinOrLose(PDISPLAY GameBoard) 
{
	int i = 0;
	int j = 0;

	GameBoard->count = 0;

	for (i = 0; i < SIZE_BOARD; i++)
	{
		for (j = 0; j < SIZE_BOARD; j++)
		{
			if (GameBoard->board[i][j] != 0)
				GameBoard->count++;
			else if (GameBoard->board[i][j] == 2048) 
				return 1;
		}
	}
	if (GameBoard->count == SIZE_BOARD * SIZE_BOARD && IsRound(GameBoard, 0, 0) == 0)
	{
		return -1;
	}

	return 0;
}
int IsRound(PDISPLAY GameBoard, int x, int y) // Определение, есть ли числа, которые можно складывать вокруг чисел на шахматной доске
{
    int ret = 0;

    if (x == SIZE_BOARD - 1)
    {
        ret = 0;
    }
    else if (y == SIZE_BOARD - 1)
    {
        IsRound(GameBoard, x + 1, 0);
    }
    else if (GameBoard->board[x][y] == GameBoard->board[x][y + 1])
    {
        ret = 1;
    }
    else if (GameBoard->board[x][y] != GameBoard->board[x][y + 1])
    {
        ret += IsRound(GameBoard, x, y + 1);
    }

    return ret;
}

void AddBlock(PDISPLAY GameBoard)
{
    while (1)
    {
        int x = rand() % SIZE_BOARD;
        int y = rand() % SIZE_BOARD;

        if (GameBoard->board[x][y] == 0)
        {
            int tmp = rand() % 2;

            GameBoard->board[x][y] = ((tmp) ? (2) : (4));
            break;
        }
    }
}

void SetBlock(PDISPLAY GameBoard, int flag, int uplimit, int i, int j)   
{
    int k = 0;

    if (GameBoard->board[i][j] == 0)
        return;

    for (k = 0; k < uplimit; k++)  
    {
        if (GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y] == 0)
        {
            if (k == uplimit - 1)
            {
                GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y] = GameBoard->board[i][j];  // Перемещаем блок
                GameBoard->board[i][j] = 0;
                break;
            }
            continue;
        }
        else if (GameBoard->board[i][j] == GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y] &&
            GameBoard->flag[i + pozXY[flag][k].x][j + pozXY[flag][k].y] == 0)
        {
            GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y] *= 2;
            GameBoard->board[i][j] = 0;   
            GameBoard->score += GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y];
            GameBoard->flag[i + pozXY[flag][k].x][j + pozXY[flag][k].y] = 1;
            break;
        }
        else if (GameBoard->board[i][j] != GameBoard->board[i + pozXY[flag][k].x][j + pozXY[flag][k].y] ||
            GameBoard->flag[i + pozXY[flag][k].x][j + pozXY[flag][k].y] == 1)
        {
            if (k == 0)
            {
                break;
            }
            GameBoard->board[i + pozXY[flag][k - 1].x][j + pozXY[flag][k - 1].y] = GameBoard->board[i][j];  // Перемещаем блок
            GameBoard->board[i][j] = 0;
            break;
        }
    }
}

int PlayAct(PDISPLAY GameBoard) 
{
    int i = 0;
    int j = 0;
    int ret = 0;

    switch (_getch())
    {
    case 'w':
    case 'W':
    {
        for (i = 1; i < SIZE_BOARD; i++)
        {
            for (j = 0; j < SIZE_BOARD; j++)
            {
                SetBlock(GameBoard, 0, i, i, j);
            }
        }
    }
    break;
    case 's':
    case 'S':
    {
        for (i = SIZE_BOARD - 2; i >= 0; i--)
        {
            for (j = 0; j < SIZE_BOARD; j++)
            {
                SetBlock(GameBoard, 1, SIZE_BOARD - i - 1, i, j);
            }
        }
    }
    break;
    case 'a':
    case 'A':
    {
        for (i = 0; i < SIZE_BOARD; i++)
        {
            for (j = 1; j < SIZE_BOARD; j++)
            {
                SetBlock(GameBoard, 2, j, i, j);
            }
        }
    }
    break;
    case 'd':
    case 'D':
    {
        for (i = 0; i < SIZE_BOARD; i++)
        {
            for (j = SIZE_BOARD - 2; j >= 0; j--)
            {
                SetBlock(GameBoard, 3, SIZE_BOARD - j - 1, i, j);
            }
        }
    }
    break;
    }

    return ret;
}

void RecallBoard(PDISPLAY GameBoard, int Command)   
{
    int i = 0;
    int j = 0;

    if (Command)                              
    {
        for (i = 0; i < SIZE_BOARD; i++)
        {
            for (j = 0; j < SIZE_BOARD; j++)
            {
                GameBoard->board[i][j] = GameBoard->recall[i][j];
            }
        }
        GameBoard->score = GameBoard->outscore;
    }
    else    // Сохраняем последний ход
    {
        for (i = 0; i < SIZE_BOARD; i++)
        {
            for (j = 0; j < SIZE_BOARD; j++)
            {
                GameBoard->recall[i][j] = GameBoard->tmp[i][j];
                GameBoard->tmp[i][j] = GameBoard->board[i][j];
                GameBoard->flag[i][j] = 0;   // Отметить позицию 0 для следующего использования
            }
        }
        GameBoard->outscore = GameBoard->score;
    }
}

void Game()   
{
    int count = 0;
    int ret = 0;            
    DISPLAY GameBoard = { 0 }; 

    srand((unsigned)time(NULL));

    board(&GameBoard); 

    while (1)
    {
        system("cls");
        PrintBoard(&GameBoard);  
        ret = PlayAct(&GameBoard);
        if (ret == 1)
        {
            RecallBoard(&GameBoard, ret);
            continue;
        }
        else if (ret == 0)
        {
            AddBlock(&GameBoard);
            RecallBoard(&GameBoard, 0);
        }
        ret = WinOrLose(&GameBoard);
        if (ret == 1)  
        {
            system("cls");
            PrintBoard(&GameBoard); 
            printf("\nПобеда! \n");
            system("pause");
            return;
        }
        else if (ret == -1)
        {
            system("cls");
            PrintBoard(&GameBoard); 
            return;
        }
    }
}
int main()
{
    setlocale(LC_ALL, "RU");
    int input = 0;

    do
    {
        menu();
        printf("Чего ты хочешь?:>");
        scanf("%d", &input);

        switch (input)
        {
        case PLAY: Game(), system("cls");
            break;
        case EXIT: printf("\nВыход из игры \n");
            break;
        }
    } while (input);

    return 0;
}