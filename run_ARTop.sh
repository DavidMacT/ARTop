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
inputdir=${array[10]}/Data/AR_${array[0]}/input_images
outputdir=${array[10]}/Data/AR_${array[0]}/output


# make input files
mkdir -p $inputdir

# make output files
mkdir -p $outputdir
> $outputdir/regionData.dat


# download the magnetogram data
python $script_path/source/python/MagDown.py ${array[0]} ${array[1]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} $inputdir $outputdir

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
python $script_path/source/python/DAVE4vm.py ${array[0]} ${array[1]} ${array[2]} ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[7]} ${array[8]} $inputdir $outputdir ${array[9]}


#remove the initial download files
if [ "${array[14]}" == "true" ]; then
rm -rf $inputdir
fi

echo 'Calculating the Potential field..'
# calculate the potential fields
python $script_path/source/python/potentialbxby.py ${array[0]} 0 $endfl $nx $ny $outputdir

##-----------------------------------------------------------------------------------------------------------

if [ "${array[11]}" == "true" ]; then
cd $main_dir

echo '-------------------------------------- '
echo 'Computing the winding set potential..'

g++ -c -O3 -std=gnu++14 -o $src/c++/pointCheap.o  $src/c++/pointCheap.cpp -fopenmp

g++ -c -O3 -std=gnu++14 -o $src/c++/interpolation2Large.o $src/c++/interpolation2Large.cpp -fopenmp
g++ -c -O3 -std=gnu++14 -o $src/c++/biotSavartGauge.o $src/c++/biotSavartGauge.cpp -fopenmp

g++ -c -O3 -std=gnu++14 -std=gnu++14 -o $src/c++/mainObservationalWindingPotentialFast.o $src/c++/mainObservationalWindingPotentialFast.cpp -fopenmp

g++ -O3 -std=gnu++14 -o $src/c++/observationalWindingPotentialFast $src/c++/pointCheap.o $src/c++/interpolation2Large.o  $src/c++/biotSavartGauge.o $src/c++/mainObservationalWindingPotentialFast.o -fopenmp

# create file for the images
#mkdir -p ${array[10]}/Data/AR_${array[0]}/generated_images

cd $src/c++/

#The start and end files
startfl=0
end=$(($endfl - 1))
for i in $(seq $startfl $end)
do
echo $i
echo $outputdir/Ux_${array[0]}_${array[9]}_$i.txt
./observationalWindingPotentialFast $outputdir/Ux_${array[0]}_${array[9]}_$i.txt $outputdir/Uy_${array[0]}_${array[9]}_$i.txt $outputdir/Uz_${array[0]}_${array[9]}_$i.txt $outputdir/bx_${array[0]}_$i.txt $outputdir/by_${array[0]}_$i.txt $outputdir/bz_${array[0]}_$i.txt $nx $ny 360 360 ${array[12]} $outputdir/Bxp_${array[0]}_$i.txt $outputdir/Byp_${array[0]}_$i.txt $outputdir/windDatPotentialFastCO${array[12]}_VS${array[9]}_${array[13]}_$i.dat ${array[13]}


# plotting
#anaconda3-launch python $script_path/source/python/plotting.py $nx $ny $outputdir/windDatPotentialFastCO${array[12]}_VS${array[9]}_${array[13]}_$i.dat ${array[10]}/Data/AR_${array[0]}/generated_images

done
fi
