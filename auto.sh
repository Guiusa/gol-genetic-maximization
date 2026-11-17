#! /bin/bash
# ./auto.sh ROWS COLUMNS indNum genNum rounds

[[ ! -f geneticlife ]] && \
  make

ROWS=${1:-100}
COLUMNS=${2:-100}
indNum=${3:-20}
genNum=${4:-30}
rounds=${5:-200}

echo "$ROWS $COLUMNS $indNum $genNum $rounds" | ./geneticlife
