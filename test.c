#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>

#define FUNC_STOP 1
#define ERROR 1

int stopper;
int how_many_lines;
int error_code;
int main_allocs = 2;

void test_malloc();
void test_realloc();

void test_malloc()
{
	printf("Calling malloc multiple times:\n");

	for (int i = 0; i < 10; ++i) {
		int* array = malloc(5 * sizeof(int));
		if (array != NULL) {
			printf("Good allocation\n");
			free(array);
		}
		else {
			printf("Bad allocation, null pointer returned\n");
		}
	}
}

void test_realloc()
{
	printf("Calling realloc multiple times:\n");

	int* array = NULL;
	for (int i = 0; i < 10; ++i) {
		int* new_array = realloc(array, (i + 1) * sizeof(int));
		if (new_array != NULL) {
			printf("Good allocation\n");
			array = new_array;
		}
		else {
			printf("Bad allocation, null pointer returned\n");
		}
	}
	free(array);
}

// ----------------------------

char* getLine()
{
	char* line;
	char* chr;
	int allocates = 2;
	line = malloc(10 * sizeof(char));
	if (line == NULL) {
		printf("couldn't allocate requested memory\n");
		free(line);
		error_code = ERROR;
		return NULL;
	}

	chr = line;
	int tmp_char = getchar();
	if (tmp_char != EOF) {
		while (1) {
			if ((chr - line) % 10 == 8) {
				int distance = chr - line;
				void* ralloc_ptr;
				if ((ralloc_ptr = realloc(line, sizeof(char) * 10 *
													allocates)) == NULL) {
					printf("couldn't reallocate requested memory\n");
					free(line);
					error_code = ERROR;
					break;
				}
				else {
					line = ralloc_ptr;
					allocates++;
					chr = line + distance;
				}
			}
			if (tmp_char == EOF) {
				*chr = '\n';
				stopper = FUNC_STOP;
				return line;
			}
			else if (tmp_char == '\n') {
				*chr = '\n';
				return line;
			}
			*chr = tmp_char;
			chr++;
			tmp_char = getchar();
		}
	}
	stopper = FUNC_STOP;
	free(line);
	return NULL;
}

int checkLineLength(char* text)
{
	char* text_ptr = text;
	int length = 0;
	while (*text_ptr != '\n') {
		if (*text_ptr != '\n') {
			length++;
		}
		text_ptr++;
	}
	return length;
}

void printOutput(char** pointers_array)
{
	printf("\nText:\n");
	for (int i = how_many_lines - 1; i >= 0; i--) {
		int line_len = checkLineLength(pointers_array[i]);
		int j = line_len - 1;
		while (j >= 0) {
			int realloc_crashed = 0;
			int word_len = 0;
			int allocates = 2;

			char* word_pointer = malloc(10 * sizeof(char));
			if (word_pointer == NULL) {
				printf("not enough memory for this line");
				break;
			}
			char* char_pointer = word_pointer;

			while (j != -1 && pointers_array[i][j] != ' ') {
				*char_pointer = pointers_array[i][j];
				char_pointer++;
				j--;
				word_len++;

				if ((char_pointer - word_pointer) % 10 == 8) {
					int distance = char_pointer - word_pointer;
					void* ralloc_ptr;
					if ((ralloc_ptr = realloc(word_pointer,
											  sizeof(char) * 10 * allocates)) ==
						NULL) {
						printf("couldn't reallocate requested memory\n");
						realloc_crashed = 1;
						break;
					}
					else {
						word_pointer = ralloc_ptr;
						allocates++;
						char_pointer = word_pointer + distance;
					}
				}
			}
			if (realloc_crashed == 1) {
				printf("not enough memory for this line");
				break;
			}
			else {
				char_pointer--;
				for (int printer = j + word_len; printer > j; printer--) {
					printf("%c", *char_pointer);
					char_pointer--;
				}
				printf(" ");
				j--;
				free(word_pointer);
			}
		}
		printf("\n");
	}
}

char** createArrayOfLines()
{
	char** array_of_lines = malloc(10 * sizeof(char*));
	if (array_of_lines == NULL) {
		printf("couldn't allocate requested memory for array of lines");
		free(array_of_lines);
		return NULL;
	}
	else {
		return array_of_lines;
	}
}

int main()
{
	stopper = 0;
	char* getline_pointer;
	char** lines_array = createArrayOfLines();
	if (lines_array == NULL) {
		return 0;
	}
	
	while (stopper != 1 && error_code != ERROR) {
		getline_pointer = getLine();
		if (getline_pointer != NULL) {
			lines_array[how_many_lines] = getline_pointer;
			how_many_lines++;
			if (how_many_lines % 10 == 8) {
				void* ralloc_ptr;
				if ((ralloc_ptr = realloc(lines_array,
										  sizeof(char*) * 10 * main_allocs)) ==
					NULL) {
					printf("couldn't reallocate requested memory");
					break;
				}
				else {
					lines_array = ralloc_ptr;
					main_allocs++;
				}
			}
		}
	}
	if (error_code == ERROR) {
		printf("lacking memory to complete requested operation, aborting");
	}
	else {
		printOutput(lines_array);
	}

	for (int x = 0; x < how_many_lines; x++) {
		if (lines_array[x] != NULL) {
			free(lines_array[x]);
		}
	}
	free(lines_array);
	return 0;
}
