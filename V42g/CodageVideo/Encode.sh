#!/bin/sh


for i in 0 2 3
do
    mkdir "Quant$i"
    for vid in *_O.avi
    do
        #echo $vid 
        radical=`echo $vid | cut -d'_' -f1`
        #echo $radical
        ../v42.exe $vid "${radical}_Enc.avi" c $i
        ../v42.exe "${radical}_Enc.avi" "${radical}_DecSM.avi" D $i
        mv "${radical}_DecAM.avi" "Quant$i"
        #../v42.exe "{$radical}_Enc.avi" "${radical}_DecSM.avi" d $i

    done

done


for vid in *_O.avi
do
        #echo $vid 
        radical=`echo $vid | cut -d'_' -f1`
        #echo $radical
        ../v42.exe $vid "${radical}_Enc.avi" c $i
        ../v42.exe "${radical}_Enc.avi" "${radical}_DecSM.avi" D $i
        mv "${radical}_DecAM.avi" "Quant$i"
        #../v42.exe "{$radical}_Enc.avi" "${radical}_DecSM.avi" d $i

done
