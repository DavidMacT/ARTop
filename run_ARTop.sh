#!/bin/bash

exec 2>/dev/null

script_path=`dirname "$0"`
script_path=`eval "cd \"$script_path\" && pwd"`
main_dir=$script_path/
src=$script_path/source

file=read_data.txt

# declaring array list and index iterator
declare -a array=()
i=0
# reading file in row mode, insert each line into array
while IFS="=""," read -r A B; do
    array[i]=$B
    let "i++"
# reading from file path
done < "$file"

cd
inputdir=${array[11]}
outputdir=${array[12]}

# specifications
> $outputdir/specifications.txt
echo ${array[10]} >> $outputdir/specifications.txt
echo ${array[14]} >> $outputdir/specifications.txt
echo ${array[15]} >> $outputdir/specifications.txt
echo 'start time' >> $outputdir/specifications.txt
echo ${array[2]}${array[3]}${array[4]}'T'${array[5]} >> $outputdir/specifications.txt
echo 'end time' >> $outputdir/specifications.txt
echo ${array[6]}${array[7]}${array[8]}'T'${array[9]} >> $outputdir/specifications.txt
python $script_path/source/python/numbering.py ${array[2]}  ${array[3]}  ${array[4]}  ${array[5]}  ${array[6]}  ${array[7]}  ${array[8]}  ${array[9]} $outputdir

# download the magnetogram data
if [ "${array[1]}" == "true" ]; then
    > $outputdir/regionData.dat
    
    python $script_path/source/python/MagDown.py ${array[0]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} ${array[9]} $inputdir $outputdir
fi

# get region pixel numbers
ny=$(tail -1 $outputdir/regionData.dat)
nx=$(tail -2 $outputdir/regionData.dat | head -1)
echo 'x pixels= '$nx
echo 'y pixels= '$ny

# the number of time steps for main loop
endfl=$(tail -1 $outputdir/specifications.txt | head -1)
echo "Number of time  dumps: ${endfl}"
#endfl is the value of this last file


echo '  '
echo 'Calculating the velocity distributions..'
# calculate the velocity distributions
python $script_path/source/python/DAVE4vm.py ${array[0]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} ${array[9]} $inputdir $outputdir ${array[10]}


#remove the initial download files
if [ "${array[16]}" == "true" ]; then
    rm -rf $inputdir
fi

echo 'Calculating the potential field..'
# calculate the potential fields
python $script_path/source/python/potentialbxby.py ${array[0]} 0 $endfl $nx $ny $outputdir

##----------------------------------------------------------------------------

if [ "${array[13]}" == "true" ]; then
    cd $main_dir

    # create file for the images
    mkdir -p ${array[12]}/generated_images

    echo '-------------------------------------- '
    echo 'Calculating topological quantities..'

    cd $src/c++/
    # compile the code
    chmod u+x compTopCode
    ./compTopCode

    # the start and end files
    startfl=0
    end=$(($endfl - 1))

    # run the code, loop through all the files
    for i in $(seq $startfl $end)
    do
	echo $i
	
	./ARTop $outputdir/Ux_${array[0]}_${array[10]}_$i.txt $outputdir/Uy_${array[0]}_${array[10]}_$i.txt $outputdir/Uz_${array[0]}_${array[10]}_$i.txt $outputdir/bx_${array[0]}_$i.txt $outputdir/by_${array[0]}_$i.txt $outputdir/bz_${array[0]}_$i.txt $nx $ny 360 360 ${array[14]} $outputdir/Bxp_${array[0]}_$i.txt $outputdir/Byp_${array[0]}_$i.txt $outputdir/windDatPotentialFastCO${array[14]}_VS${array[10]}_${array[15]}_$i.dat ${array[15]} >&2
    done

    #integrated variables
    for i in $(seq $startfl $end)
    do
	tail -10 $outputdir/windDatPotentialFastCO${array[14]}_VS${array[10]}_${array[15]}_$i.dat >> $outputdir/netWindDatPotFast${array[14]}_VS${array[10]}_${array[15]}.dat
    done

fi
