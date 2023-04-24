#!/bin/bash
if test $# -ne 2 
 then 
    echo "Usage: $0 <file>"
    exit 1
fi
errorReg="error:*"
warningsReg="warning:*"
count=0
if ! test -f $2 ||  test -h $2
then
    exit 2
fi

if test -f $1 || ! test -h $1  
then 
    gcc -Wall $1 2> $2      
    counterError=`cat $2 | grep -c "$errorReg"`
    counterWarnings=`cat $2 | grep -c "$warningsReg"`
    echo ""
    echo " Total number of errors and warnings in the file $1 :  Error -> $counterError Warnings -> $counterWarnings"
fi
