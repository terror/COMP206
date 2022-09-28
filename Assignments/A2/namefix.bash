#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

PROG='/home/2013/jdsilv2/206/mini2/namefix'

err() {
  echo -e "\e[31merror\e[0m: $1" && exit "$2"
}

out() {
  echo "$1" && exit "$2"
}

run() {
  if [[ ! -f $2 ]]; then touch "$2"; fi 
  $PROG "$1" "$2"
}

if [[ $# -ne 2 ]]; then
  out 'Usage: namefix.bash <inputfile> <outputfile>' 1
fi

if [[ ! -f $1 || ! -r $1 ]]; then
  err 'Input file must exist, be a file, and be readable' 3
fi

if [[ $1 -ef $2 ]]; then 
  err 'Input and output files must be different' 2
fi 

if [[ -f $2 || -d $2 && ! -w $2 ]]; then
  err 'Output file or directory must be writeable' 4
fi

if [[ -d $2 && $2 -ef $PWD ]]; then
  err 'Output directory must differ from the current working directory' 1
fi

if [[ -d $2 ]]; then
  run "$1" "$2/$(basename "$1")"
else
  run "$1" "$2"
fi
