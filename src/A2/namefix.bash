#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

PROG='/home/2013/jdsilv2/206/mini2/namefix'

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
  out "error: $1" "$2"
}

# 1 - Input file
# 2 - Output file
# 
# Runs the program stored in $PROG and outputs
# the contents of the second argument (output file).

run() {
  $PROG "$1" "$2" && cat "$2"
}

# If the number of arguments passed to this program is not equal
# to 2, we output a simple usage message and exit with code 1.

if [[ $# -ne 2 ]]; then
  out 'Usage: namefix.bash <inputfile> <outputfile>' 1
fi

# If the input file does not exist, we output an error
# message and exit with code 3.

if [[ ! -f $1 ]]; then
  err "Input file $1 does not exist" 3
fi

# If the input file is not readable, we output an error
# message and exit with code 3.

if [[ ! -r $1 ]]; then
  err "Do not have read permissions on the input file $1" 3
fi

# If the input and output files are the same, we output
# an error message and exit with code 2.
#
# We handle two cases here:
# 
# 1 - The output is a file
# 
# We check if the file is the same as the input file.

if [[ $1 -ef $2 ]]; then
  err "Input file $1 is the same as the output file $2" 2
fi

# 2 - The output is a directory
# 
# We check if the output directory is the same as the 
# current working directory.

if [[ $2 -ef $PWD ]]; then 
  err "Input file $1 is the same as the output file $2/$(basename "$1")" 2
fi 

# If the output file or directory is not writeable, we
# output an error message and exit with code 4.
# 
# We handle a few cases here:
# 
# 1 - The output is a directory and is not writeable.

if [[ -d $2 && ! -w $2 ]]; then
  err "Do not have write permissions on output directory $2" 4
fi

# 2 - The output is an existing file and is not writeable.

if [[ -f $2 && ! -w $2 ]]; then
  err "Do not have write permissions on output file $2" 4
fi

# 3 - The output is a non-existent file and the directory which it 
#     should be created in is not writeable.

if [[ ! -f $2 && ! -w $(dirname "$2") ]]; then
  err "Do not have write permissions on output directory $(dirname "$2")" 4
fi

# If the output is a directory and the file we're supposed to write to
# is a directory or an existing file without write permissions, we output
# an error message and exit with code 4.
# 
# We handle two cases:

if [[ -d $2 ]]; then
  OUTPUT="$2/$(basename "$1")"
  
  # 1 - The output file we're supposed to write to is a directory.

  if [[ -d $OUTPUT ]]; then
    err "The output file we're supposed to write to $OUTPUT is a directory" 4
  fi

  # 2 - The output file we're supposed to write to is an existing file 
  #     without write permissions.

  if [[ -f $OUTPUT && ! -w $OUTPUT ]]; then
    err "Do not have write permissions on the output we're supposed to write to $OUTPUT" 4
  fi
fi

# Now we're ready to run the program. 
#
# If the output is a directory, we use the input file name in the 
# output directory, otherwise we use the output file as provided.

if [[ -d $2 ]]; then
  run "$1" "$OUTPUT"
else
  run "$1" "$2"
fi
