#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

int letter_indexes[26] = {0};

void parse_expression (struct entity* list_entities,int count_entites, char* expression)
{
  char* sep = " ";
  for (int i = 0; i < count_entites; i++)
  {
    if (!i)
      list_entities[i].word = strtok(expression, sep);
    else
      list_entities[i].word = strtok(NULL, sep);
  }
}

int parser(struct entity list_entities[])
{
  char symbol = '0'; 
  int is_end = 0;
  int len_expression = 10;
  char *expression = (char*)malloc(len_expression);
  int count_symbols = 0;
  int count_entites = 0;

  do
  {
    symbol = getc(stdin);
    if (symbol == ' ')
      continue;
    if (symbol == '=' || symbol == '+' || symbol == '\n')
      {
        is_end = (symbol == '\n');
        symbol = ' ';
        count_entites++;
      }
    if (len_expression < count_symbols)
    {
      len_expression *= 2;
      char* tmp = realloc(expression, len_expression);
      expression = tmp;
    }
    expression[count_symbols++] = symbol;
  } while (!is_end);

  parse_expression(list_entities, count_entites, expression);

  // free(expression);
  return count_entites;
}

int set_index_letter(struct entity list_entities[], int count_entities, int* no_zero)
{
  int unique_letters = 0;
  for (int i = 0; i < count_entities; i++)
  {
    int letter_index = 0;
    while (list_entities[i].word[letter_index])
    {
      int letter = list_entities[i].word[letter_index++] - 'A';
      if (!letter_indexes[letter])
      {
        unique_letters++;
        letter_indexes[letter] = unique_letters;
      }
      list_entities[i].value *= 10;
      list_entities[i].value += letter_indexes[letter]-1;
    }
    if (letter_index != 1)
      no_zero[letter_indexes[list_entities[i].word[0]-'A']-1] = 1;
    list_entities[i].len = letter_index;
  }
  return unique_letters;
}
