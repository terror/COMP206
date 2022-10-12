#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

FILE=
IFS=$'\n'
LARGEST=false
MAX_PRIME=-1
PROG='/home/2013/jdsilv2/206/mini2/primechk'

# 1 - Message to write
# 2 - Exit code
#
# A small wrapper for writing a message to stdout
# and then exiting with an exit code.

out() {
  echo "$1" && exit "$2"
}

# 1 - Message to write
# 2 - Exit code
#
# A small wrapper for writing a message to stdout
# with the prefix `error: ` and then exiting with
# some specified exit code.

err() {
  echo "error: $1" && exit "$2"
}

# A helper function that outputs the usage of this program
# while exiting afterward with code 1.

usage() {
  out 'Usage: ./primechk.bash -f <numbersfile> [-l]' 1
}

# If there aren't any arguments passed to this program, we output
# a usage message and exit with code 1.

if [[ $# -eq 0 ]]; then
  usage
fi

# Here we parse the command-line options for this program. 
# 
# If the `-f` option is set we set it to $FILE and if the `-l` 
# flag is set we turn on $LARGEST. 
# 
# Anything else passed in results in the output of a usage message 
# with an exit status of 1.

while [[ $# -gt 0 ]]; do
  case $1 in
    -f) 
      if [[ -n $FILE ]]; then
        usage
      else
        FILE="$2"; shift; shift
      fi;;
    -l) 
      if [[ $LARGEST = true ]]; then
        usage
      else
        LARGEST=true; shift
      fi;;
     *) 
      usage;;
  esac
done

# If the `-f` command-line option was not passed,
# we output an error message and exit with code 1.

if [[ -z $FILE ]]; then
  err 'Required option -f missing' 1
fi

# If the file passed into `-f` does not exist,
# we output an error message and exit with code 2.

if [[ ! -f $FILE ]]; then
  err "Input file $FILE does not exist" 2
fi

# Filter and go through the filtered contents of the input file,
# checking whether or not each filtered line is a prime.

for line in $(grep -E '^([1-9]|[1-9][0-9]{1,17})$' "$FILE"); do
  if $PROG "$line" &>/dev/null ; then
    if [[ $LARGEST = true && "$line" -gt $MAX_PRIME ]]; then
      MAX_PRIME="$line"
    else
      echo "$line"
    fi
  fi
done

# If the `-l` flag was passed, we output the largest prime 
# encountered, otherwise if there weren't any, we output an
# error message and exit with code 3.

if [[ $LARGEST = true ]]; then
  if [[ $MAX_PRIME -ne -1 ]]; then
    echo "$MAX_PRIME"
  else
    err "No prime numbers were found in the file $FILE" 3
  fi
fi
