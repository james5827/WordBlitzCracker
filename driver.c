#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "strmap.h"
#define GRID_SIZE 4
#define WORD_SIZE GRID_SIZE * GRID_SIZE + 1
typedef char byte;

void populate_grids();
void duplicate_active_grid(bool target[GRID_SIZE][GRID_SIZE], bool source[GRID_SIZE][GRID_SIZE]);

char letter_grid[GRID_SIZE][GRID_SIZE];
bool active_grid[GRID_SIZE][GRID_SIZE];

char word[WORD_SIZE];

void recursive_search(bool grid_cpy[GRID_SIZE][GRID_SIZE], byte row, byte col, byte letter);

StrMap *sm;
char * s_gets(char *st, int n, FILE * fp);

int main(void)
{
	memset(word, '\0', WORD_SIZE);
	populate_grids();
	
	sm = sm_new(466500);
	if (sm == NULL)
		return 2;

	FILE *dictionary;
	dictionary = fopen("dictionary/words.txt", "r");
	char buf[128];

	while (s_gets(buf, 128, dictionary)) {
		sm_put(sm, buf, buf);
	}
	fclose(dictionary);

	for (int i = 0; i < GRID_SIZE; ++i) 
		for (int j = 0; j < GRID_SIZE; ++j)
			recursive_search(active_grid, i, j, 0);
	
	sm_delete(sm);

	return 1;
}

void recursive_search(bool act_master[GRID_SIZE][GRID_SIZE], byte row, byte col, byte letter)
{
	act_master[row][col] = false;
	word[letter++] = letter_grid[row][col];

	if (sm_exists(sm, word) == true) {
		printf("%s\n", word);
	}

	bool act_cpy[GRID_SIZE][GRID_SIZE];
	duplicate_active_grid(act_cpy, act_master);

	// top
	if (row - 1 >= 0 && act_master[row - 1][col]) {
		recursive_search(act_cpy, row - 1, col, letter);
	}

	// top right
	if (row - 1 >= 0 && col + 1 < GRID_SIZE && act_master[row - 1][col + 1]) {
		recursive_search(act_cpy, row - 1, col + 1, letter);
	}

	// right
	if (col + 1 < GRID_SIZE && act_master[row][col + 1]) {
		recursive_search(act_cpy, row, col + 1, letter);
	}

	// bottom right
	if (row + 1 < GRID_SIZE && col + 1 < GRID_SIZE && act_master[row + 1][col + 1]) {
		recursive_search(act_cpy, row + 1, col + 1, letter);
	}

	// bottom
	if (row + 1 < GRID_SIZE && act_master[row + 1][col]) {
		recursive_search(act_cpy, row + 1, col, letter);

	}

	// bottom left
	if (row + 1 < GRID_SIZE && col - 1 >= 0 && act_master[row + 1][col - 1]) {
		recursive_search(act_cpy, row + 1, col - 1, letter);
	}

	//left
	if (col - 1 >= 0 && act_master[row][col - 1]) {
		recursive_search(act_cpy, row, col - 1, letter);
	}

	//top left
	if (row - 1 >= 0 && col - 1 >= 0 && act_master[row - 1][col - 1]) {
		recursive_search(act_cpy, row - 1, col - 1, letter);
	}

	act_master[row][col] = true;
	word[letter] = '\0';
}

//GRID FUNCTIONS
void populate_grids()
{
	char c;
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			printf("Enter Letter for Row %d Column %d: ", i, j);
			c = getchar();
			if (c != '\n')
				while (getchar() != '\n')
					continue;

			if (isalpha(c)) {
				letter_grid[i][j] = tolower(c);
				active_grid[i][j] = true;
			} else {
				puts("Invalid Input");
				--j;
				continue;
			}
		}
		putchar('\n');
	}
}

void duplicate_active_grid(bool target[GRID_SIZE][GRID_SIZE], bool source[GRID_SIZE][GRID_SIZE])
{
	for (int i = 0; i < GRID_SIZE; ++i)
		for (int j = 0; j < GRID_SIZE; ++j)
			target[i][j] = source[i][j];
}

char * s_gets(char *st, int n, FILE * fp)
{
	char * ret;
	char * find;

	ret = fgets(st, n, fp);

	if (ret) {
		find = strchr(st, '\n');
		if (find)
			*find = '\0';
		else
			while (getchar() != '\n')
				continue;
	}

	return ret;
}
