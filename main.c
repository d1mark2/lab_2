#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parser.h"
#include <math.h>


int list_letters[26];
int no_zero[10];
int min_digit[10];
int max_digit[10];
int pow10[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
int success;
int count_entities;
struct entity list_entities[8];


void null_vars ()
{
  memset(list_letters, 0, 26*sizeof(int));
  memset(no_zero, 0, 10*sizeof(int));
  memset(list_entities, 0, 8*sizeof(struct entity));
  memset(letter_indexes, 0, 26*sizeof(int));
  memset(min_digit, 0, 10*sizeof(int));
  memset(max_digit, 0, 10*sizeof(int));
  success = 0;
  count_entities = 0;
}

int get_entity_value(int* permutation, int entity_index)
{
  int value = 0;
  int number = list_entities[entity_index].value;
  for (int i = 0; i < list_entities[entity_index].len; i++)
  {
    value += permutation[(number % 10)]*pow10[i];
    number /= 10;
  }
  return value;
}

void print_result(int* permutation) 
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
      printf("+ %d ", get_entity_value(permutation, i));
    else
      printf("%d ", get_entity_value(permutation, i));
  }
  printf("= %d\n", get_entity_value(permutation, count_entities-1));
}



int get_entity_digit(int* permutation, int entity_index, int index)
{
  int number = list_entities[entity_index].value;
  number /= pow10[index];
  if (list_entities[entity_index].len <= index)
    return 0;
  return permutation[(number % 10)];
}

int check_permutation(int* permutation, int count_entities)
{
  int digit_sum = 0;
  int digit_answer = 0;

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < count_entities-1; j++)
    {
      digit_sum += get_entity_digit(permutation, j, i);
    }
    digit_answer = get_entity_digit(permutation, count_entities-1, i);

    if ((digit_sum % 10) == digit_answer)
      digit_sum /= 10;
    else 
      return 0;
  }
  return 1;
}

void set_digit_bounds(int unique_letters)
{
  for (int i = 0; i < unique_letters; i++)
  {
    min_digit[i] = no_zero[i] ? 1 : 0;
    max_digit[i] = 9;
  }

  int end = count_entities - 1;
  int max_len = 0;
  for (int i = 0; i < end; i++)
    if (list_entities[i].len > max_len)
      max_len = list_entities[i].len;

  struct entity* result = &list_entities[count_entities - 1];
  if (result->len > max_len)
  {
    int lead = result->value / pow10[result->len - 1];
    min_digit[lead] = 1;
    max_digit[lead] = end - 1;
  }
}

int permutations(int* permutation, int unique_letters, int current_len, int* used)
{
  if (success) return success; 
  if (current_len == unique_letters)
  {
    if (check_permutation(permutation, count_entities))
      success = 1;
    return success;
  }
  for (int i = min_digit[current_len]; i <= max_digit[current_len] && !success; i++)
  {
    if (!success && !used[i])
    {
      used[i] = 1;
      permutation[current_len] = i;
      success = permutations(permutation, unique_letters, current_len+1, used);
      used[i] = 0;
    }
  }
  return success;
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
    set_digit_bounds(unique_letters);
    if (unique_letters > 10)
      printf("%d too many unique letters", unique_letters);

    permutation = malloc(sizeof(int)*unique_letters);
    used = calloc(10, sizeof(int));

    success = permutations(permutation, unique_letters, 0, used);

    clock_t end = clock();

    if (success)
      print_result(permutation);
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("solve time: %lf\n\n", cpu_time_used); 

    free(list_entities[0].word);
    free(permutation);
    free(used);
    count--;
  } 

  return 0;
}
