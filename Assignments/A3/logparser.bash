#!/bin/bash

# Name:       Liam Scalzulli
# Department: Computer Science
# Email:      liam.scalzulli@mail.mcgill.ca

BROADCAST='broadcastMsg'

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
# A small wrapper for writing a message to stderr 
# with the prefix `error: ` and then exiting with 
# some specified exit code.

err() {
  echo "$1" >&2 && exit "$2"
}

# 1 - Input file path
# 2 - Pattern
# 
# A helper utility to filter lines from a file using
# grep.

lines() {
  grep -i "$2" "$1" | while read -r line; do
    echo "$line"
  done
}

# 1 - Input directory
# 2 - Sender process identifier
# 3 - Message identifier
# 4 - Broadcast timestamp
#
# Goes through each logfile in the input directory and builds an entry for
# each receiving process of the form:
#
#         <receiver>,<broadcasted>,<received>,<delivered>

entries() {
  for path in "$1"/*.log; do
    filename=$(basename "$path")
    echo -n "$(echo ${filename%.*} | sed 's/\./\:/'),$4,"
    echo -n "$(lines "$path" "Received a message from. message: \[senderProcess:$2:val:$3\]" | awk '{print $4}'),"
    echo $(lines "$path" "Received :$3 from : $2" | awk '{print $4}')
  done
}

# 1 - The input directory
# 
# Parses all log files in the input directory and
# writes content to stdout for creating `logdata.csv`.

log() {
  for path in "$1"/*.log; do
    grep -i $BROADCAST "$path" | while read -r line; do
      filename=$(basename "$path")
      message_id=$(echo "$line" | awk -F: '{print $NF}' | sed 's/ //')
      sender_process=$(echo ${filename%.*} | sed 's/\./\:/')
      broadcast_timestamp=$(echo "$line" | awk '{print $4}')
      for entry in $(entries "$1" "$sender_process" "$message_id" "$broadcast_timestamp"); do
        echo "$sender_process,$message_id,$entry"
      done
    done
  done
}

# 1 - The logdata.csv file
# 
# Parses the logdata.csv file and writes statistics
# to stdout based on that parsed information.

stats() {
  echo 'foo'
}

# 1 - The stats.csv file
# 
# Turns the stats.csv file into a valid html file by
# replacing the appropriate commas with html tags.

page() {
  echo 'foo'
}

# If the number of arguments passed to this program is not
# equal to 2, we output a simple usage message and exit with
# code 1.

if [[ $# -ne 1 ]]; then
  out 'Usage: ./logparser.bash <logdir>' 1
fi

# If the input directory isn't a valid directory,
# we write an error message to stderr and exit with
# code 2.

if [[ ! -d $1 ]]; then
  err "$1 is not a valid directory name" 2
fi

# 1 - The input directory
# 
# The program entrypoint.

main() {
  log   "$1"        > logdata.csv
  stats logdata.csv > stats.csv
  page  stats.csv   > stats.html
}

main "$1"
