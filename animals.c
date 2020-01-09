#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_utils.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

NODE *root = NULL;




// Returns true if user types "yes" or "y" (upper or lower case)
// and returns false if user types "no" or "n". Keeps
// prompting otherwise.

BOOL yes_response()
{
	char ans[100];
	fgets(ans, 100, stdin); //Store user input in ans
  ans[strlen(ans)-1] = 0;

	ans[strcspn(ans, "\n")] = '\0'; //Remove newline attached to end of ans

	char s1[] = "yes"; //Use s to compare with ans
  char s2[] = "y";

	int result = strcasecmp(s1, ans);
  int result2 = strcasecmp(s2, ans);

  char s3[] = "no";
  char s4[] = "n";

  int result3 = strcasecmp(s3, ans);
  int result4 = strcasecmp(s4, ans);

	if(result==0 || result2==0){
		return TRUE;
	}
	if(result3==0 || result4==0){
		return FALSE;
	}
  else{
    printf("Invalid input. Try again. \n");
    yes_response(); //keep calling yes_reponse until user has valid input
  }

  //Hint: You might consider using the C library function strcasecmp()

}

// This procedure creates a new NODE and copies
// the contents of string s into the
// question_or_animal field.  It also initializes
// the left and right fields to NULL.
// It should return a pointer to the new node

NODE *new_node(char *s)
{
	NODE *newn = (NODE *) malloc(sizeof(NODE));
	strcpy((*newn).question_or_animal, s);
	newn->left = NULL;
	newn->right = NULL;
	return newn;
}

// This is the procedure that performs the guessing.
// If the animal is not correctly guessed, it prompts
// the user for the name of the animal and inserts a
// new node for that animal into the tree.

void guess_animal()
{

  char q[500];
  char a[500];
  // If the root is NULL, then there are no animals in
  // the tree. Prompt the player for the name of an
  // animal, create a node for that animal, and make
  // that node the root of the tree.
  NODE *p = root;
  //p is a pointer used to traverse the tree without altering the root

  if(root==NULL){
    printf("Enter the name of an animal. > ");
    fgets(q, 500, stdin);
    q[strlen(q)-1] = 0;
    NODE *newanimal = new_node(q);
    *root = *newanimal;
  }

    /*
      Otherwise (i.e. if the root is not NULL)
      Then descend down the tree. At each node encountered:

      If the left and right fields of the node are not NULL, i.e. the node
      is an interior node and must contain a question, then:
      - print the question_or_answer field, which will be a yes/no question.
      - read the response from the user
      - If the response is yes, follow the left field to the next node. Otherwise,
        follow the right field.
    */


  if(p != NULL){
    //keep traversing until we reach a leaf
    while(p->left != NULL & p->right != NULL){
      //this is an interior node containing a question
      printf("%s > ", (*p).question_or_animal);
      if(yes_response()){
        //go LEFT
        p = p->left;
      }
      else{
        //go RIGHT
        p = (p->right);
      }
    }



  /*
      Otherwise, if the left and right fields are NULL (i.e. the node is a leaf),
      then the question_or_animal field contains an animal. Print the
      the question_or_animal field as the guess and prompt the user
      if the guess was correct. If the guess was correct, then
      return. Otherwise, do the following:
           - prompt the user for the name of the new animal she was thinking of
           - prompt the user for a yes or no question distinguishing the new animal from the guessed animal
     - ask the user what the answer for the new animal would be (yes or no)
           - create a new node with the new animal in the question_or_animal field
     - create another new node that has the guessed animal in the question_or_animal field
           - make the two new nodes the children of the current node, where the animal for which the
             answer to the question is "yes" is the left child and the other animal is the right child.
           - overwrite the question_or_animal field of the current node with the new question.

      Note that the tree is a stricly binary tree, i.e. left and right fields of each node are either both NULL or
      both valid pointers to child nodes (in other words, each node is a leaf or has exactly two children).
    */

  if(p->left == NULL & p->right == NULL){
    //reached a leaf, now guess
    printf("Is it a %s? > ", (*p).question_or_animal);
    if(yes_response()){
      printf("I win!");
      return;
    }
    else{
      printf("What animal were you thinking of? > ");
      fgets(a, 500, stdin);
      a[strlen(a)-1] = 0;
      NODE *newanimal = new_node(a);

      printf("Enter a question that distinguishes your animal from a %s > ", (*p).question_or_animal);
      fgets(q, 500, stdin);
      q[strlen(q)-1] = 0;

      NODE *current = new_node((*p).question_or_animal);

      printf("Is the answer to your question yes or no? > ");
      if(yes_response()){
        p->left = newanimal;
        p->right = current;
      }
      else{
        p->left = current;
        p->right = newanimal;
      }
      strcpy((*p).question_or_animal, q);
      return;

    }
  }

}
}



//This code is complete. Just add comments where indicated.

int main()
{ int i;
  BOOL done = FALSE;

  //insert comment here
  FILE *datafile = fopen("data.dat", "r");

  if (datafile == NULL) {
    printf("data.dat not found\n");
    exit(1);
  }

  //insert comment here
  FILE *backupfile = fopen("data.dat.bak", "w");

  //insert comment here
  root = read_tree(datafile);

  //call write_tree() to write the initial tree to the
  //backup file (i.e. to backup the tree data)
  write_tree(root,backupfile);

  //close both files (for now)
  fclose(backupfile);
  fclose(datafile);


  printf("Welcome to the animal guessing game (like 20 questions).\n");
  do {
    printf("\nThink of an animal...\n");
    guess_animal();  //traverses the tree saved into datafile starting from the root
    printf("\nDo you want to play again? (yes/no) > ");
  } while (yes_response());  //keeps the game running without having to run ./animals manually


  //now open the "data.dat" file for writing
  datafile = fopen("data.dat", "w");

  //saves tree into datafile for next time the game is run
  write_tree(root, datafile);

  //close the data.dat file
  fclose(datafile);

}
