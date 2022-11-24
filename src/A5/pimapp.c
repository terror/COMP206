/*
  Name:    Liam Scalzulli
  Program: Computer Science
  Faculty: Arts
  Email:   liam.scalzulli@mail.mcgill.ca
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Definition for a linked list node.
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
 */
struct List {
  struct PersonalInfoRecord *head;
};

/*
 * Insert a node into this linked list.
 */
void insert(struct List *list) {}

/*
 * Delete a node from this linked list.
 */
void delete(struct List *list) {}

/*
 * Print all node information.
 */
void print(struct List list) {}

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
 * @return The correspoding `Command` enum.
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
 * Represents an 'operation', which is essentially a command
 * that contains its arguments and its original name.
 */
struct Operation {
  char *name;
  enum Command cmd;
  char *args[5];
};

/*
 * The line parser, responsible for turning lines
 * into valid `Operation` structs.
 */
struct Parser {
  char line[LINE_MAX];
};

/*
 * Read in and set a line on the parser from
 * standard input.
 */
void read(struct Parser *parser) {
  fgets(parser->line, LINE_MAX, stdin);
  parser->line[strcspn(parser->line, "\n")] = 0;
}

/*
 * Parse the current set line into an `Operation`.
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
  struct List list;
  struct Operation op;
};

/*
 * Load an operation onto the interpreter.
 */
void load(struct Interpreter *i, struct Operation op) {
  i->op = op;
}

/*
 * Evaluate the currently loaded operation.
 */
void eval(struct Interpreter *i) {
  switch (i->op.cmd) {
    // Turn `args` into a correct PersonalInfoRecord
    // then call `delete` on the list.
    case DELETE:
      break;
    case END:
      exit(0);
    // Turn `args` into a correct PersonalInfoRecord
    // then call `insert` on the list.
    case INSERT:
      break;
    case INVALID:
      printf("error: Invalid command `%s`.\n", i->op.name);
      break;
    // Traverse and print all list information
    case LIST:
      break;
  }
}

/*
 * The program entrypoint.
 */
int main(int argc, char *argv[]) {
  struct Parser parser;

  struct Interpreter interpreter = {
    .list = {
      .head = NULL
    }
  };

  // Require a passed in database file
  if (argc < 2) {
    fprintf(stderr, "error: Missing database filename\n");
    fprintf(stderr, "usage: ./pimapp <dbfile>\n");
    exit(1);
  }

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
