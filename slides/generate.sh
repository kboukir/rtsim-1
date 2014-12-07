#!/bin/sh

../simEDF s9.system 0 --scheduler RM --steps 160 --svg s9.svg
../simEDF s16.system 0 --scheduler fixed --steps 220 --svg s16.svg
../simEDF s18.system 0 --scheduler EDF --steps 220 --svg s18.svg
../simEDF s25.system 0 --scheduler fixed --steps 70 --svg s25.svg
../simEDF s43.system 0 --scheduler RM --steps 100 --svg s43.svg
../simEDF s53.system 0 --scheduler fixed --steps 2700 --svg s53.svg # Il faut renommer T2 en T3 et vice-versa. La tâche d'offset 40 a un WCET de 10 comme montré dans la figure, alors que le texte en slide 52 dit que c'est la tâche d'offset 0 (T3) qui a un WCET de 10.
../simEDF s54.system 0 --scheduler fixed --steps 360 --svg s54.svg
../simEDF s55.system 0 --scheduler DM --steps 120 --svg s55.svg
../simEDF s61.system 0 --scheduler EDF --steps 220 --svg s61.svg
../simEDF s69.system 0 --scheduler EDF --steps 280 --svg s69.svg
../simEDF s77.system 0 --scheduler EDF --steps 100 --svg s77.svg
../simEDF s78.system 0 --scheduler LLF --steps 100 --svg s78.svg