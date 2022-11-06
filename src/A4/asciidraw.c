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

      int xc = args[0], yc = args[1], radius = args[2];

      int x = 0, y = radius, d = 3 - 2 * radius;

      if (xc + x >= 0 && xc + x < width && yc + y >= 0 && yc + y < height) grid[xc + x][yc + y] = character;
      if (xc - x >= 0 && xc - x < width && yc + y >= 0 && yc + y < height) grid[xc - x][yc + y] = character;
      if (xc + x >= 0 && xc + x < width && yc - y >= 0 && yc - y < height) grid[xc + x][yc - y] = character;
      if (xc - x >= 0 && xc - x < width && yc - y >= 0 && yc - y < height) grid[xc - x][yc - y] = character;
      if (xc + y >= 0 && xc + y < width && yc + x >= 0 && yc + x < height) grid[xc + y][yc + x] = character;
      if (xc - y >= 0 && xc - y < width && yc + x >= 0 && yc + x < height) grid[xc - y][yc + x] = character;
      if (xc + y >= 0 && xc + y < width && yc - x >= 0 && yc - x < height) grid[xc + y][yc - x] = character;
      if (xc - y >= 0 && xc - y < width && yc - x >= 0 && yc - x < height) grid[xc - y][yc - x] = character;

      while (y >= x) {
        ++x;

        if (d > 0) {
          --y;
          d = d + 4 * (x - y) + 10;
        } else {
          d = d + 4 * x + 6;
        }

        if (xc + x >= 0 && xc + x < width && yc + y >= 0 && yc + y < height) grid[xc + x][yc + y] = character;
        if (xc - x >= 0 && xc - x < width && yc + y >= 0 && yc + y < height) grid[xc - x][yc + y] = character;
        if (xc + x >= 0 && xc + x < width && yc - y >= 0 && yc - y < height) grid[xc + x][yc - y] = character;
        if (xc - x >= 0 && xc - x < width && yc - y >= 0 && yc - y < height) grid[xc - x][yc - y] = character;
        if (xc + y >= 0 && xc + y < width && yc + x >= 0 && yc + x < height) grid[xc + y][yc + x] = character;
        if (xc - y >= 0 && xc - y < width && yc + x >= 0 && yc + x < height) grid[xc - y][yc + x] = character;
        if (xc + y >= 0 && xc + y < width && yc - x >= 0 && yc - x < height) grid[xc + y][yc - x] = character;
        if (xc - y >= 0 && xc - y < width && yc - x >= 0 && yc - x < height) grid[xc - y][yc - x] = character;
      }

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

      for (int i = 0; i < width; ++i) {
        printf("%d ", ((9 - i) % wrap + wrap) % wrap);
        for (int j = 0; j < height; ++j)
          printf("%c", grid[j][i]);
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

      int dx =  fabs((double)(x2 - x1)), sx = x1 < x2 ? 1 : -1;
      int dy = -fabs((double)(y2 - y1)), sy = y1 < y2 ? 1 : -1;

      int a = dx + dy, b = 0;

      for (;;) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
          grid[x1][y1] = character;
        if (x1 == x2 && y1 == y2) break;
        b = 2 * a;
        if (b >= dy) { a += dy; x1 += sx; }
        if (b <= dx) { a += dx; y1 += sy; }
      }

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

      // Draw line from (x1, y1) -> (x1 + (x2 - x1), y1)
      {
        int x1 = args[0], y1 = args[1];
        int x2 = args[0] + fabs((double)(args[2] - args[0])), y2 = args[1];

        int dx =  fabs((double)(x2 - x1)), sx = x1 < x2 ? 1 : -1;
        int dy = -fabs((double)(y2 - y1)), sy = y1 < y2 ? 1 : -1;

        int a = dx + dy, b = 0;

        for (;;) {
          if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
            grid[x1][y1] = character;
          if (x1 == x2 && y1 == y2) break;
          b = 2 * a;
          if (b >= dy) { a += dy; x1 += sx; }
          if (b <= dx) { a += dx; y1 += sy; }
        }
      }

      // Draw line from (x1 + (x2 - x1), y1) -> (x2, y2)
      {
        int x1 = args[0] + fabs((double)(args[2] - args[0])), y1 = args[1];
        int x2 = args[2], y2 = args[3];

        int dx =  fabs((double)(x2 - x1)), sx = x1 < x2 ? 1 : -1;
        int dy = -fabs((double)(y2 - y1)), sy = y1 < y2 ? 1 : -1;

        int a = dx + dy, b = 0;

        for (;;) {
          if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
            grid[x1][y1] = character;
          if (x1 == x2 && y1 == y2) break;
          b = 2 * a;
          if (b >= dy) { a += dy; x1 += sx; }
          if (b <= dx) { a += dx; y1 += sy; }
        }
      }

      // Draw (x2, y2) -> (x1, y1 + (y2 - y1))
      {
        int x1 = args[2], y1 = args[3];
        int x2 = args[0], y2 = args[1] + fabs((double)(args[3] - args[1]));

        int dx =  fabs((double)(x2 - x1)), sx = x1 < x2 ? 1 : -1;
        int dy = -fabs((double)(y2 - y1)), sy = y1 < y2 ? 1 : -1;

        int a = dx + dy, b = 0;

        for (;;) {
          if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
            grid[x1][y1] = character;
          if (x1 == x2 && y1 == y2) break;
          b = 2 * a;
          if (b >= dy) { a += dy; x1 += sx; }
          if (b <= dx) { a += dx; y1 += sy; }
        }
      }

      // Draw (x1, y1 + (y2 - y1)) -> (x1, y1)
      {
        int x1 = args[0], y1 = args[1] + fabs((double)(args[3] - args[1]));
        int x2 = args[0], y2 = args[1];

        int dx =  fabs((double)(x2 - x1)), sx = x1 < x2 ? 1 : -1;
        int dy = -fabs((double)(y2 - y1)), sy = y1 < y2 ? 1 : -1;

        int a = dx + dy, b = 0;

        for (;;) {
          if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
            grid[x1][y1] = character;
          if (x1 == x2 && y1 == y2) break;
          b = 2 * a;
          if (b >= dy) { a += dy; x1 += sx; }
          if (b <= dx) { a += dx; y1 += sy; }
        }
      }

      continue;
    }

  /*─────────────────────────────────────────────────────────────────────────────╗
   │ INVALID                                                                     │
   ╚────────────────────────────────────────────────────────────────────────────*/

    printf("error: Invalid command `%s`\n", token);
  }
}
