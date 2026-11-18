#! /bin/bash
# ./auto.sh ROWS COLUMNS indNum genNum rounds

[[ ! -f geneticlife ]] && \
  make

ROWS=${1:-100}
COLUMNS=${2:-100}
indNum=${3:-10}
genNum=${4:-50}
rounds=${5:-300}

echo "$ROWS $COLUMNS $indNum $genNum $rounds" | ./geneticlife
