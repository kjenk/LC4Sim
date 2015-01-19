#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_LINE_LENGTH         512
#define MAX_WORD  120

char *uppercase(char *input) {
  char *out = malloc((strlen(input) + 1) * sizeof(char));
  char *label = input;
  int i = 0;
  int j = 0;

  for (i; label[i]; i++) {
    if (isalpha(label[i])) {
      out[j] = toupper(label[i]);
      j++;
    }
    else{
      fprintf(stderr,"invalid line\n");
    }
  }
  //out[j] = '\0';
  return out;
}

int main(int argc, char **argv)
{
  FILE *file;
  char input[MAX_LINE_LENGTH];
  char name[MAX_WORD];
  char label;
  signed int value;
  char *uname;
  list_elt *node = NULL;
  list_elt *first_node = NULL;
      char *fname;
      signed int fvalue ;
  list_elt *dude = NULL;
  list_elt *end_node = NULL;

  file = fopen(argv[1],"r");
  if (!file) {
    printf("Cannot find file\n");
    exit(1);
  }
    while (fgets (input, sizeof(input), file)) {  
	    if (sscanf(input, "%s %d", name, &value) == 2) {
	     

         uname = uppercase(name);


			 node = lookup (first_node, uname);
       end_node = end(first_node);

      //  printf("%s %d\n", uname, value);
     
       if(node != NULL){
    fname = rname(first_node);
    fvalue = rvalue(first_node);
            first_node = update(first_node,node,value); 
                 //    printf("%s %d\n", uname, value);


      //      printf("this should work\n");

                fname = rname(first_node);
    fvalue = rvalue(first_node);
        } 

		   if(node== NULL && value>0){
        
            first_node=push(first_node,uname,value,end(first_node));
                  // printf("%s %d\n", uname, value);
         //   printf("tmaybe\n");
          
      
	     }


		}


	}//while fgets



while(first_node!=NULL){
    fname = rname(first_node);
    fvalue = rvalue(first_node);
    printf("%s %d\n", fname, fvalue);
    dude = shift(first_node);
    first_node= loop(first_node, dude);


}
  


      exit(0);
  }



