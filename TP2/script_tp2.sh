#!/bin/bash

gcc -Wall --std=c99 -o tp2_mp tp2.c -fopenmp

cat /dev/null > resultat.txt
for i in `seq 1 20`;
do
    export OMP_NUM_THREADS=$i
	time ./tp2_mp 15000000 >> resultat.txt
done

gnuplot << EOF
set terminal pdfcairo # ou pdf/jpeg/png...
set output "Graphe_tp2.pdf"
set title "Graphe représentant le temps d'exécution de la région // en fonction du nombre de threads"
plot "resultat.txt" using 1:2 with lines

exit 1
