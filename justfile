default:
  just --list

all: forbid

forbid:
  ./bin/forbid

lint:
  shellcheck **/*.bash
