#!/bin/sh
# 0 true, success
# 1 false, error

# Ce script compile le code source dans le dossier scr/ puis l'excécute avec n processus passé en paramètre.
# Il compare les résultats obtenus avec ceux du dossier Resultat/ puis affiche True ou False

#
# RAJOUTEZ LES OPTIONS DE COMPILATION SI BESOIN ! ↓↓↓
#

CC="mpicc"
OPTION="-std=c99 -g -Wall"
OMP="-fopenmp"          # pour openmp (-fopenmp)
MATH=""                 # -lm si vous utilisez math.h
RUN="mpirun"

# NE PAS Y TOUCHER
file=src/*
result=""
exec=""

if [ $# -ne 1 ]; then
    echo "Usage : $0 numprocs"
    exit 1
fi
exec=${1%.*}
$CC $OPTION $OMP -o $exec $file $MATH

for i in {1..11}; do
    $RUN --oversubscribe -np $1 ./$exec Test/test${i}A.txt Test/test${i}B.txt > tmp.txt

    diff Resultat/resultat${i}.txt tmp.txt
    if [ $? ]; then
        result+="True;"
    else result+="False;"
    fi
done

rm tmp.txt
echo $result
exit 0
