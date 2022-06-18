#!/bin/bash


script_path=`dirname "$0"`
script_path=`eval "cd \"$script_path\" && pwd"`
main_dir=$script_path/
# create batch_info first
src=$script_path/batch_info

number_of_files=2

for i in $(seq 1 $number_of_files)
do

chmod u+x run_ARTop.sh
./run_ARTop.sh -u $src/read_data_$i > out_batch &
done
