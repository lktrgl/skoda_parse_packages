#!/bin/bash

my_dir=$( dirname $0 )

my_pref="$(dirname $0)"
my_file_type="-iname '*.cpp' -or -iname '*.c' -or -iname '*.h' -or -iname '*.hpp'"
my_find_cmd="find ${my_pref} -type f -and \\( $my_file_type \\)"
my_fmt_cmd="astyle"
my_cmd="$my_find_cmd | xargs -n 1 $my_fmt_cmd"


echo
echo " cmd: '$my_cmd' "
echo

eval "$my_cmd"


echo
echo " spec processing (beauty - undo) "
echo

my_cmd="cd ${my_dir}/contrib/rapidjson && git status && git reset --hard HEAD"

echo " cmd: '$my_cmd' "
echo

eval "$my_cmd"
