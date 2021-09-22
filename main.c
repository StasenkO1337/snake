#include <malloc.h>
#include <conio.h> 
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>


typedef enum Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT
} Direction;

typedef struct {
	COORD;
}Object;

typedef struct {
	Direction direction;
	COORD* body;	
	unsigned int length;
	unsigned int speed;
}Snake;

typedef struct {
	COORD leftUp,rightDown;
}GameField;

typedef struct {
	Snake snake;
	GameField field;
	Object food;
	unsigned int score;
} Game;

void respawnFood();

void drawField();

void growSnake();

void pauseGame();

void restartGame();

void gameOver();

void control();

void gameTact();

void newGame();

void draw();

void removeTail();


unsigned short int FPS = 50;

unsigned int length = 3;
unsigned int speed = 1;
unsigned int width = 50;
unsigned int height = 20;

Game game;
HANDLE hConsole;

int main(){
	int tmp = 0;
	srand(time(NULL));
	printf("choose a difficulty\n1-ez\n2-normal\n3-hard\n4-ultra veri slojno\n");
	scanf_s("%d", &tmp);
	system("cls");
	switch (tmp)
	{
	case 1:
		FPS = 5;
		break;
	case 2:
		FPS = 10;
		break;
	case 3:
		FPS = 30;
		break;
	case 4:
		FPS = 60;
		break;
	default:
		exit(0);
		break;
	}
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
	newGame();
	drawField(hConsole);
	while (TRUE) {
		removeTail();
		gameTact();
		draw();
		Sleep(1000 / FPS);
	}
}

void respawnFood() {
	game.food.X = 1 + rand() % (game.field.rightDown.X - 2);
	game.food.Y = 1 + rand() % (game.field.rightDown.Y - 2);
	for (int i = 0; i < game.snake.length; ++i) {
		if (game.snake.body[i].X == game.food.X && game.snake.body[i].X == game.food.X) {
			respawnFood();
			break;
		}
	}
}

void drawField() {
	SetConsoleCursorPosition(hConsole, (COORD) { width + 10, 5 });
	printf("Score : %d",game.score);
	SetConsoleCursorPosition(hConsole, (COORD) { 0, 0 });
	SetConsoleTextAttribute(hConsole, (WORD)(12 | 5));
	for (int i = 0; i <= game.field.rightDown.X; ++i) {
		printf("#");
	}
	for (int i = 0; i <= game.field.rightDown.Y; ++i) {
		SetConsoleCursorPosition(hConsole, (COORD) { game.field.rightDown.X, i });
		printf("#");
	}
	SetConsoleCursorPosition(hConsole, (COORD) { 0, 0 });
	for (int i = 0; i <= game.field.rightDown.Y; ++i) {
		SetConsoleCursorPosition(hConsole, (COORD) { 0, i });
		printf("#");
	}
	for (int i = 0; i < game.field.rightDown.X; ++i) {
		printf("#");
	}
	SetConsoleTextAttribute(hConsole, (WORD)(12 | 0));
}

void growSnake() {
	COORD newCoord;
	game.snake.length += 1;
	game.score += 1;
	newCoord = (COORD){ game.snake.body[game.snake.length - 1].X,game.snake.body[game.snake.length - 1].Y };
	game.snake.body[game.snake.length] = newCoord;
}

void pauseGame() {
	while (!_kbhit() && _getwch() != L' ') Sleep(100);
}

void restartGame() {
	system("cls");
	drawField();
	newGame();
}

void gameOver() {
	system("cls");
	drawField(hConsole, game);
	SetConsoleCursorPosition(hConsole, (COORD) { game.field.rightDown.X / 2, game.field.rightDown.Y / 2 });
	printf_s("GAME OVER");
	SetConsoleCursorPosition(hConsole, (COORD) { game.field.rightDown.X / 2, game.field.rightDown.Y / 2 + 1 });
	printf_s("YOUR SCORE : % d", game.score);
	pauseGame();
	restartGame();
}

void control() {
	wchar_t key = _getwch();
	if ((key == L'a' || key == L'A') && game.snake.direction != RIGHT) {
		game.snake.direction = LEFT;
	}
	else if ((key == 'd' || key == 'D') && game.snake.direction != LEFT) {
		game.snake.direction = RIGHT;
	}
	else if ((key == 'w' || key == 'W') && game.snake.direction != DOWN) {
		game.snake.direction = UP;
	}
	else if ((key == 's' || key == 'S') && game.snake.direction != UP) {
		game.snake.direction = DOWN;
	}
	else if (key == L' ') {
		pauseGame();
	}
}

void gameTact() {
	BOOL moved = FALSE;

	if (_kbhit()) {
		control(hConsole, game);
	}

	for (int i = game.snake.length - 1; i > 0; --i) {
		game.snake.body[i].X = game.snake.body[i - 1].X;
		game.snake.body[i].Y = game.snake.body[i - 1].Y;
	}
	switch (game.snake.direction)
	{
	case UP:
		if (game.snake.body->Y > game.snake.speed) {
			game.snake.body->Y -= game.snake.speed;
			moved = TRUE;
		}
		break;
	case DOWN:
		if (game.snake.body->Y < game.field.rightDown.Y - game.snake.speed) {
			game.snake.body->Y += game.snake.speed;
			moved = TRUE;
		}
		break;
	case LEFT:
		if (game.snake.body->X > game.snake.speed) {
			game.snake.body->X -= game.snake.speed;
			moved = TRUE;
		}
		break;
	case RIGHT:
		if (game.snake.body->X < game.field.rightDown.X - game.snake.speed) {
			game.snake.body->X += game.snake.speed;
			moved = TRUE;
		}
		break;
	}

	if (!moved) gameOver();

	if (game.food.X == game.snake.body->X && game.food.Y == game.snake.body->Y) {
		growSnake();
		respawnFood();
		
	}
	for (int i = 1; i < game.snake.length; ++i) {
		if (game.snake.body[0].X == game.snake.body[i].X && game.snake.body[0].Y == game.snake.body[i].Y) {
			gameOver(hConsole, game);
		}
	}
}

void newGame() {
	game.field.leftUp = (COORD){ 0,0 };
	game.field.rightDown = (COORD){ width,height };
	game.snake.direction = RIGHT;
	game.snake.speed = 1;
	game.snake.length = length;
	game.snake.body = (COORD*)malloc(sizeof(COORD) * (width * height));
	if (!game.snake.body) {
		exit(-1);
	}
	game.snake.body->X = width / 2;
	game.snake.body->Y = height / 2;
	for (int i = 1; i < length; ++i) {
		game.snake.body[i].Y = game.snake.body->Y;
		game.snake.body[i].X = game.snake.body->X - i;
	}
	game.score = 0;
	respawnFood();
}

void draw() {
	SetConsoleTextAttribute(hConsole, (WORD)(14 | 14));
	SetConsoleCursorPosition(hConsole, (COORD) { game.food.X, game.food.Y });
	printf("@");
	SetConsoleTextAttribute(hConsole, (WORD)(10 | 0));
	SetConsoleCursorPosition(hConsole, (COORD) { game.snake.body->X, game.snake.body->Y });
	printf("O");
	SetConsoleTextAttribute(hConsole, (WORD)(5 | 10));
	for (int i = 1; i < game.snake.length; i++) {
		SetConsoleCursorPosition(hConsole, (COORD) { game.snake.body[i].X, game.snake.body[i].Y });
		printf("o");
	}
}

void removeTail() {
	SetConsoleCursorPosition(hConsole, (COORD) { game.snake.body[game.snake.length - 1].X, game.snake.body[game.snake.length - 1].Y });
	printf(" ");
	SetConsoleCursorPosition(hConsole, (COORD) { game.snake.body[game.snake.length].X, game.snake.body[game.snake.length].Y });
	printf(" ");
	drawField();
}
