#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parser.h"
#include <math.h>


int list_letters[26];
int no_zero[10];
int success;
int count_entities;
struct entity list_entities[8];

void null_vars ()
{
  memset(list_letters, 0, 26*sizeof(int));
  memset(no_zero, 0, 10*sizeof(int));
  memset(list_entities, 0, 8*sizeof(struct entity));
  memset(letter_indexes, 0, 26*sizeof(int));
  success = 0;
  count_entities = 0;
}

void print_result(int* result) 
{
  for (int i = 0; i < count_entities-1; i++) 
  { 
    if (i)
      printf("+ %s ", list_entities[i].word);
    else
      printf("%s ", list_entities[i].word);
  }
  printf("= %s\n", list_entities[count_entities-1].word);


  for (int i = 0; i < count_entities-1; i++) 
  { 
    if (i)
      printf("+ %d ", result[i]);
    else
      printf("%d ", result[i]);
  }
  printf("= %d\n", result[count_entities-1]);
}

int get_entity_value(int* permutation, int entity_index)
{
  int value = 0;
  int number = list_entities[entity_index].value;
  for (int i = 0; i < list_entities[entity_index].len; i++)
  {
    value += permutation[(number % 10)]*(int)pow(10, i);
    number /= 10;
  }
  return value;
}

int check_permutation(int* permutation, int count_entities)
{
  int* result = calloc(count_entities, sizeof(int));
  int sum_results = 0;
  for (int i = 0; i < count_entities-1; i++)
  {
    result[i] = get_entity_value(permutation, i);
    sum_results += result[i];
  }
  result[count_entities-1] = get_entity_value(permutation, count_entities-1);
  if (sum_results == result[count_entities-1])
    {
      print_result(result);
      return 1;
    }
  return 0;
}

void permutations(int* permutation, int unique_letters, int current_len, int* used)
{
  if (success) return;
  if (current_len == unique_letters)
  {
    if (check_permutation(permutation, count_entities))
      success = 1;
    return;
  }
  for (int i = 0; i < 10; i++)
  {
    if (!used[i] && (!no_zero[current_len] + i))
    {
      used[i] = 1;
      permutation[current_len] = i;
      permutations(permutation, unique_letters, current_len+1, used);
      used[i] = 0;
    }
  }
}

int main (int argc, char* argv[]) 
{
  FILE* input = (argc > 1) ? fopen(argv[1], "r") : stdin;
  int count = (argc > 1) ? atoi(argv[2]) : 1;

  while (count)
  {
    null_vars();
    clock_t start = clock();
    count_entities = parser(list_entities, input);
    int* permutation;
    int* used;
    int unique_letters;

    if (!count_entities)
      printf("Error in parser");

    unique_letters = set_index_letter(list_entities, count_entities, no_zero);
    if (unique_letters > 10)
      printf("%d too many unique letters", unique_letters);

    permutation = malloc(sizeof(int)*unique_letters);
    used = calloc(10, sizeof(int));

    permutations(permutation, unique_letters, 0, used);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("solve time: %lf\n\n", cpu_time_used); 

    free(list_entities[0].word);
    free(permutation);
    free(used);
    count--;
  } 

  return 0;
}
