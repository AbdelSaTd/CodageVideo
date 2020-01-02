#!/bin/bash

if [ $# -ne 1 ]
then
    echo name of the ouput file needed
    exit
fi

echo "ouput file: $1"
videos=( brandon2 bus )

touch $1

echo "Analysis file" >> $1
echo " " >> $1

bus_zip_compRate=0.32
bus_lzma_compRate=0.53

brandon2_zip_compRate=0.32
brandon2_lzma_compRate=0.54


for quant in Quant*
do
    echo "Quant folder: $quant"
    echo $quant >> $1
    cd $quant

    for vid in ${videos[@]}
    do
        echo "video: $vid" >> ../$1
        cd $vid
        nb_quant=`echo $quant | cut -d'_' -f2`
        enc_size=`stat --printf="%s" ${vid}_Enc_Q${nb_quant}.avi`
        echo "encoded : $enc_size bytes" >> ../../$1
        
        enc_zip_size=`stat --printf="%s" ${vid}_Enc_Q${nb_quant}.avi.zip`
        enc_lzma_size=`stat --printf="%s" ${vid}_Enc_Q${nb_quant}.avi.lzma`

        if [ $vid = "bus" ]
        then
            o_zip_cR=$bus_zip_compRate
            o_lzma_cR=$bus_lzma_compRate
        else
            o_zip_cR=$brandon2_zip_compRate
            o_lzma_cR=$brandon2_lzma_compRate
        fi


    #ZIP
        echo "scale=3; 1-($enc_zip_size/$enc_size)" > tmp.txt
        compRate=`bc < tmp.txt`

        echo "scale=3; $compRate-$o_zip_cR" > tmp.txt
        diffRate=`bc < tmp.txt`

        #echo "diffRate: $diffRate"
        if (( $(echo "$diffRate >= 0" | bc -l) ));
        then
            diffAdj="increase"
            #echo $diffAdj
        else
            diffAdj="decrease"
            #echo $diffAdj
        fi

        echo "encoded-compressed (zip): $enc_zip_size bytes -> CompRate: $compRate either $diffRate compression $diffAdj" >> ../../$1
    #end ZIP

    #LZMA

        echo "scale=3; 1-($enc_lzma_size/$enc_size)" > tmp.txt
        compRate=`bc < tmp.txt`

        echo "scale=3; $compRate-$o_lzma_cR" > tmp.txt
        diffRate=`bc < tmp.txt`

        rm tmp.txt

        #echo "diffRate: $diffRate"
        if (( $(echo "$diffRate >= 0" | bc -l) ));
        then
            diffAdj="increase"
            #echo $diffAdj
        else
            diffAdj="decrease"
            #echo $diffAdj
        fi

        echo "encoded-compressed (lzma): $enc_lzma_size bytes -> CompRate: $compRate either $diffRate compression $diffAdj" >> ../../$1

    #end LZMA

        cd ..
        printf "\n" >> ../$1
    done

    printf "\n" >> ../$1
    printf "\n" >> ../$1

    cd ..
done