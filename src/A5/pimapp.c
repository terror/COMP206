/*
  Name:    Liam Scalzulli
  Program: Computer Science
  Faculty: Arts
  Email:   liam.scalzulli@mail.mcgill.ca
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Command {
  Delete,
  End,
  Insert,
  List
};

typedef struct PersonalInfoRecord {
  char id[10];
  char ptype;
  char name[31];

  union {
    struct {
      char dept[31];
      int hireyear;
      char tenured;
    } prof;
    struct {
      char faculty[31];
      int admyear;
    } stud;
  } info;

  struct PersonalInfoRecord *next;
} PersonalInfoRecord;

int main(int argc, char *argv[]) {
  char inputbuffer[100], *input;

  if (argc < 2) {
    fprintf(stderr, "error: Missing database filename\n");
    fprintf(stderr, "usage: ./pimapp <dbfile>\n");
    exit(1);
  }

  while (fgets(input=inputbuffer, 100, stdin) != NULL) {
    if(strncmp(input, "END", 3) == 0) break;
    int field = 1; char *data;
    while ((data = strsep(&input, ",\n")) != NULL) {
      printf("Field %d is %s\n", field++, data);
    }
  }

  return 0;
}
