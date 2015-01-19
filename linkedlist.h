//linklist

typedef struct list_elt_tag {
  char *name;
  signed int number;  
  struct list_elt_tag *prev, *next; // Pointers to the previous and next elements in the list
} list_elt;

list_elt *lookup (list_elt *first_elt, char *name)
{
  list_elt *elt = first_elt;
  // Iterate down the linked list
  while (elt != NULL) {
    // Compare the name using strcmp
    if (strcmp (name, elt->name) == 0)
      return elt;
    elt = elt->next;
  }
  /* If you didn't find it return NULL */
  return NULL;
}

/* 
 * Creates a new list element and pushes it on the front of the list
 * returns a pointer to the newly created element.
 */

list_elt *push (list_elt *first_elt, char *name, signed int number, list_elt *end)
{
  list_elt *elt;

  // Allocate a new list element with malloc
  elt = malloc (sizeof(*elt));

  // If malloc fails end the program
  if (elt == NULL) {
    printf ("Couldn't allocate a new list element\n");
    exit (1);
  }

  // Allocate space to store the name
  elt->name = malloc(strlen(name) + 1);

  // If malloc fails end the program
  if (elt->name == NULL) {
    printf ("Couldn't allocate a new name\n");
    exit (1);
  }

  // Copy the name
  strcpy (elt->name, name);

  elt->number = number;

    if(first_elt==NULL){
        elt->prev=NULL; 
        elt->next=NULL; 
        return elt;
      } 
    end->next=elt;
    elt->prev=end; 
    elt->next=NULL;  

  return first_elt;
}

/*
 * delete : Deletes an element from the list returns a pointer to the new
 * first element of the list which may just be the old first element.
 */

list_elt *update (list_elt *first_elt, list_elt *elt,  int value)
{  list_elt *prev, *next;
   elt->number=((elt->number)+value);
  prev = elt->prev;
  next = elt->next;
  /* First we fix the pointers of the next and previous elements */
 if(elt->number <= 0){

  if (prev){
    prev->next = elt->next;
  }
  if (next){
    next->prev = elt->prev;
  }
  // Free the space we allocated for the name
  free (elt->name);

  // Free the space we allocated for the list element
  free (elt);
}//remove completely
  // Check if elt was the first element in the list
  if (elt == first_elt){
    if(first_elt-> number > 0){
      return first_elt;
    }
    return next;
  }
  else{

    return first_elt;
  }
}

char *rname (list_elt *first_node)
{
        char *lname = first_node-> name ;
        return lname;
}

int rvalue (list_elt *first_node)
{
        int lvalue = first_node-> number ;
        return lvalue;
}
list_elt *shift (list_elt *first_node){
        list_elt *node;
        node = first_node->next;
        return node;
}

list_elt *loop (list_elt *first_node, list_elt *node)
{
      free(first_node->name);
      free(first_node);
      first_node = node; 
      return first_node;
}

list_elt *end(list_elt *first_node){
  if(first_node == NULL){
      return first_node;
    }
  else{

    while(first_node->next!=NULL)
    {
      first_node=first_node->next;
    } 
    return first_node;
  }
}

