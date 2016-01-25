#!/bin/bash
# Takes no params, must be called from test dir

bd=`pwd | sed 's/-test.*//'`
pr=`pwd | sed 's/.*PR//'`
rA=testPR$pr; rB=orig;
source ~/tools/validateJR.sh ${bd}-${rA} ${bd}-${rB} ${rA}VS${rB} ~/tools/matrix_70X.txt |& tee ${rA}VS${rB}.log 
