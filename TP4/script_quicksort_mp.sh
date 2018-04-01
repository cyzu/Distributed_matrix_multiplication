#!/bin/bash
cat /dev/null > resultat_mp.txt
gcc -g -Wall --std=c99 -o quicksort_mp quicksort.c -fopenmp -lm

for((i=0 ; $i < 1000000 ; i=i+5000))
do
	export OMP_NUM_THREADS=4
	./quicksort_mp 4 $i >> resultat_mp.txt
done
gnuplot -persist <<-EOFMarker
	set title "Graphe parallelise"
	set xlabel "taille du tableau"
	set ylabel "temps d'execution en secondes"
	plot "resultat_mp.txt" using 1:2 with lines
EOFMarker

