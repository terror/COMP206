#!/bin/bash

COUNT=1
FILE='asciidraw.c'

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

  if ! gcc -o asciidraw asciidraw.c -lm; then
    err "Failed to compile $FILE"
  fi

	test "Draw horizontal lines" "$(cat <<-EOL
			GRID 10   10
      LINE 0,0  10,0
      LINE 0,1  10,1
      LINE 0,2  10,2
      LINE 0,3  10,3
      LINE 0,4  10,4
      LINE 0,5  10,5
      LINE 0,6  10,6
      LINE 0,7  10,7
      LINE 0,8  10,8
      LINE 0,9  10,9
      LINE 0,10 10,10
      DISPLAY
      END
		EOL
  )"
}

main
