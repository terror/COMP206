#!/bin/bash

COUNT=1
FILE='asciidraw.c'
OUT='asciidraw'

# 1 - The message to write.
#
# A small wrapper to produce an error message to standard
# output and then exit with code 1.

err() {
  echo "error: $1" && exit 1;
}

# 1 - The test name
# 2 - Commands to run
#
# A utility to run a single test case.

test() {
  echo "Test $COUNT: $1"
  echo "$2"
  ./asciidraw <<-EOL
  $2
	EOL
  ((COUNT += 1))
}

# The program entrypoint.

main() {
  if [[ ! -f $FILE ]]; then
    err "Cannot locate $FILE"
  fi

  if ! gcc -o $OUT asciidraw.c -lm; then
    err "Failed to compile $FILE"
  fi

  test "Only display the drawing area" "$(cat <<-EOL
		GRID 10 10
		DISPLAY
		END
		EOL
  )"

	test "Draw two lines" "$(cat <<-EOL
		GRID 10 10
		LINE 0,0 9,9
		LINE 9,0 0,9
		DISPLAY
		END
		EOL
  )"

	test "Draw a rectangle" "$(cat <<-EOL
		GRID 10 10
		RECTANGLE 0,0 9,9
		DISPLAY
		END
		EOL
  )"

	test "Draw a circle" "$(cat <<-EOL
		GRID 10 10
		CIRCLE 5,5,2
		DISPLAY
		END
		EOL
  )"

	test "Draw all shapes" "$(cat <<-EOL
		GRID 10 10
		CIRCLE 5,5,2
		LINE 0,5 0,9
		RECTANGLE 0,0 9,9
		DISPLAY
		END
		EOL
  )"

	test "Set a different character" "$(cat <<-EOL
		CHAR +
		GRID 10 10
		CHAR ~
		CIRCLE 5,5,2
		CHAR -
		LINE 0,5 0,9
		CHAR =
		RECTANGLE 0,0 9,9
		DISPLAY
		END
		EOL
  )"

	test "Handle command precedence" "$(cat <<-EOL
		CHAR +
		CIRCLE 5,5,2
		LINE 0,5 0,9
		RECTANGLE 0,0 9,9
		GRID 10 10
		CIRCLE 5,5,2
		LINE 0,5 0,9
		RECTANGLE 0,0 9,9
		DISPLAY
		END
		EOL
  )"

	test "Handle invalid commands" "$(cat <<-EOL
		FOO 1 2 3
		GRID 10 10
		FOO 1 2 3
		CIRCLE 4,4,2
		DISPLAY
		END
		EOL
  )"

	test "Handle larger grid" "$(cat <<-EOL
		GRID 40 40
		LINE 0,0 39,39
		LINE 0,39 39,0
		CIRCLE 20,20,19
		DISPLAY
		END
		EOL
  )"

	test "Cannot resize grid" "$(cat <<-EOL
		GRID 10 10
		GRID 20 20
		END
		EOL
  )"
}

main
