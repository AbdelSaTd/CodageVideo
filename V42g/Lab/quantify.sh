#!/bin/bash


dirname=${PWD##*/}
dirname=`printf '%q\n' "${PWD##*/}"`
cd ..
make
cd $dirname

echo " "

echo "Starting of quantifications"


quants=( 3 )
quant_names=( DoubleStep8-128 )
compressors=( zip lzma )
compressors_ext=( zip lzma )

if [ ${#quants[@]} -ne ${#quant_names[@]} -o ${#compressors[@]} -ne ${#compressors_ext[@]} ] 
then
    echo "Error: Incoherences in the arrays parameters"
    exit
fi

# Need to be write depending of the usage of the compressing program to release a file
# fil.ext : where ext is the extention of the compressing program given in the array compressors_ext
compress(){
    echo compress $1 $2
    echo "algo: $1"
    echo "file: $2"

    if [ $1 = "zip" ]
    then
        zip "$2.zip" $2

    elif [ $1 = "lzma" ]  
    then
        lzma -k $2
    else
        echo "$1 : Unknown algorithm of compression"
    fi
}


k=0
for i in "${quants[@]}"
do
    echo "Quantification(${i}): ${quant_names[$k]}"
    echo " "

    mkdir "Quant_${i}_${quant_names[$k]}"
    dst_fold="Quant_${i}_${quant_names[$k]}"
    for vid in *_O.avi
    do
        radical=`echo $vid | cut -d'_' -f1`

        cd $dst_fold
        mkdir $radical
        cd ..

        ../v42.exe $vid "${radical}_Enc_Q$i.avi" c $i
        ../v42.exe "${radical}_Enc_Q$i.avi" "${radical}_DecSVM_Q$i.avi" d $i
        ../v42.exe "${radical}_Enc_Q$i.avi" "${radical}_DecAVM_Q$i.avi" d $i

        gen_vids=( "${radical}_Enc_Q$i.avi" "${radical}_DecSVM_Q$i.avi" "${radical}_DecAVM_Q$i.avi" )
        
        for vidComp in "${gen_vids[@]}"
        do
            j=0
            for comp in "${compressors[@]}"
            do
                compress $comp $vidComp
                mv "$vidComp.${compressors_ext[$j]}" "$dst_fold/$radical"
                j=$(($j+1))
            done
        done

        mv ${gen_vids[@]} "$dst_fold/$radical"
       
    done
    k=$(($k+1))
done

# Compress originals videos
#for vid in *_O.avi
#do
#    for comp in "${compressors[@]}"
#    do
#        compress $comp $vid
#    done
#done

cd ..
./v42.exe j


