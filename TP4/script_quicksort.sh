#!/bin/bash
cat /dev/null > resultat.txt
gcc -g -Wall -o quicksort quicksort.c -lm

for((i=0 ; $i < 1000000 ; i=i+5000))
do
	./quicksort 4 $i >> resultat.txt
done
gnuplot -persist <<-EOFMarker
	set xlabel "taille du tableau"
	set ylabel "temps d'execution en secondes"
	plot "resultat.txt" using 1:2 with lines
EOFMarker

#Complexité Quicksort : O(log n)
#pire cas : O(n²)

