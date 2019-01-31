#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "strmap.h"
#define GRID_SIZE 4
#define WORD_SIZE GRID_SIZE * GRID_SIZE + 1
#define DICTIONARY_LENGTH 466500
#define BUF_SIZE 128
typedef char byte;

void populate_grid();
void read_dictionary();
void recursive_search(byte row, byte col, byte letter);
char * s_gets(char *st, int n, FILE * fp);

struct node {
	char letter;
	bool active;
};

struct node grid[GRID_SIZE][GRID_SIZE];
char word[WORD_SIZE];
StrMap *sm; 

unsigned int searches = 0;
unsigned int words_found = 0;

int main(void)
{
	sm = sm_new(DICTIONARY_LENGTH);
	read_dictionary();
	populate_grid();

	memset(word, '\0', WORD_SIZE);

	clock_t start, end;
	double cpu_time;

	start = clock();
	for (int i = 0; i < GRID_SIZE; ++i) 
		for (int j = 0; j < GRID_SIZE; ++j)
			recursive_search(i, j, 0);
	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	
	sm_delete(sm);

	printf("Searches: %d, Words Found: %d, Search Time: %lf\n", searches, words_found, cpu_time);

	return 1;
}

void recursive_search(byte row, byte col, byte letter)
{
	grid[row][col].active = false;
	word[letter++] = grid[row][col].letter;

	++searches;
	if (sm_exists(sm, word) == true) {
		++words_found;
		printf("%s\n", word);
	}

	// top
	if (row - 1 >= 0 && grid[row - 1][col].active)
		recursive_search(row - 1, col, letter);

	// top right
	if (row - 1 >= 0 && col + 1 < GRID_SIZE && grid[row - 1][col + 1].active)
		recursive_search(row - 1, col + 1, letter);

	// right
	if (col + 1 < GRID_SIZE && grid[row][col + 1].active)
		recursive_search(row, col + 1, letter);

	// bottom right
	if (row + 1 < GRID_SIZE && col + 1 < GRID_SIZE && grid[row + 1][col + 1].active)
		recursive_search(row + 1, col + 1, letter);

	// bottom
	if (row + 1 < GRID_SIZE && grid[row + 1][col].active)
		recursive_search(row + 1, col, letter);

	// bottom left
	if (row + 1 < GRID_SIZE && col - 1 >= 0 && grid[row + 1][col - 1].active)
		recursive_search(row + 1, col - 1, letter);

	//left
	if (col - 1 >= 0 && grid[row][col - 1].active)
		recursive_search(row, col - 1, letter);

	//top left
	if (row - 1 >= 0 && col - 1 >= 0 && grid[row - 1][col - 1].active)
		recursive_search(row - 1, col - 1, letter);

	grid[row][col].active = true;
	word[letter] = '\0';
}

void read_dictionary()
{
	FILE *dictionary;
	char buf[BUF_SIZE];

	dictionary = fopen("dictionary/words.txt", "r");
	while (s_gets(buf, BUF_SIZE, dictionary))
		sm_put(sm, buf, buf);
	fclose(dictionary);
}

void populate_grid()
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
				grid[i][j].letter = tolower(c);
				grid[i][j].active = true;
			} else {
				puts("Invalid Input");
				--j;
				continue;
			}
		}
		putchar('\n');
	}
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
