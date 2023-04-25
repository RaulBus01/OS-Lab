#!/bin/bash
red='\033[0;31m'
resetColor='\033[0m'
green='\033[0;32m'
blue='\033[0;34m'
if test $# -ne 1
then 
    echo -e "\n ${red} Please introduce a filename to be found ${resetColor}\n"
    exit 1
fi
filname=$1
if test -f $1
then 
    touch $1 
    echo -e "\n ${green} File ${blue} $1 ${green} already exists and was updated successfully ${resetColor}\n"
    else
    touch $1
    echo -e "\n  ${green} File ${blue} $1 ${green} was created successfully ${resetColor}\n"
fi