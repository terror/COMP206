#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

PROG='/home/2013/jdsilv2/206/mini2/primechk'
MIN=1
MAX=1000000000000000000

err() {
  echo -e "\e[31merror\e[0m: $1" && exit "$2"
}

out() {
  echo "$1" && exit "$2"
}

is_valid_number() {
  if [[ $line =~ ^[0-9]+$ && $line -ge $MIN && $line -le $MAX ]]; then
    return 0
  else
    return 1
  fi
}

FILE=
LARGEST=false

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
    *)
     out 'Usage: primechk.bash -f <numbersfile> [-l]' 1;;
  esac
done

if [[ -z $FILE ]]; then
  err 'Required option -f missing' 1
fi

if [[ ! -f $FILE ]]; then
  err 'Input file must exist' 1
fi

OLDIFS=$IFS
IFS=$'\n'
MAX_PRIME=-1

for line in $(cat "$FILE"); do
  if is_valid_number "$line" && $PROG "$line" &>/dev/null; then
    if [[ $LARGEST = true && "$line" -gt $MAX_PRIME ]]; then
      MAX_PRIME="$line"
    else
      echo "$line"
    fi
  fi
done

if [[ $LARGEST && $MAX_PRIME -ne -1 ]]; then echo "$MAX_PRIME"; fi
