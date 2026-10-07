// Element of expression
struct entity
{
  char* word;
  int value;  // this number consists indexes of value letters
  int len;
};

int parser(struct entity list_entities[]);
void parse_expression (struct entity* list_entities,int count_entites, char* expression);
int set_index_letter(struct entity list_entities[], int count_entities, int* no_zero);