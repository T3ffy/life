#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
//#include <unistd.h> // linux user 💋
#include <windows.h> // windows user 🙄🤢
#include <conio.h>


#define WORLD_WIDTH 20// Ширина игрового поля
#define WORLD_HEIGHT 40 // Высота игрового поля
#define SIZE 200000 // Количество "миров" для определения цикличности
#define CELL "#" // Как выглядит клетка
#define TIME 50000 // Время задержки между кадрами в микросекундах 
// Структура для хранения ячейки
struct point
{
	bool is_live : true;
};
struct cycleWorld
{
	struct point w[WORLD_WIDTH][WORLD_HEIGHT];
};




struct point world[WORLD_WIDTH][WORLD_HEIGHT]; // Мир =)
struct cycleWorld worlds[SIZE];

unsigned int randy(int min, int max); //Генерирует псевдослучайные значения от min до max
void initWorld(struct point world[WORLD_WIDTH][WORLD_HEIGHT],int kk); // Инициализация мира
void printWorld(struct point world[WORLD_WIDTH][WORLD_HEIGHT]); // Печать мира
unsigned int getLiveCount(struct point world[WORLD_WIDTH][WORLD_HEIGHT]); // Подсчет живых клеток
void readPointNeighbors(signed int nb[][2], unsigned int x, unsigned int y); // Получение координат соседей клетки
unsigned int countLiveNeighbors(
	struct point world[WORLD_WIDTH][WORLD_HEIGHT],
	unsigned int x,
	unsigned int y
); //Сколько живых соседей у клетки (x,y)
void nextGenerations(
	struct point world[WORLD_WIDTH][WORLD_HEIGHT],
	struct point prev_world[WORLD_WIDTH][WORLD_HEIGHT]
); //Следующее поколение клеток
void copyWorld(
	struct point src[WORLD_WIDTH][WORLD_HEIGHT],
	struct point dect[WORLD_WIDTH][WORLD_HEIGHT]
); //Копировать мир
int cmpWorlds(
	struct point world[WORLD_WIDTH][WORLD_HEIGHT],
	struct point prev_world[WORLD_WIDTH][WORLD_HEIGHT]
); //Сравнить миры 


unsigned int randy(int min, int max) {
	//srand(time(NULL));
	return min + rand() % (max - min);
}

void initWorld(struct point world[WORLD_WIDTH][WORLD_HEIGHT],int kk) {
	int i, j,count=0, rnd = 0;
	srand(time(NULL)); //Randomagic! Инициализация генератора случайных величин
	for (i = 1; i < WORLD_WIDTH;i++) {
		for (j = 1; j < WORLD_HEIGHT;j++) {
			if (rand() % 2 == 0) {
				world[i][j].is_live = 0;

				//printf("%d ,%d\n", WORLD_WIDTH,WORLD_HEIGHT );
			}
			else {
				if (count == kk) { continue; }
				world[i][j].is_live = 1;
				count++;
			}
		}
	}
}
void printWorld(struct point world[WORLD_WIDTH][WORLD_HEIGHT]) {
	int i, j = 0;
	printf("\n---------------------------------------------\n");
	for (i = 1; i < WORLD_WIDTH;i++) {
		printf("\n");
		for (j = 1; j < WORLD_HEIGHT;j++) {
			if (world[i][j].is_live == 1) {
				printf(CELL);
			}
			else {
				printf(" ");
			}
		}
	}
	printf("\n---------------------------------------------\n");
}
unsigned int getLiveCount(struct point world[WORLD_WIDTH][WORLD_HEIGHT]) {
	int i, j, count = 0;
	for (i = 1; i < WORLD_WIDTH;i++) {
		for (j = 1; j < WORLD_HEIGHT;j++) {
			if (world[i][j].is_live == 1) {
				count++;
			}
		}
	}
	return count;
}
void readPointNeighbors(signed int nb[][2], unsigned int x, unsigned int y) {
	unsigned int i, j, k = 0;
	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) {
				continue;
			}
			nb[k][0] = i;
			nb[k][1] = j;
			k++;
		}
	}
}
unsigned int countLiveNeighbors(struct point world[WORLD_WIDTH][WORLD_HEIGHT], unsigned int x, unsigned int y) {
	unsigned int count = 0;
	unsigned int i;
	signed int nb[8][2];
	signed int _x, _y;
	readPointNeighbors(nb, x, y);
	for (i = 0; i < 8; i++) {
		_x = nb[i][0];
		_y = nb[i][1];
		if (_x < 0 || _y < 0) {
			continue;
		}
		if (_x >= WORLD_WIDTH || _y >= WORLD_HEIGHT) {
			continue;
		}
		if (world[_x][_y].is_live == 1) {
			count++;
		}
	}
	return count;
}

void nextGenerations(struct point world[WORLD_WIDTH][WORLD_HEIGHT], struct point prev_world[WORLD_WIDTH][WORLD_HEIGHT]) {
	unsigned int i, j;
	unsigned int live_nb;
	struct point p;
	for (i = 0; i < WORLD_WIDTH; i++) {
		for (j = 0; j < WORLD_HEIGHT; j++) {
			p = prev_world[i][j];
			live_nb = countLiveNeighbors(prev_world, i, j);
			if (p.is_live == 0) {
				if (live_nb == 3) {
					world[i][j].is_live = 1;
				}
			}
			else {
				if (live_nb < 2 || live_nb > 3) {
					world[i][j].is_live = 0;
				}
			}
		}
	}
}


void copyWorld(struct point src[WORLD_WIDTH][WORLD_HEIGHT], struct point dect[WORLD_WIDTH][WORLD_HEIGHT]) {
	unsigned int i, j;
	for (i = 0; i < WORLD_WIDTH; i++) {
		for (j = 0; j < WORLD_HEIGHT; j++) {
			dect[i][j] = src[i][j];
		}
	}
}

int cmpWorlds(struct point world[WORLD_WIDTH][WORLD_HEIGHT], struct point prev_world[WORLD_WIDTH][WORLD_HEIGHT]) {
	unsigned int i, j;
	for (i = 0; i < WORLD_WIDTH; i++) {
		for (j = 0; j < WORLD_HEIGHT; j++) {
			if (world[i][j].is_live != prev_world[i][j].is_live) {
				return -1;
			}
		}
	}
	return 1;
}


int main(int argc, char const* argv[])

{
	char ll[10];
	int kk;
	setlocale(LC_ALL, "Rus");
	struct point world[WORLD_WIDTH][WORLD_HEIGHT];
	struct point prev_world[WORLD_WIDTH][WORLD_HEIGHT];
	printf("Введите количество стартовых клеток: ");
	scanf_s("%d",&kk);
	initWorld(world,kk);
	int live_points = -1;
	int opt, count = 0;
	int i, cycledCount = 0;
	do {
		printWorld(world);
		Sleep(20);
		system("CLS");// очистка консоли
		Sleep(2);
		// Задержка между кадрами
		Sleep(10);
		copyWorld(world, prev_world);
		copyWorld(world, worlds[cycledCount].w);

		nextGenerations(world, prev_world);
		opt = cmpWorlds(world, prev_world);
		for (i = 0; i < cycledCount;i++) {
			if (cmpWorlds(world, worlds[i].w) == 1) {
				opt = 1;
				printf("Циклическая структура\n");
				break;
			}
		}
		live_points = getLiveCount(world);

		printf("Живых клеток - %d\n", live_points);
		if (live_points == 0) {
			printf("Все клетки мертвы... =(\n");
		}
		if (cycledCount < SIZE) {
			cycledCount++;
		}
		else {
			cycledCount = 0;
		}
		count++;
		Sleep(10);
	} while (live_points != 0 && opt < 0);
	if (opt == 1) {
		printf("Устойчивая конфигурация достигнута\nКоличество итераций %d\n", count);
	}
	scanf_s("%d", &ll);

	return 0;

}