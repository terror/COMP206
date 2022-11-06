#include <math.h>
#include <stdio.h>
#include <string.h>

const int ARGS_MAX = 4;
const int LINE_MAX = 256;
const int SIZE_MAX = 1000;

int main() {
  char character = '*', grid[SIZE_MAX][SIZE_MAX];
  int width = 0, height = 0, initialized = 0;

  for (;;) {
    char line[LINE_MAX];

    // Read in a line from standard input
    fgets(line, LINE_MAX, stdin);
    line[strcspn(line, "\n")] = 0;

    // Grab the first token out
    char *inner, *outer;
    char *token = strtok_r(line, " ", &outer);

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ CHAR                                                                        │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "CHAR")) {
      token = strtok_r(NULL, " ", &outer);
      character = *token;
      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ CIRCLE                                                                      │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "CIRCLE")) {
      int index = 0, args[ARGS_MAX];

      if (!initialized) {
        printf("error: Only the CHAR command is allowed before the grid size is set\n");
        continue;
      }

      for (;;) {
        token = strtok_r(NULL, " ", &outer);

        if (!token) break;

        char *local = strtok_r(token, ",", &inner);

        while (local) {
          int value = 0;

          for (int i = 0; i < strlen(local); ++i)
            value = value * 10 + local[i] - '0';

          args[index++] = value;

          local = strtok_r(NULL, ",", &inner);
        }
      }

      int x = args[0], y = args[1], radius = args[2];
      printf("x: %d, y: %d, radius: %d\n", x, y, radius);

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ DISPLAY                                                                     │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "DISPLAY")) {
      int wrap = 10;

      if (!initialized) {
        printf("error: Only the CHAR command is allowed before the grid size is set\n");
        continue;
      }

      for (int i = 0; i < height; ++i) {
        printf("%d ", ((9 - i) % wrap + wrap) % wrap);
        for (int j = 0; j < width; ++j)
          printf("%c", grid[i][j]);
        printf("\n");
      }

      printf(" ");

      for (int i = 0; i < width; ++i)
        printf("%d", ((9 - i) % wrap + wrap) % wrap);

      printf("\n");

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ END                                                                         │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "END"))
      return 0;

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ GRID                                                                        │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "GRID")) {
      int index = 0, args[ARGS_MAX];

      if (initialized) {
        printf("error: Grid is already initialized\n");
        continue;
      }

      for (;;) {
        token = strtok_r(NULL, " ", &outer);

        if (!token) break;

        int value = 0;

        for (int i = 0; i < strlen(token); ++i)
          value = value * 10 + token[i] - '0';

        args[index++] = value;
      }

      width = args[0], height = args[1];

      for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
          grid[i][j] = ' ';

      initialized = 1;

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ LINE                                                                        │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "LINE")) {
      int index = 0, args[ARGS_MAX];

      if (!initialized) {
        printf("error: Only the CHAR command is allowed before the grid size is set\n");
        continue;
      }

      for (;;) {
        token = strtok_r(NULL, " ", &outer);

        if (!token) break;

        char *local = strtok_r(token, ",", &inner);

        while (local) {
          int value = 0;

          for (int i = 0; i < strlen(local); ++i)
            value = value * 10 + local[i] - '0';

          args[index++] = value;

          local = strtok_r(NULL, ",", &inner);
        }
      }

      int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];
      printf("x1: %d, y1: %d, x2: %d, y2: %d\n", x1, y1, x2, y2);

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ RECTANGLE                                                                   │
   ╚────────────────────────────────────────────────────────────────────────────*/

    if (!strcmp(token, "RECTANGLE")) {
      int index = 0, args[ARGS_MAX];

      if (!initialized) {
        printf("error: Only the CHAR command is allowed before the grid size is set\n");
        continue;
      }

      for (;;) {
        token = strtok_r(NULL, " ", &outer);

        if (!token) break;

        char *local = strtok_r(token, ",", &inner);

        while (local) {
          int value = 0;

          for (int i = 0; i < strlen(local); ++i)
            value = value * 10 + local[i] - '0';

          args[index++] = value;

          local = strtok_r(NULL, ",", &inner);
        }
      }

      int x1 = args[0], y1 = args[1], x2 = args[2], y2 = args[3];
      printf("x1: %d, y1: %d, x2: %d, y2: %d\n", x1, y1, x2, y2);

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ INVALID                                                                     │
   ╚────────────────────────────────────────────────────────────────────────────*/

    printf("error: Invalid command `%s`\n", token);
  }
}
