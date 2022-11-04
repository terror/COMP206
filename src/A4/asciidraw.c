#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

const int MAX_ARGS = 4;

enum Command {
  CHAR,
  CIRCLE,
  DISPLAY,
  GRID,
  INVALID,
  LINE,
  RECTANGLE,
};

const static struct {
  enum Command command;
  const char *str;
} command_string [] = {
  {CHAR, "CHAR"},
  {CIRCLE, "CIRCLE"},
  {DISPLAY, "DISPLAY"},
  {GRID, "GRID"},
  {LINE, "LINE"},
  {RECTANGLE, "RECTANGLE"}
};

enum Command command_from_string(const char *str) {
  for (int i = 0; i < sizeof(command_string) / sizeof(command_string[0]); ++i)
    if (!strcmp(str, command_string[i].str))
      return command_string[i].command;
  return INVALID;
}

struct Operation {
  enum Command cmd;
  int *args;
};

struct Grid {
  char **state;
  char character;
  int width;
  int height;
};

void character(struct Grid *grid, int args[]) {
  grid->character = args[0];
}

void circle(struct Grid *grid, int args[]) {
  int x = args[0], y = args[1], radius = args[2];
  printf("x: %d, y: %d, radius: %d\n", x, y, radius);
}

void display(struct Grid grid) {
  for (int i = 0; i < grid.width; ++i) {
    for (int j = 0; j < grid.height; ++j)
      printf("%c", grid.state[i][j]);
    printf("\n");
  }
}

void grid(struct Grid *grid, int args[]) {
  int width = args[0], height = args[1];

  grid->state = (char**)malloc(sizeof(char*)*width);

  for (int i = 0; i < width; ++i)
    grid->state[i] = (char*)malloc(sizeof(char)*height);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      grid->state[i][j] = grid->character;

  grid->width = width;
  grid->height = height;
}

void line(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];
  printf("x1: %d, y1: %d, x2: %d, y2: %d\n", x1, y1, x2, y2);
}

void rectangle(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];
  printf("x1: %d, y1: %d, x2: %d, y2: %d\n", x1, y1, x2, y2);
}

struct Lexer {
  char line[LINE_MAX];
};

void read(struct Lexer *lexer) {
  fgets(lexer->line, LINE_MAX, stdin);
  lexer->line[strcspn(lexer->line, "\n")] = 0;
}

struct Operation lex(struct Lexer lexer) {
  char *token = strtok(lexer.line, " ");

  struct Operation operation;

  operation.cmd = command_from_string(token);

  int index = 0, args[MAX_ARGS];

  while (1) {
    token = strtok(NULL, " ");
    if (token == NULL) break;
    args[index++] = atoi(token);
  }

  operation.args = args;

  return operation;
}

struct Interpreter {
  struct Grid grid;
  struct Operation op;
};

void load(struct Interpreter *interpreter, struct Operation op) {
  interpreter->op = op;
}

void eval(struct Interpreter *i) {
  switch(i->op.cmd) {
    case CHAR:
      character(&i->grid, i->op.args);
      break;
    case CIRCLE:
      circle(&i->grid, i->op.args);
      break;
    case GRID:
      grid(&i->grid, i->op.args);
      break;
    case DISPLAY:
      display(i->grid);
      break;
    case INVALID:
      printf("error: Invalid operation\n");
      break;
    case LINE:
      line(&i->grid, i->op.args);
      break;
    case RECTANGLE:
      rectangle(&i->grid, i->op.args);
      break;
  }
}

int main() {
  struct Grid grid;

  grid.character = '*';

  struct Lexer lexer;
  struct Interpreter interpreter;

  interpreter.grid = grid;

  for (;;) {
    // Display the prompt
    printf("> ");

    // Read a line in from stdin
    read(&lexer);

    // Load the operation onto the interpreter
    load(&interpreter, lex(lexer));

    // Evaluate the currently loaded operation
    eval(&interpreter);
  }
}
