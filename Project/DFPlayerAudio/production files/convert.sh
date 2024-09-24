#!/usr/bin/env bash

# NOTE: `lame' must be installed for this script to work

BASE_DIR="$(dirname "$(realpath "$0")")"

SRC=("${BASE_DIR}/mp3" "${BASE_DIR}/ringtones")

COUNT=( "$(ls -1 "${SRC[0]}" | wc -l)" "$(ls -1 "${SRC[1]}" | wc -l)" )
TOTAL_COUNT=$(( COUNT[0] + COUNT[1] ))

current=0

###

cd "${BASE_DIR}"

if [ -e out ]
then
  rm -rf out
fi

mkdir out
cd out

for i_folder in $(seq 0 1)
do
  folder_name=$(printf "%02d" $(( i_folder + 1 )) )
  mkdir $folder_name
  cd $folder_name

  i_file=1
  for file in "${SRC[i_folder]}"/*
  do
    file_name=$(printf "%04d" $i_file)
    lame -b 128k --resample 24 --quiet "$file" ${file_name}.mp3

    (( current++ ))
    (( i_file++ ))
    printf "\r%3d / %-3d" $current $TOTAL_COUNT
  done

  cd ..
done

printf "\n"
