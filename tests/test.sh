# Fichier : test.sh
# Date : 2026-05-31
#
# Ce script lance deux processus pour les tests du module :
# - un processus envoie les données au module (MIDI IN)
# - un processus lit les données renvoyées par le module (MIDI OUT)
# Les données lues sont comparées aux données attendues. Le test passe
# si elles sont égales. La lecture débute à la rencontre d'un flag de type
# FC FA et à BPM lent pour traiter convenablement les signaux 
# d'horloge F8.
#
# Il est conseillé de redémarrer le module avant chaque test sauf à 
# remettre tous les canaux de sorties sur OFF au préalable.

#!/bin/bash

if [ $# == 0 ]
then
    echo "usage : $0 nom_du_module"
    exit 1
fi

python3 receive.py $1 & python3 $1.py
