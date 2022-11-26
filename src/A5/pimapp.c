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
 * Represents an *operation* which is essentially a command
 * that contains its arguments and its original name.
 *
 * Note: the maximum amount of arguments a command can have is 6.
 */
struct Operation {
  char *name;
  enum Command cmd;
  char *args[6];
};

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
    *id   = op.args[0],
    *type = op.args[1],
    *name = op.args[2];

  struct PersonalInfoRecord *node =
    (struct PersonalInfoRecord*)
    malloc(sizeof(struct PersonalInfoRecord));

  strcpy(node->id, id);

  node->ptype = *type;

  strcpy(node->name, name);

  if (*type == 'S') {
    strcpy(node->info.stud.faculty, op.args[3]);
    node->info.stud.admyear = atoi(op.args[4]);
  } else {
    strcpy(node->info.prof.dept, op.args[3]);
    node->info.prof.hireyear = atoi(op.args[4]);
    node->info.prof.tenured = *op.args[5];
  }

  return node;
}

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
 * Update a linked list nodes' fields.
 *
 * Note: this only updates non-empty fields.
 *
 * @param source The source node.
 * @param dest The node to update.
 */
void update(
  struct PersonalInfoRecord *source,
  struct PersonalInfoRecord *dest
) {
  if (strlen(dest->name) > 0)
    strcpy(dest->name, source->name);

  if (dest->ptype == 'S') {
    if (strlen(dest->info.stud.faculty) > 0)
      strcpy(dest->info.stud.faculty, source->info.stud.faculty);
    if (dest->info.stud.admyear != 0)
      dest->info.stud.admyear = source->info.stud.admyear;
  } else {
    if (strlen(dest->info.prof.dept) > 0)
      strcpy(dest->info.prof.dept, source->info.prof.dept);
    if (dest->info.prof.hireyear != 0)
      dest->info.prof.hireyear = source->info.prof.hireyear;
    if (dest->info.prof.tenured != 0)
      dest->info.prof.tenured = source->info.prof.tenured;
  }
}

/*
 * Insert a node into a linked list.
 *
 * @param list The linked list to operate on.
 * @param node The node to insert.
 */
void insert(struct List *list, struct PersonalInfoRecord *node) {
  struct PersonalInfoRecord *curr = list->head, *prev = NULL;

  if (curr == NULL) {
    list->head = node;
    return;
  }

  if (curr->next == NULL && !strcmp(curr->id, node->id)) {
    update(node, curr);
    return;
  }

  while (curr != NULL && atoi(curr->id) <= atoi(node->id)) {
    if (!strcmp(curr->id, node->id)) {
      update(node, curr);
      return;
    }
    prev = curr;
    curr = curr->next;
  }

  if (prev == NULL) {
    node->next = curr;
    list->head = node;
  } else if (curr == NULL) {
    prev->next = node;
  } else {
    node->next = prev->next;
    prev->next = node;
  }
}

/*
 * Delete a node from this linked list.
 *
 * @param list The list to operate on.
 * @param node The node to delete.
 */
void delete(struct List *list, char *id) {
  if (list->head == NULL)
    return;

  struct PersonalInfoRecord *curr = list->head, *prev;

  if (curr != NULL && !strcmp(curr->id, id)) {
    list->head = curr->next;
    free(curr);
    return;
  }

  while (curr != NULL && strcmp(curr->id, id)) {
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
  struct PersonalInfoRecord *curr;

  while (list->head != NULL) {
    curr = list->head;
    list->head = list->head->next;
    free(curr);
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
      "%s,%c,%s,%s,%d%c%c\n",
      curr->id,
      curr->ptype,
      curr->name,
      curr->ptype == 'S' ?
      curr->info.stud.faculty :
      curr->info.prof.dept,
      curr->ptype == 'S' ?
      curr->info.stud.admyear :
      curr->info.prof.hireyear,
      curr->ptype == 'P' &&
      curr->info.prof.tenured != 0 ?
      ',' :
      ' ',
      curr->ptype == 'P' ?
      curr->info.prof.tenured :
      ' '
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
void dump(struct List *list, char *db) {
  FILE *file = fopen(db, "w");

  // Couldn't open it
  if (file == NULL) {
    printf("error: Unable to open file %s for writing.\n", db);
    exit(3);
  }

  // Write to the file
  print(*list, file);

  // Close the file
  fclose(file);

  // Free all memory
  clear(list);

  // Exit the program
  exit(0);
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
      delete(&i->list, i->op.args[0]);
      break;
    case END:
      dump(&i->list, i->db);
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
    .db = argv[1],
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
