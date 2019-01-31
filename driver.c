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
#define FOUND_SIZE 2000
#define BUF_SIZE 128
typedef char byte;

void populate_grid();
void read_dictionary();
void recursive_search(byte row, byte col, byte letter);
char * s_gets(char *st, int n, FILE * fp);
int str_sort_cb(const void *str1, const void *str2);
void remove_duplicates();

struct node {
	char letter;
	bool active;
};

struct node grid[GRID_SIZE][GRID_SIZE];

unsigned int fw_index = 0;
char *found_words[FOUND_SIZE];

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

	qsort(&found_words[0], fw_index, sizeof(char *), str_sort_cb);

	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

	while (fw_index--) {
		printf("%d %s\n", fw_index, found_words[fw_index]);
		free(found_words[fw_index]);
	}

	printf("Hash Count: %d Searches: %d, Words Found: %d, Search Time: %lf\n", sm_get_count(sm) ,searches, words_found, cpu_time);
	sm_delete(sm);
	return 1;
}

int str_sort_cb(const void *str1, const void *str2)
{
	unsigned int len1 = strlen(*(char **) str1);
	unsigned int len2 = strlen(*(char **) str2);
	if (len1 > len2)
		return -1;
	else if (len1 < len2)
		return 1;
	else 
		return 0;
}

void recursive_search(byte row, byte col, byte letter)
{
	grid[row][col].active = false;
	word[letter++] = grid[row][col].letter;

	++searches;
	if (sm_exists(sm, word)) {
		++words_found;

		//replace this monstrosity
		bool unique = true;
		for (int i = 0; i < fw_index; ++i) {
			if (strcmp(word, found_words[i]) == 0) {
				unique = false;
				break;
			}
		}

		if (unique) {
			found_words[fw_index] = malloc(letter + 1);
			strcpy(found_words[fw_index++], word);
		}
	}

	if (row - 1 >= 0) {
		if (col - 1 >= 0 && grid[row - 1][col - 1].active)
			recursive_search(row - 1, col - 1, letter);

		if (grid[row - 1][col].active)
			recursive_search(row - 1, col, letter);

		if (col + 1 < GRID_SIZE && grid[row - 1][col + 1].active)
			recursive_search(row - 1, col + 1, letter);
	}

	if (col + 1 < GRID_SIZE && grid[row][col + 1].active)
		recursive_search(row, col + 1, letter);

	if (col - 1 >= 0 && grid[row][col - 1].active)
		recursive_search(row, col - 1, letter);

	if (row + 1 < GRID_SIZE) {
		if (col + 1 < GRID_SIZE && grid[row + 1][col + 1].active)
			recursive_search(row + 1, col + 1, letter);

		if (grid[row + 1][col].active)
			recursive_search(row + 1, col, letter);

		if (col - 1 >= 0 && grid[row + 1][col - 1].active)
			recursive_search(row + 1, col - 1, letter);
	}

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

void populate_grid() {
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
