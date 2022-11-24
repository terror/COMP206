/*
  Name:    Liam Scalzulli
  Program: Computer Science
  Faculty: Arts
  Email:   liam.scalzulli@mail.mcgill.ca
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * All available commands the interpreter
 * can evaluate.
 */
enum Command {
  DELETE,
  END,
  INSERT,
  INVALID,
  LIST
};

/*
 * A struct array that corresponds a `Command`
 * enum with a string.
 */
const static struct {
  enum Command command;
  const char *str;
} COMMAND_STRING [] = {
  { DELETE, "D"    },
  { END,    "END"  },
  { INSERT, "I"    },
  { LIST,   "LIST" }
};

/*
 * Find a corresponding `Command` enum from a specified string.
 *
 * @param str The string to associate with a `Command` enum.
 * @return The corresponding `Command` enum.
 */
enum Command command_from_string(char *str) {
  int size = (
    sizeof(COMMAND_STRING) /
    sizeof(COMMAND_STRING[0])
  );

  for (int i = 0; i < size; ++i)
    if (!strcmp(str, COMMAND_STRING[i].str))
      return COMMAND_STRING[i].command;

  return INVALID;
}

/*
 * Represents an *operation* which is essentially a command
 * that contains its arguments and its original name.
 *
 * Note: the maximum amount of arguments a command can have is 5.
 */
struct Operation {
  char *name;
  enum Command cmd;
  char *args[5];
};

/*
 * Definition for a linked list node given
 * for this assignment.
 */
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

/*
 * Container for a linked list.
 *
 * Contains a single pointer to the head
 * node of the list.
 */
struct List {
  struct PersonalInfoRecord *head;
};

/*
 * Insert a node into a linked list.
 *
 * @param list The linked list to operate on.
 * @param node The node to insert.
 */
void insert(struct List *list, struct PersonalInfoRecord *node) {
  struct PersonalInfoRecord *curr = list->head;

  if (curr == NULL) {
    list->head = node;
    return;
  }

  while (curr->next != NULL) {
    if (curr->id == node->id)
      printf("Found existing node!");
    curr = curr->next;
  }

  curr->next = node;
}

/*
 * Delete a node from this linked list.
 *
 * @param list The list to operate on.
 * @param node The node to delete.
 */
void delete(struct List *list, struct PersonalInfoRecord *node) {
  if (list->head == NULL)
    return;

  struct PersonalInfoRecord *curr = list->head, *prev;

  if (curr != NULL && curr->id == node->id) {
    list->head = curr->next;
    free(curr);
    return;
  }

  while (curr != NULL && curr->id != node->id) {
    prev = curr;
    curr = curr->next;
  }

  if (curr == NULL)
    return;

  prev->next = curr->next;

  free(curr);
}

/*
 * Clear the linked list (freeing all its allocated memory).
 *
 * @param list The list to clear.
 */
void clear(struct List *list) {
  struct PersonalInfoRecord *curr = list->head, *prev;

  while (curr != NULL) {
    if (prev != NULL) free(prev);
    prev = curr;
    curr = curr->next;
  }
}

/*
 * Print all node information from the specified
 * linked list.
 *
 * @param list The list to print.
 */
void print(struct List list, FILE *fptr) {
  struct PersonalInfoRecord *curr = list.head;

  while (curr != NULL) {
    fprintf(
      fptr,
      "%s,%c,%s,%s,%d",
      curr->id,
      curr->ptype,
      curr->name,
      curr->ptype == 'S' ?
      curr->info.stud.faculty :
      curr->info.prof.dept,
      curr->ptype == 'S' ?
      curr->info.stud.admyear :
      curr->info.prof.hireyear
    );
    curr = curr->next;
  }
}

/*
 * Dump the contests of the list to the database file.
 *
 * @param list The list to work with.
 * @param db The name of the database file.
 */
void dump(struct List list, char *db) {
  FILE *file = fopen(db, "w");

  if (file == NULL) {
    printf("error: Unable to open file %s for writing.", db);
    exit(3);
  }

  print(list, file);

  clear(&list);
}

/*
 * Convert an operation to a valid linked list node
 * of type `PersonalInfoRecord`.
 *
 * Note: arguments should always be valid,
 * i.e. [id, ptype, name, ..] <-- the rest depending
 * on whether its a student or a professor.
 *
 * @param op The operation to convert.
 */
struct PersonalInfoRecord* to_node(struct Operation op) {
  char
    id = *op.args[0],
    ptype = *op.args[1],
    name = *op.args[2];

  struct PersonalInfoRecord *node =
    (struct PersonalInfoRecord*)
    malloc(sizeof(struct PersonalInfoRecord));

  strcpy(node->id, &id);
  node->ptype = ptype;
  strcpy(node->name, &name);

  return node;
}

/*
 * The line parser, responsible for turning lines
 * into valid `Operation` structs.
 */
struct Parser {
  char line[2048];
};

/*
 * Read in and set a line on the parser from
 * standard input.
 *
 * @param parser A pointer to a parser.
 */
void read(struct Parser *parser) {
  fgets(parser->line, 2048, stdin);
  parser->line[strcspn(parser->line, "\n")] = 0;
}

/*
 * Parse the current set line into an `Operation`.
 *
 * @param parser A parser.
 */
struct Operation parse(struct Parser parser) {
  char *token = strtok(parser.line, ",");

  struct Operation operation;

  operation.name = strdup(token);
  operation.cmd = command_from_string(token);

  int index = 0;

  for (;;) {
    token = strtok(NULL, ",");
    if (!token) break;
    operation.args[index++] = strdup(token);
  }

  return operation;
}

/*
 * The 'interpreter', responsible for the evaluation
 * of 'operations', i.e. a command + argument combination.
 */
struct Interpreter {
  char *db;
  struct List list;
  struct Operation op;
};

/*
 * Load an operation onto the interpreter.
 *
 * @param i A pointer to an interpreter.
 * @param op The operation to set.
 */
void load(struct Interpreter *i, struct Operation op) {
  i->op = op;
}

/*
 * Evaluate the currently loaded operation.
 *
 * @param i A pointer to an interpreter.
 */
void eval(struct Interpreter *i) {
  switch (i->op.cmd) {
    case DELETE:
      delete(&i->list, to_node(i->op));
      break;
    case END:
      dump(i->list, i->db);
      break;
    case INSERT:
      insert(&i->list, to_node(i->op));
      break;
    case INVALID:
      printf("error: Invalid command `%s`.\n", i->op.name);
      break;
    case LIST:
      print(i->list, stdout);
      break;
  }
}

/*
 * The program entrypoint.
 */
int main(int argc, char *argv[]) {
  struct Parser parser;

  if (argc < 2) {
    fprintf(stderr, "error: Missing database filename\n");
    fprintf(stderr, "usage: ./pimapp <dbfile>\n");
    exit(1);
  }

  struct Interpreter interpreter = {
    .db = argv[0],
    .list = { .head = NULL }
  };

  for (;;) {
    // Read in a line from stdin
    read(&parser);
    // Load in an operation onto the interpreter
    load(&interpreter, parse(parser));
    // Evaluate the current operation
    eval(&interpreter);
  }

  return 0;
}
