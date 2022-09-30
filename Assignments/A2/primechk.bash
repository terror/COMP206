#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

FILE=
IFS=$'\n'
LARGEST=false
MAX_PRIME=-1
PROG='/home/2013/jdsilv2/206/mini2/primechk'

err() {
  echo "error: $1" && exit "$2"
}

out() {
  echo "$1" && exit "$2"
}

while [[ $# -gt 0 ]]; do
  case $1 in
    -f)
      FILE="$2"
      shift
      shift
      ;;
    -l)
      LARGEST=true
      shift
      ;;
    *) out 'Usage: primechk.bash -f <numbersfile> [-l]' 1;;
  esac
done

if [[ -z $FILE ]]; then
  err 'Required option -f missing' 1
fi

if [[ ! -f $FILE ]]; then
  err 'Input file must exist' 2
fi

for line in $(grep -E '^[1-9][0-9]{0,16}' "$FILE"); do
  if $PROG "$line" &>/dev/null ; then
    if [[ $LARGEST = true && "$line" -gt $MAX_PRIME ]]; then
      MAX_PRIME="$line"
    else
      echo "$line"
    fi
  fi
done

if [[ $LARGEST = true ]]; then
  if [[ $MAX_PRIME -ne -1 ]]; then
    echo "$MAX_PRIME"
  else
    err 'Did not find any prime numbers' 3
  fi
fi
