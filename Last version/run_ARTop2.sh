#!/bin/bash


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
inputdir=${array[11]}/Data/AR_${array[0]}/input_images
outputdir=${array[11]}/Data/AR_${array[0]}/output


# make input files
mkdir -p $inputdir

# make output files
mkdir -p $outputdir

# specifications
> $outputdir/specifications.txt
echo ${array[10]} >> $outputdir/specifications.txt
echo ${array[13]} >> $outputdir/specifications.txt
echo ${array[14]} >> $outputdir/specifications.txt



# download the magnetogram data
if [ "${array[1]}" == "true" ]; then
> $outputdir/regionData.dat

anaconda3-launch python $script_path/source/python/MagDown.py ${array[0]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} ${array[9]} $inputdir $outputdir
fi

# get region pixel numbers
ny=$(tail -1 $outputdir/regionData.dat)
nx=$(tail -2 $outputdir/regionData.dat | head -1)
echo 'x pixels= '$nx
echo 'y pixels= '$ny

#The number of downloaded files
endfl=$(ls $inputdir/ | grep "bx" | wc -l)
echo "Number of dumps: ${endfl}"
#endfl is the value of this last file


echo '  '
echo 'Calculating the velocity distributions..'
# calculate the velocity distributions
anaconda3-launch python $script_path/source/python/DAVE4vm.py ${array[0]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} ${array[9]} $inputdir $outputdir ${array[10]}


#remove the initial download files
if [ "${array[15]}" == "true" ]; then
rm -rf $inputdir
fi

echo 'Calculating the Potential field..'
# calculate the potential fields
anaconda3-launch python $script_path/source/python/potentialbxby.py ${array[0]} 0 $endfl $nx $ny $outputdir

##-----------------------------------------------------------------------------------------------------------

if [ "${array[12]}" == "true" ]; then
cd $main_dir

echo '-------------------------------------- '
echo 'Computing the winding set potential..'

g++ -c -O3 -std=gnu++14 -o $src/c++/pointCheap.o  $src/c++/pointCheap.cpp -fopenmp

g++ -c -O3 -std=gnu++14 -o $src/c++/interpolation2Large.o $src/c++/interpolation2Large.cpp -fopenmp
g++ -c -O3 -std=gnu++14 -o $src/c++/biotSavartGauge.o $src/c++/biotSavartGauge.cpp -fopenmp

g++ -c -O3 -std=gnu++14 -std=gnu++14 -o $src/c++/mainObservationalWindingPotentialFast.o $src/c++/mainObservationalWindingPotentialFast.cpp -fopenmp

g++ -O3 -std=gnu++14 -o $src/c++/observationalWindingPotentialFast $src/c++/pointCheap.o $src/c++/interpolation2Large.o  $src/c++/biotSavartGauge.o $src/c++/mainObservationalWindingPotentialFast.o -fopenmp

# create file for the images
mkdir -p ${array[11]}/Data/AR_${array[0]}/generated_images

cd $src/c++/

#The start and end files
startfl=0
end=$(($endfl - 1))
for i in $(seq $startfl $end)
do
echo $i
#echo $outputdir/Ux_${array[0]}_${array[10]}_$i.txt
./observationalWindingPotentialFast $outputdir/Ux_${array[0]}_${array[10]}_$i.txt $outputdir/Uy_${array[0]}_${array[10]}_$i.txt $outputdir/Uz_${array[0]}_${array[10]}_$i.txt $outputdir/bx_${array[0]}_$i.txt $outputdir/by_${array[0]}_$i.txt $outputdir/bz_${array[0]}_$i.txt $nx $ny 360 360 ${array[13]} $outputdir/Bxp_${array[0]}_$i.txt $outputdir/Byp_${array[0]}_$i.txt $outputdir/windDatPotentialFastCO${array[13]}_VS${array[10]}_${array[14]}_$i.dat ${array[14]}
done

#integrated variables
for i in $(seq $startfl $end)
do
tail -10 $outputdir/windDatPotentialFastCO${array[13]}_VS${array[10]}_${array[14]}_$i.dat >> $outputdir/netWindDatPotFast${array[13]}_VS${array[10]}_${array[14]}.dat
done

fi
