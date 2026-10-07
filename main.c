#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <math.h>


int list_letters[26] = {0};
int no_zero[10] = {0};
int count_entities;
struct entity list_entities[8];


void print_result(int* result) {

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
  if (current_len == unique_letters)
  {
    if (check_permutation(permutation, count_entities))
      exit(0);
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

int main () 
{
  count_entities = parser(list_entities);
  int* permutation;
  int* used;

  if (!count_entities)
    printf("Error in parser");

  int unique_letters = set_index_letter(list_entities, count_entities, no_zero);
  if (unique_letters > 10)
    printf("%d too many unique letters", unique_letters);

  permutation = malloc(sizeof(int)*unique_letters);
  used = calloc(10, sizeof(int));

  permutations(permutation, unique_letters, 0, used);

  free(list_entities[0].word);
  return 0;
}