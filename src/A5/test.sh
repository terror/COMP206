#!/usr/bin/env bash

# TODO:
# - [x] Preserve insert order
# - [x] Delete nodes
# - [x] Don't insert duplicate items
# - [x] Write to db file
# - [x] Fix output
# - [x] Update non-empty fields on insert
# - [ ] Create file if not exists
# - [ ] Error on non-writeable file
# - [ ] See how to display empty fields?

COUNT=1
FILE='pimapp.c'
OUT='pimapp'
DB='db.txt'

err() {
  echo "error: $1" && exit 1;
}

test() {
  echo "============================================"
  echo "Test $COUNT: $1"
  echo "============================================"
  echo "$2"
  ./$OUT $DB <<-EOL
	$2
	EOL
  ((COUNT += 1))
}

if [[ ! -f $FILE ]]; then
  err "Cannot locate $FILE"
fi

if ! gcc -o $OUT $OUT.c; then
  err "Failed to compile $FILE"
fi

test "Preserves ascending order" "$(cat <<-EOL
	I,3,S,Ali,Faculty of Science,2022
	I,2,S,Ali,Faculty of Science,2022
	I,1,S,Ali,Faculty of Science,2022
	LIST
	END
	EOL
)"

test "Delete nodes" "$(cat <<-EOL
	I,3,S,Ali,Faculty of Science,2022
	I,2,S,Ali,Faculty of Science,2022
	I,1,S,Ali,Faculty of Science,2022
	D,1
	D,2
	LIST
	END
	EOL
)"

test "Don't insert duplicate items" "$(cat <<-EOL
	I,3,S,Ali,Faculty of Science,2022
	I,3,S,Ali,Faculty of Science,2022
	LIST
	END
	EOL
)"

test "Insert professors and students" "$(cat <<-EOL
	I,5,S,John Doe,Science,2022
	I,3,S,Sheila,Software Eng,2021
	I,7,P,Jane Doe,Computer Science,2020,N
	LIST
	END
	EOL
)"

test "Delete non-existent ID field" "$(cat <<-EOL
	D,1
	LIST
	END
	EOL
)"

test "Update non-empty fields on insert" "$(cat <<-EOL
	I,5,S,John Doe,Science,2022
	I,7,P,Jane Doe,Computer Science,2020,N
	I,5,S,Jane Doe,Arts,2024
	I,7,P,John Doe,Arts,2024,Y
	LIST
	END
	EOL
)"

test "Don't update empty fields" "$(cat <<-EOL
	I,5,S,,Science,2022
	I,5,S,Jane Doe,Science,2022
	I,7,P,,,2020,N
	I,7,P,Jane Doe,Computer Science,2020,N
	LIST
	END
	EOL
)"
