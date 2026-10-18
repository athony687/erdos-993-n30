#!/bin/bash
# Build the gentreeg-linked checker and run orders given as arguments.
# Usage: ./run.sh 26 27 28 29     (each order split into 16 parts, run in parallel)
set -e
cd "$(dirname "$0")"
ND=nauty2_8_8
if [ ! -f $ND/nauty.h ]; then (cd $ND && ./configure >/dev/null); fi
if [ ! -x check ]; then
  gcc -O3 -march=native -o check -DOUTPROC=check_tree -DSUMMARY=report -include proto.h -I$ND \
    $ND/gentreeg.c $ND/gtools.c $ND/nauty.c $ND/nautil.c $ND/naugraph.c \
    $ND/schreier.c $ND/naurng.c unimodal.c
fi
P=16
for n in "$@"; do
  mkdir -p out/n$n
  seq 0 $((P-1)) | xargs -P $P -I{} sh -c "./check -q $n {}/$P 2> out/n$n/part{}.log"
  sum() { grep -h '^RESULT' out/n$n/part*.log | sed "s/.* $1=\([0-9]*\).*/\1/" | awk '{s+=$1} END {printf "%.0f", s}'; }
  trees=$(sum trees); nonuni=$(sum nonunimodal); nonlc=$(sum nonlogconcave); bad=$(sum badorder)
  parts=$(grep -c '^RESULT' out/n$n/part*.log | awk -F: '{s+=$2} END {print s}')
  grep -h '^NONLC' out/n$n/part*.log > out/nonlc$n.txt || true
  echo "SUMMARY n=$n parts=$parts trees=$trees nonunimodal=$nonuni nonlogconcave=$nonlc badorder=$bad"
done
