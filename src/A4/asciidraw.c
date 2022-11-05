#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

const int MAX_ARGS = 4;

enum Command {
  CHAR,
  CIRCLE,
  DISPLAY,
  END,
  GRID,
  INVALID,
  LINE,
  RECTANGLE,
};

const static struct {
  enum Command command;
  const char *str;
} COMMAND_STRING [] = {
  {CHAR, "CHAR"},
  {CIRCLE, "CIRCLE"},
  {DISPLAY, "DISPLAY"},
  {END, "END"},
  {GRID, "GRID"},
  {LINE, "LINE"},
  {RECTANGLE, "RECTANGLE"}
};

enum Command command_from_string(const char *str) {
  int size = sizeof(COMMAND_STRING) / sizeof(COMMAND_STRING[0]);

  for (int i = 0; i < size; ++i)
    if (!strcmp(str, COMMAND_STRING[i].str))
      return COMMAND_STRING[i].command;

  return INVALID;
}

struct Operation {
  enum Command cmd;
  int args[MAX_ARGS];
};

struct Grid {
  char **state;
  char character;
  int width;
  int height;
  bool initialized;
};

void character(struct Grid *grid, int args[]) {
  grid->character = args[0];
}

void circle(struct Grid *grid, int args[]) {
  int x = args[0], y = args[1], radius = args[2];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  printf("x: %d, y: %d, radius: %d\n", x, y, radius);
}

void display(struct Grid grid) {
  int wrap = 10;

  if (!grid.initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  for (int i = 0; i < grid.height; ++i) {
    printf("%d ", ((9 - i) % wrap + wrap) % wrap);
    for (int j = 0; j < grid.width; ++j)
      printf("%c", grid.state[i][j]);
    printf("\n");
  }

  printf(" ");

  for (int i = 0; i < grid.width; ++i)
    printf("%d", ((9 - i) % wrap + wrap) % wrap);

  printf("\n");
}

void grid(struct Grid *grid, int args[]) {
  int width = args[0], height = args[1];

  if (grid->initialized) {
    printf("error: Grid has already been initialized\n");
    return;
  }

  grid->state = (char**)malloc(sizeof(char*)*width);

  for (int i = 0; i < width; ++i)
    grid->state[i] = (char*)malloc(sizeof(char)*height);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      grid->state[i][j] = ' ';

  grid->width = width;
  grid->height = height;
  grid->initialized = true;
}

void line(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  printf("x1: %d, y1: %d, x2: %d, y2: %d\n", x1, y1, x2, y2);
}

void rectangle(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

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
  char *inner, *outer;

  struct Operation operation;

  char *token = strtok_r(lexer.line, " ", &outer);

  operation.cmd = command_from_string(token);

  int index = 0;

  for (;;) {
    token = strtok_r(NULL, " ", &outer);

    if (!token) break;

    char *local = strtok_r(token, ",", &inner);

    while (local) {
      operation.args[index++] = atoi(local);
      local = strtok_r(NULL, ",", &inner);
    }
  }

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
    case DISPLAY:
      display(i->grid);
      break;
    case END:
      exit(0);
    case GRID:
      grid(&i->grid, i->op.args);
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
  grid.initialized = false;

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
