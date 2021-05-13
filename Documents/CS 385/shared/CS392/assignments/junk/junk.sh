###############################################################################
# Author: Sanjana Madhu and Lasya Josyula
# Date: 02/16/2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Description: Recycling bin for files with the ability to list and purge.
###############################################################################
#!/bin/bash

help_flag=0
list_flag=0
purge_flag=0
no_args="true"

readonly DIRECTORY=~/.junk

displayHelp(){
cat << displayHelp
Usage: $(basename "$0") [-hlp] [list of files]
    -h: Display help.
    -l: List junked files.
    -p: Purge all files.
    [list of files] with no other arguments to junk those files.
displayHelp
}

displayTooMany(){
cat << displayTooMany
Error: Too many options enabled.
Usage: $(basename "$0") [-hlp] [list of files]
    -h: Display help.
    -l: List junked files.
    -p: Purge all files.
    [list of files] with no other arguments to junk those files.
displayTooMany
}

while getopts ":hlp" option; do
    case "$option" in
        h) help_flag=1
           ;;
        l) list_flag=1
           ;;
        p) purge_flag=1
           ;;
        ?) printf "Error: Unknown option '-$OPTARG'.\n" $OPTARG >&2
           displayHelp
           exit 1
           ;;
    esac
    no_args="false"
done

declare -a fileNames
shift "$((OPTIND-1))"
index=0
for f in $@; do
    fileNames[$index]="$f"
    (( ++index ))
done

if [[ "$no_args" == "true" || "$help_flag" -eq 1 ]]; then
    if [ ${#fileNames[*]} -eq 0 ]; then
    	displayHelp
    	exit 0
    fi
fi

count=$(( list_flag + purge_flag + help_flag ))

if [ $count -gt 1 ]; then
    displayTooMany
    exit 1
fi

if [[ ${#fileNames[*]} -ge 1 && $count -ge 1 ]]; then
    displayTooMany
    exit 1
fi

if [ ! -d "$DIRECTORY" ]; then
    mkdir "$DIRECTORY"
    exit 0
fi

if [ -d "$DIRECTORY" ]; then
    if [ "$help_flag" -eq 1 ]; then
        displayHelp
        exit 0
    fi

    if [ $list_flag -eq 1 ]; then
        ls -lAF "$DIRECTORY"
        exit 0
    fi

    if [ $purge_flag -eq 1 ]; then
        rm -rf $DIRECTORY/* 2>/dev/null
        rm -rf $DIRECTORY/.* 2>/dev/null
        exit 0
    fi
fi

for name in ${fileNames[@]}; do
    if [ ! -e "$name" ]; then
        printf "Warning: '$name' not found. \n"
        exit 1
    else
        mv $name $DIRECTORY
    fi
done

exit 0
