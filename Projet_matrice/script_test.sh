#!/bin/sh
# 0 true, success
# 1 false, error

# Ce script compile le code source passé en paramètre puis l'excécute avec un nombre de processus défini.
# Il compare le résultat obtenu avec le résultat qu'on devrait avoir.
# Si c'est juste il affiche TRUE, FALSE sinon.

CC=mpicc
RUN=mpirun

result=""

if [ $# -ne 3 ]; then
    echo "Usage : $0 programme.c exec numprocs"
    exit 1
fi
$CC -g -Wall -o $2 $1

for i in {1..11}; do
    $RUN --oversubscribe -np $3 ./$2 Test/test${i}A.txt Test/test${i}B.txt > tmp.txt

    diff Test/resultat${i}.txt tmp.txt
    if [ $? ]; then
        result+="True;"
    else result+="False;"
    fi
done

rm tmp.txt
echo $result
exit 0
