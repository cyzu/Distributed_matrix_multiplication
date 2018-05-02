#!/bin/sh

# Ce script compile le code source dans le dossier scr/ puis l'excécute avec n processus passé en paramètre.
# Il compare les résultats obtenus avec ceux du dossier Resultat/ puis affiche True ou False

#
# RAJOUTEZ LES OPTIONS DE COMPILATION SI BESOIN ! ↓↓↓
#

CC="mpicc"
OPTION="-std=c99 -g -Wall"
OMP=""          # pour openmp (-fopenmp)
MATH=""                 # -lm si vous utilisez math.h
RUN="mpirun"

# NE PAS Y TOUCHER
file=src/*
result=""
exec=executable

if [ $# -ne 1 ]; then
    echo "Usage : $0 numprocs"
    exit 1
fi

echo $CC $OPTION $OMP -o $exec $file $MATH
$CC $OPTION $OMP -o $exec $file $MATH

for i in {1..11}; do
    echo $RUN --oversubscribe -np $1 ./$exec Test/test${i}A.txt Test/test${i}B.txt
    $RUN --oversubscribe -np $1 ./$exec Test/test${i}A.txt Test/test${i}B.txt > tmp.txt

    diff Resultat/resultat${i}.txt tmp.txt
    if [ $? = 0 ]; then
        result+="True;"
    else result+="False;"
    fi
done

rm tmp.txt $exec
echo ""
echo $result
exit 0
