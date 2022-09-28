default:
  just --list

lint:
  shellcheck **/*.bash
