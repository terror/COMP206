#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_ARGS = 4;

/*
 * All available commands the interpreter
 * can evaluate.
 */
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
  { CHAR,      "CHAR"      },
  { CIRCLE,    "CIRCLE"    },
  { DISPLAY,   "DISPLAY"   },
  { END,       "END"       },
  { GRID,      "GRID"      },
  { LINE,      "LINE"      },
  { RECTANGLE, "RECTANGLE" }
};

/*
 * Find a corresponding `Command` enum from a specified string.
 *
 * @param str The string to associate with a `Command` enum.
 * @return The correspoding `Command` enum.
 */
enum Command command_from_string(const char *str) {
  int size = sizeof(COMMAND_STRING) / sizeof(COMMAND_STRING[0]);

  for (int i = 0; i < size; ++i)
    if (!strcmp(str, COMMAND_STRING[i].str))
      return COMMAND_STRING[i].command;

  return INVALID;
}

/*
 * Represents an `operation`, which is essentially a command
 * that contains its arguments and its original name.
 */
struct Operation {
  char* name;
  enum Command cmd;
  int args[MAX_ARGS];
};

/*
 * The canvas the user can draw on, holds all
 * relevant state on itself.
 */
struct Grid {
  char **state;
  char character;
  int width;
  int height;
  bool initialized;
};

/*
 * Bresenham's line drawing algorithm.
 *
 * @param grid A pointer to a grid.
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param dx The difference in x values.
 * @param dy The difference in y values.
 */
void bresenham(struct Grid *grid, int x1, int y1, int x2, int y2, int dx, int dy) {
  int pk = 2 * dy - dx;

  for (int i = 0; i <= dx; ++i) {
    // Ensure we're within bounds of the grid
    if (x1 >= 0 && x1 < grid->width && y1 >= 0 && y1 < grid->height)
      grid->state[x1][y1] = grid->character;

    x1 < x2 ? ++x1 : --x1;

    if (pk < 0)
      pk = pk + 2 * dy;
    else {
      y1 < y2 ? ++y1 : --y1;
      pk = pk + 2 * dy - 2 * dx;
    }
  }
}

/*
 * A small wrapper around Bresenham's line drawing algorithm.
 *
 * @param grid A pointer to a grid.
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void draw_line(struct Grid*grid, int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1), dy = abs(y2 - y1);

  dx > dy ?
    bresenham(grid, x1, y1, x2, y2, dx, dy) :
    bresenham(grid, y1, x1, y2, x2, dy, dx);
}

/*
 * Handler for the `CHAR` operation.
 *
 * @param grid A pointer to a grid.
 * @param args [character, ..].
 */
void character(struct Grid *grid, int args[]) {
  grid->character = args[0];
}

/*
 * Handler for the `CIRCLE` operation.
 *
 * @param grid A pointer to a grid
 * @param args [x, y, radius, ..].
 */
void circle(struct Grid *grid, int args[]) {
  int x = args[0], y = args[1], radius = args[2];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  printf("x: %d, y: %d, radius: %d\n", x, y, radius);
}

/*
 * Handler for the `DISPLAY` operation.
 *
 * @param grid A grid struct.
 */
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

/*
 * Handler for the `GRID` operation.
 *
 * @param grid A pointer to a grid.
 * @param args [width, height, ..].
 */
void grid(struct Grid *grid, int args[]) {
  int width = args[0], height = args[1];

  if (grid->initialized) {
    printf("error: Grid has already been initialized\n");
    return;
  }

  grid->state = (char**)malloc(sizeof(char*)*width);

  for (int i = 0; i < height; ++i)
    grid->state[i] = (char*)malloc(sizeof(char)*height);

  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      grid->state[i][j] = ' ';

  grid->width = width;
  grid->height = height;
  grid->initialized = true;
}

/*
 * Handler for the `LINE` operation.
 *
 * @param grid A pointer to a grid.
 * @param args [x1, y1, x2, y2].
 */
void line(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  draw_line(grid, x1, y1, x2, y2);
}

/*
 * Handler for the `RECTANGLE` operation.
 *
 * @param grid A pointer to a grid.
 * @param args [x1, y1, x2, y2].
 */
void rectangle(struct Grid *grid, int args[]) {
  int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];

  if (!grid->initialized) {
    printf("error: Grid isn't initialized\n");
    return;
  }

  draw_line(grid, x1, y1, x1 + abs(x2 - x1), y1);
  draw_line(grid, x1 + abs(x2 - x1), y1, x2, y2);
  draw_line(grid, x2, y2, x1, y1 + abs(y2 - y1));
  draw_line(grid, x1, y1 + abs(y2 - y1), x1, y1);
}

/*
 * The line parser responsible for turning lines read
 * from standard input into valid `Operation` structs.
 */
struct Parser {
  char line[LINE_MAX];
};

/*
 * Read a line in from standard input and set it on the
 * passed in parser.
 *
 * @param parser A pointer to a parser.
 */
void read(struct Parser *parser) {
  fgets(parser->line, LINE_MAX, stdin);
  parser->line[strcspn(parser->line, "\n")] = 0;
}

/*
 * Parse the current string that's set on the passed
 * in parser.
 *
 * It first splits on spaces `' '` and then handles each
 * individual token by splitting on commas `,`.
 *
 * @param parser A parser struct.
 * @return The parsed operation.
 */
struct Operation parse(struct Parser parser) {
  char *inner, *outer;

  struct Operation operation;

  char *token = strtok_r(parser.line, " ", &outer);

  operation.name = strdup(token);
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

/*
 * The struct responsible for evaluating operations
 * parsed by the parser.
 */
struct Interpreter {
  struct Grid grid;
  struct Operation op;
};

/*
 * Load an operation onto the passed in interpreter.
 *
 * @param interpreter A pointer to an interpreter.
 * @param op An operation struct.
 */
void load(struct Interpreter *interpreter, struct Operation op) {
  interpreter->op = op;
}

/*
 * Evaluate the operation that's present on the passed in
 * interpreter.
 *
 * This method essentially associates commands with their
 * corresponding methods on `Grid`.
 *
 * @param i A pointer to an interpreter.
 */
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
      printf("error: Invalid command `%s`\n", i->op.name);
      break;
    case LINE:
      line(&i->grid, i->op.args);
      break;
    case RECTANGLE:
      rectangle(&i->grid, i->op.args);
      break;
  }
}

/*
 * The program entrypoint.
 */
int main() {
  struct Grid grid;

  grid.character = '*';
  grid.initialized = false;

  struct Parser parser;
  struct Interpreter interpreter;

  interpreter.grid = grid;

  for (;;) {
    // Display the prompt
    printf("> ");

    // Read a line in from stdin
    read(&parser);

    // Load the operation onto the interpreter
    load(&interpreter, parse(parser));

    // Evaluate the currently loaded operation
    eval(&interpreter);

  }
}
