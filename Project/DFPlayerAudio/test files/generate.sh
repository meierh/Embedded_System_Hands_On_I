#!/usr/bin/env bash

# NOTE: This script utilises `say', which is only available on MacOS
#       Furthermore, `lame' must be installed

FOLDERS=2
FILES_PER_FOLDER=400

###

if [ -e out ]
then
  rm -rf out
fi

mkdir out
cd out

total=$(( FOLDERS * FILES_PER_FOLDER ))
current=0

for i_folder in $(seq 1 $FOLDERS)
do
  folder_name=$(printf "%02d" $i_folder)
  mkdir $folder_name
  cd $folder_name

  for i_file in $(seq 1 $FILES_PER_FOLDER)
  do
    file_name=$(printf "%04d" $i_file)
    message=$(printf "Folder %d, file %d." $i_folder $i_file)

    say "$message" -o ${file_name}.aif
    lame -b 128k --resample 48 --quiet ${file_name}.aif ${file_name}.mp3
    rm ${file_name}.aif

    (( current++ ))
    printf "\r%3d / %-3d" $current $total
  done

  cd ..
done

printf "\n"